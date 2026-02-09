#!/usr/bin/env python3
"""
QuickTune Goertzel Detection Prototype
======================================

Validates the QuickTune room correction algorithm using:
- Stepped sine tones at EQ10 band center frequencies
- Goertzel algorithm for single-frequency energy detection
- MEMS microphone calibration compensation
- Biquad parametric EQ correction computation
- Multi-room validation scenarios

Author: DSP Team (Prototype Agent)
Date: 2026-02-09
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy import signal
from typing import Tuple, List, Dict
import os


# ============================================================================
# CONSTANTS
# ============================================================================

FS = 48000  # Sample rate (Hz)
TONE_SETTLING_MS = 200  # Settling time (ms)
TONE_ANALYSIS_MS = 100  # Analysis window (ms)
TONE_TOTAL_MS = TONE_SETTLING_MS + TONE_ANALYSIS_MS  # 300 ms per band

# EQ10 band center frequencies (Hz)
EQ10_BANDS = np.array([25, 40, 63, 100, 160, 250, 400, 630, 1000, 1600])

# MEMS calibration offsets (dB) - compensate for MEMS roll-off
MEMS_CALIBRATION = {
    25: 3.0,
    40: 1.5,
    63: 0.0,
    100: 0.0,
    160: 0.0,
    250: 0.0,
    400: 0.0,
    630: 0.0,
    1000: 0.0,
    1600: 0.0
}

# QuickTune parameters
GAIN_RANGE_DB = 12.0  # ±12 dB correction range
EQ_Q = 2.0  # Fixed Q for all correction bands
TARGET_ACCURACY_DB = 1.0  # Target: ±1 dB accuracy
MAX_ITERATIONS = 3  # Maximum number of iterative refinements


# ============================================================================
# TONE GENERATOR
# ============================================================================

def generate_sine_tone(frequency: float, duration_ms: float) -> np.ndarray:
    """Generate sine tone at specified frequency.

    Args:
        frequency: Tone frequency (Hz)
        duration_ms: Duration (ms)

    Returns:
        Audio signal (float32, -1.0 to +1.0)
    """
    num_samples = int(FS * duration_ms / 1000)
    t = np.arange(num_samples) / FS
    tone = np.sin(2 * np.pi * frequency * t)

    # Apply fade in/out to reduce clicks (10ms)
    fade_samples = int(0.01 * FS)
    fade_in = np.linspace(0, 1, fade_samples)
    fade_out = np.linspace(1, 0, fade_samples)

    tone[:fade_samples] *= fade_in
    tone[-fade_samples:] *= fade_out

    return tone.astype(np.float32)


# ============================================================================
# GOERTZEL ALGORITHM
# ============================================================================

def goertzel_power(signal_data: np.ndarray, target_freq: float) -> float:
    """Compute power at target frequency using Goertzel algorithm.

    Args:
        signal_data: Input signal
        target_freq: Target frequency (Hz)

    Returns:
        Power at target frequency
    """
    N = len(signal_data)
    k = int(0.5 + (N * target_freq / FS))  # Bin number
    w = (2.0 * np.pi * k) / N
    coeff = 2.0 * np.cos(w)

    # Goertzel iteration
    s1, s2 = 0.0, 0.0
    for sample in signal_data:
        s0 = coeff * s1 - s2 + sample
        s2 = s1
        s1 = s0

    # Compute power
    power = s1**2 + s2**2 - coeff * s1 * s2

    return power


def measure_frequency_response_goertzel(signal_data: np.ndarray,
                                        frequency: float) -> float:
    """Measure frequency response at specific frequency using Goertzel.

    Args:
        signal_data: Recorded signal
        frequency: Frequency to measure (Hz)

    Returns:
        Level in dB relative to unit amplitude sine wave
    """
    # Use analysis window only (skip settling time)
    settling_samples = int(FS * TONE_SETTLING_MS / 1000)
    analysis_samples = int(FS * TONE_ANALYSIS_MS / 1000)
    analysis_window = signal_data[settling_samples:settling_samples + analysis_samples]

    # Compute power using Goertzel
    power = goertzel_power(analysis_window, frequency)

    # Goertzel power output is proportional to (amplitude)^2 * N^2 / 2
    # For unit amplitude sine: expected_power = N^2 / 2
    # So we normalize: magnitude = sqrt(2 * power) / N
    N = len(analysis_window)
    magnitude = np.sqrt(2.0 * power) / N

    # Convert to dB relative to unit amplitude (A=1.0)
    if magnitude > 1e-9:  # Avoid log(0)
        level_db = 20 * np.log10(magnitude)
    else:
        level_db = -120.0  # Floor

    return level_db


# ============================================================================
# BIQUAD FILTER DESIGN
# ============================================================================

def design_parametric_eq_biquad(fc: float, gain_db: float, Q: float) -> Tuple[np.ndarray, np.ndarray]:
    """Design parametric EQ biquad using RBJ cookbook.

    Args:
        fc: Center frequency (Hz)
        gain_db: Gain in dB (positive = boost, negative = cut)
        Q: Quality factor

    Returns:
        Tuple of (b_coeffs, a_coeffs) for scipy.signal.lfilter
    """
    A = 10**(gain_db / 40.0)
    w0 = 2 * np.pi * fc / FS
    alpha = np.sin(w0) / (2 * Q)

    b0 = 1 + alpha * A
    b1 = -2 * np.cos(w0)
    b2 = 1 - alpha * A
    a0 = 1 + alpha / A
    a1 = -2 * np.cos(w0)
    a2 = 1 - alpha / A

    # Normalize by a0
    b = np.array([b0/a0, b1/a0, b2/a0])
    a = np.array([1.0, a1/a0, a2/a0])

    return b, a


def apply_biquad_cascade(signal_data: np.ndarray, biquads: List[Tuple[np.ndarray, np.ndarray]]) -> np.ndarray:
    """Apply cascade of biquad filters.

    Args:
        signal_data: Input signal
        biquads: List of (b, a) coefficient tuples

    Returns:
        Filtered signal
    """
    output = signal_data.copy()
    for b, a in biquads:
        output = signal.lfilter(b, a, output)
    return output


# ============================================================================
# ROOM SIMULATOR
# ============================================================================

class RoomSimulator:
    """Simulate room acoustics using biquad filters."""

    def __init__(self, name: str, modes: List[Dict]):
        """Initialize room simulator.

        Args:
            name: Room name
            modes: List of room modes, each dict with keys:
                   'freq' (Hz), 'gain_db' (dB), 'Q' (quality factor)
        """
        self.name = name
        self.modes = modes
        self.biquads = []

        # Design biquad filters for each mode
        for mode in modes:
            b, a = design_parametric_eq_biquad(
                mode['freq'], mode['gain_db'], mode['Q']
            )
            self.biquads.append((b, a))

    def process(self, signal_data: np.ndarray) -> np.ndarray:
        """Process signal through room simulator.

        Args:
            signal_data: Input signal (clean)

        Returns:
            Signal with room acoustics applied
        """
        # Apply room modes
        output = apply_biquad_cascade(signal_data, self.biquads)

        # Add noise floor (-60 dB SNR)
        noise_power = 10**(-60/20) * np.sqrt(np.mean(output**2))
        noise = np.random.normal(0, noise_power, len(output))
        output += noise

        return output


def create_room_scenarios() -> List[RoomSimulator]:
    """Create test room scenarios.

    Returns:
        List of RoomSimulator objects
    """
    rooms = []

    # Room 1: Strong bass buildup (modes at 50 Hz, 80 Hz)
    rooms.append(RoomSimulator(
        name="Room 1: Strong Bass Buildup",
        modes=[
            {'freq': 50, 'gain_db': 8.0, 'Q': 3.0},
            {'freq': 80, 'gain_db': 6.0, 'Q': 2.5},
            {'freq': 35, 'gain_db': -3.0, 'Q': 1.0},  # Roll-off
        ]
    ))

    # Room 2: Bass null (cancellation at 100 Hz)
    rooms.append(RoomSimulator(
        name="Room 2: Bass Null",
        modes=[
            {'freq': 100, 'gain_db': -8.0, 'Q': 2.0},
            {'freq': 60, 'gain_db': 3.0, 'Q': 1.5},
            {'freq': 150, 'gain_db': 2.0, 'Q': 1.5},
        ]
    ))

    # Room 3: Moderate room (typical small studio)
    rooms.append(RoomSimulator(
        name="Room 3: Moderate Room",
        modes=[
            {'freq': 55, 'gain_db': 4.0, 'Q': 2.0},
            {'freq': 110, 'gain_db': -3.0, 'Q': 1.5},
            {'freq': 220, 'gain_db': 2.0, 'Q': 1.5},
        ]
    ))

    # Room 4: Flat room (minimal correction needed)
    rooms.append(RoomSimulator(
        name="Room 4: Flat Room",
        modes=[
            {'freq': 70, 'gain_db': 1.5, 'Q': 1.0},
            {'freq': 140, 'gain_db': -1.0, 'Q': 1.0},
        ]
    ))

    # Room 5: Severe room (multiple modes, large deviations)
    rooms.append(RoomSimulator(
        name="Room 5: Severe Room",
        modes=[
            {'freq': 45, 'gain_db': 10.0, 'Q': 3.5},
            {'freq': 90, 'gain_db': -6.0, 'Q': 2.0},
            {'freq': 135, 'gain_db': 7.0, 'Q': 2.5},
            {'freq': 180, 'gain_db': -4.0, 'Q': 1.5},
            {'freq': 270, 'gain_db': 5.0, 'Q': 2.0},
        ]
    ))

    return rooms


# ============================================================================
# QUICKTUNE ENGINE
# ============================================================================

def quicktune_measure_room(room: RoomSimulator) -> Tuple[np.ndarray, np.ndarray]:
    """Measure room frequency response using QuickTune.

    Args:
        room: RoomSimulator object

    Returns:
        Tuple of (frequencies, measured_levels_db)
    """
    measured_levels = []

    for freq in EQ10_BANDS:
        # Generate test tone
        tone = generate_sine_tone(freq, TONE_TOTAL_MS)

        # Pass through room
        recorded = room.process(tone)

        # Measure using Goertzel
        level_db = measure_frequency_response_goertzel(recorded, freq)

        # Apply MEMS calibration
        mems_correction = MEMS_CALIBRATION[freq]
        calibrated_level = level_db + mems_correction

        measured_levels.append(calibrated_level)

    return EQ10_BANDS, np.array(measured_levels)


def quicktune_compute_correction(measured_levels_db: np.ndarray,
                                 target_db: float = 0.0) -> np.ndarray:
    """Compute correction gains for EQ10.

    Args:
        measured_levels_db: Measured levels at each EQ10 band (dB)
        target_db: Target level (dB, typically 0.0 for flat)

    Returns:
        Correction gains (dB) for each EQ10 band
    """
    # Correction gain = target - measured
    correction_gains = target_db - measured_levels_db

    # Clip to ±12 dB
    correction_gains = np.clip(correction_gains, -GAIN_RANGE_DB, GAIN_RANGE_DB)

    return correction_gains


def quicktune_apply_correction(room: RoomSimulator,
                                correction_gains: np.ndarray) -> Tuple[np.ndarray, np.ndarray]:
    """Apply QuickTune correction and measure post-correction response.

    Args:
        room: RoomSimulator object
        correction_gains: Correction gains (dB) for each EQ10 band

    Returns:
        Tuple of (frequencies, post_correction_levels_db)
    """
    # Build EQ10 biquad cascade
    eq10_biquads = []
    for freq, gain_db in zip(EQ10_BANDS, correction_gains):
        b, a = design_parametric_eq_biquad(freq, gain_db, EQ_Q)
        eq10_biquads.append((b, a))

    # Measure post-correction response
    post_correction_levels = []

    for freq in EQ10_BANDS:
        # Generate test tone
        tone = generate_sine_tone(freq, TONE_TOTAL_MS)

        # Pass through room
        recorded = room.process(tone)

        # Apply EQ10 correction
        corrected = apply_biquad_cascade(recorded, eq10_biquads)

        # Measure using Goertzel
        level_db = measure_frequency_response_goertzel(corrected, freq)

        # Apply MEMS calibration
        mems_correction = MEMS_CALIBRATION[freq]
        calibrated_level = level_db + mems_correction

        post_correction_levels.append(calibrated_level)

    return EQ10_BANDS, np.array(post_correction_levels)


# ============================================================================
# VALIDATION
# ============================================================================

def validate_quicktune(room: RoomSimulator, plot_dir: str) -> Dict:
    """Validate QuickTune for a single room scenario with iterative refinement.

    Args:
        room: RoomSimulator object
        plot_dir: Directory to save plots

    Returns:
        Validation results dictionary
    """
    print(f"\n{'='*70}")
    print(f"Testing: {room.name}")
    print(f"{'='*70}")

    # Step 1: Measure room response
    print("Step 1: Measuring room response...")
    freqs, before_levels = quicktune_measure_room(room)

    print(f"Measured levels (dB):")
    for f, level in zip(freqs, before_levels):
        print(f"  {f:5.0f} Hz: {level:+6.2f} dB")

    # Step 2: Iterative correction
    print("\nStep 2: Computing correction gains (iterative)...")
    correction_gains = quicktune_compute_correction(before_levels)
    cumulative_gains = correction_gains.copy()

    print(f"Initial correction gains (dB):")
    for f, gain in zip(freqs, correction_gains):
        print(f"  {f:5.0f} Hz: {gain:+6.2f} dB")

    # Iterative refinement
    for iteration in range(1, MAX_ITERATIONS):
        print(f"\nIteration {iteration}: Measuring and refining...")

        # Measure with current correction
        freqs, current_levels = quicktune_apply_correction(room, cumulative_gains)
        residual_error = current_levels - 0.0
        max_error = np.max(np.abs(residual_error))

        print(f"  Max error after iteration {iteration}: {max_error:.2f} dB")

        # Check if we've converged
        if max_error <= TARGET_ACCURACY_DB:
            print(f"  Converged! Target accuracy achieved.")
            break

        # Compute refinement (with reduced gain to avoid oscillation)
        refinement = quicktune_compute_correction(current_levels)
        refinement *= 0.7  # Apply damping factor to prevent over-correction

        # Update cumulative gains (clip to range)
        cumulative_gains += refinement
        cumulative_gains = np.clip(cumulative_gains, -GAIN_RANGE_DB, GAIN_RANGE_DB)

    # Step 3: Final measurement with converged gains
    print("\nStep 3: Final measurement with converged correction...")
    freqs, after_levels = quicktune_apply_correction(room, cumulative_gains)

    print(f"Final correction gains (dB):")
    for f, gain in zip(freqs, cumulative_gains):
        print(f"  {f:5.0f} Hz: {gain:+6.2f} dB")

    print(f"\nPost-correction levels (dB):")
    for f, level in zip(freqs, after_levels):
        print(f"  {f:5.0f} Hz: {level:+6.2f} dB")

    # Step 4: Compute residual error
    residual_error = after_levels - 0.0  # Target is 0 dB
    max_error = np.max(np.abs(residual_error))
    rms_error = np.sqrt(np.mean(residual_error**2))

    print(f"\nFinal Residual Error:")
    print(f"  Max Absolute Error: {max_error:.2f} dB")
    print(f"  RMS Error:          {rms_error:.2f} dB")

    # Pass/fail criteria
    passed = max_error <= TARGET_ACCURACY_DB
    print(f"\nValidation: {'PASS' if passed else 'FAIL'}")
    print(f"  Target: ±{TARGET_ACCURACY_DB} dB")
    print(f"  Achieved: ±{max_error:.2f} dB")

    # Generate plots
    plot_room_correction(room.name, freqs, before_levels, after_levels,
                        cumulative_gains, residual_error, plot_dir)

    return {
        'room': room.name,
        'before_levels': before_levels,
        'after_levels': after_levels,
        'correction_gains': cumulative_gains,
        'residual_error': residual_error,
        'max_error': max_error,
        'rms_error': rms_error,
        'passed': passed
    }


def plot_room_correction(room_name: str, freqs: np.ndarray,
                        before_levels: np.ndarray, after_levels: np.ndarray,
                        correction_gains: np.ndarray, residual_error: np.ndarray,
                        plot_dir: str):
    """Generate before/after comparison plots.

    Args:
        room_name: Room name
        freqs: Frequencies (Hz)
        before_levels: Before correction levels (dB)
        after_levels: After correction levels (dB)
        correction_gains: Correction gains (dB)
        residual_error: Residual error (dB)
        plot_dir: Directory to save plots
    """
    fig, axes = plt.subplots(2, 2, figsize=(14, 10))
    fig.suptitle(f'QuickTune Validation: {room_name}', fontsize=14, fontweight='bold')

    # Plot 1: Before/After Frequency Response
    ax1 = axes[0, 0]
    ax1.semilogx(freqs, before_levels, 'o-', color='red', linewidth=2,
                 markersize=8, label='Before Correction')
    ax1.semilogx(freqs, after_levels, 's-', color='green', linewidth=2,
                 markersize=8, label='After Correction')
    ax1.axhline(0, color='black', linestyle='--', linewidth=1, label='Target (0 dB)')
    ax1.fill_between(freqs, -TARGET_ACCURACY_DB, TARGET_ACCURACY_DB,
                     color='green', alpha=0.2, label=f'±{TARGET_ACCURACY_DB} dB Target')
    ax1.set_xlabel('Frequency (Hz)', fontsize=11)
    ax1.set_ylabel('Level (dB)', fontsize=11)
    ax1.set_title('Frequency Response', fontsize=12, fontweight='bold')
    ax1.grid(True, which='both', alpha=0.3)
    ax1.legend(loc='best', fontsize=9)
    ax1.set_xlim([20, 2000])
    ax1.set_ylim([-15, 15])

    # Plot 2: Correction Gains
    ax2 = axes[0, 1]
    colors = ['red' if abs(g) > GAIN_RANGE_DB*0.8 else 'orange' if abs(g) > GAIN_RANGE_DB*0.5 else 'green'
              for g in correction_gains]
    ax2.bar(range(len(freqs)), correction_gains, color=colors, alpha=0.7, edgecolor='black')
    ax2.axhline(0, color='black', linestyle='-', linewidth=1)
    ax2.axhline(GAIN_RANGE_DB, color='red', linestyle='--', linewidth=1, label=f'±{GAIN_RANGE_DB} dB Limit')
    ax2.axhline(-GAIN_RANGE_DB, color='red', linestyle='--', linewidth=1)
    ax2.set_xlabel('EQ10 Band', fontsize=11)
    ax2.set_ylabel('Correction Gain (dB)', fontsize=11)
    ax2.set_title('EQ10 Correction Gains', fontsize=12, fontweight='bold')
    ax2.set_xticks(range(len(freqs)))
    ax2.set_xticklabels([f'{int(f)}' for f in freqs], rotation=45, ha='right', fontsize=9)
    ax2.grid(True, axis='y', alpha=0.3)
    ax2.legend(loc='best', fontsize=9)
    ax2.set_ylim([-GAIN_RANGE_DB-2, GAIN_RANGE_DB+2])

    # Plot 3: Residual Error
    ax3 = axes[1, 0]
    colors = ['green' if abs(e) <= TARGET_ACCURACY_DB else 'orange' if abs(e) <= 2.0 else 'red'
              for e in residual_error]
    ax3.bar(range(len(freqs)), residual_error, color=colors, alpha=0.7, edgecolor='black')
    ax3.axhline(0, color='black', linestyle='-', linewidth=1)
    ax3.axhline(TARGET_ACCURACY_DB, color='green', linestyle='--', linewidth=1, label=f'±{TARGET_ACCURACY_DB} dB Target')
    ax3.axhline(-TARGET_ACCURACY_DB, color='green', linestyle='--', linewidth=1)
    ax3.set_xlabel('EQ10 Band', fontsize=11)
    ax3.set_ylabel('Residual Error (dB)', fontsize=11)
    ax3.set_title('Post-Correction Residual Error', fontsize=12, fontweight='bold')
    ax3.set_xticks(range(len(freqs)))
    ax3.set_xticklabels([f'{int(f)}' for f in freqs], rotation=45, ha='right', fontsize=9)
    ax3.grid(True, axis='y', alpha=0.3)
    ax3.legend(loc='best', fontsize=9)
    ax3.set_ylim([-5, 5])

    # Plot 4: Summary Stats
    ax4 = axes[1, 1]
    ax4.axis('off')

    max_error = np.max(np.abs(residual_error))
    rms_error = np.sqrt(np.mean(residual_error**2))
    passed = max_error <= TARGET_ACCURACY_DB

    stats_text = f"""
    VALIDATION SUMMARY
    {'='*40}

    Room: {room_name}

    Before Correction:
      Mean Level:     {np.mean(before_levels):+6.2f} dB
      Std Dev:        {np.std(before_levels):6.2f} dB
      Range:          {np.min(before_levels):+6.2f} to {np.max(before_levels):+6.2f} dB

    After Correction:
      Mean Level:     {np.mean(after_levels):+6.2f} dB
      Std Dev:        {np.std(after_levels):6.2f} dB
      Range:          {np.min(after_levels):+6.2f} to {np.max(after_levels):+6.2f} dB

    Residual Error:
      Max Abs Error:  {max_error:6.2f} dB
      RMS Error:      {rms_error:6.2f} dB

    Target:           ±{TARGET_ACCURACY_DB:.1f} dB

    STATUS:           {'PASS' if passed else 'FAIL'}
    """

    ax4.text(0.1, 0.5, stats_text, fontsize=10, family='monospace',
             verticalalignment='center',
             bbox=dict(boxstyle='round', facecolor='lightblue' if passed else 'lightcoral', alpha=0.3))

    plt.tight_layout()

    # Save plot
    filename = room_name.replace(' ', '_').replace(':', '')
    filepath = os.path.join(plot_dir, f'{filename}.png')
    plt.savefig(filepath, dpi=150, bbox_inches='tight')
    print(f"  Plot saved: {filepath}")
    plt.close()


def plot_summary(results: List[Dict], plot_dir: str):
    """Generate summary comparison plots for all rooms.

    Args:
        results: List of validation result dictionaries
        plot_dir: Directory to save plots
    """
    fig, axes = plt.subplots(2, 2, figsize=(14, 10))
    fig.suptitle('QuickTune Multi-Room Validation Summary', fontsize=14, fontweight='bold')

    # Plot 1: Max Error Comparison
    ax1 = axes[0, 0]
    room_names = [r['room'].split(':')[0] for r in results]
    max_errors = [r['max_error'] for r in results]
    colors = ['green' if e <= TARGET_ACCURACY_DB else 'red' for e in max_errors]
    ax1.bar(range(len(results)), max_errors, color=colors, alpha=0.7, edgecolor='black')
    ax1.axhline(TARGET_ACCURACY_DB, color='green', linestyle='--', linewidth=2,
                label=f'Target: ±{TARGET_ACCURACY_DB} dB')
    ax1.set_xlabel('Room Scenario', fontsize=11)
    ax1.set_ylabel('Max Absolute Error (dB)', fontsize=11)
    ax1.set_title('Maximum Error per Room', fontsize=12, fontweight='bold')
    ax1.set_xticks(range(len(results)))
    ax1.set_xticklabels(room_names, rotation=45, ha='right', fontsize=9)
    ax1.grid(True, axis='y', alpha=0.3)
    ax1.legend(loc='best', fontsize=9)

    # Plot 2: RMS Error Comparison
    ax2 = axes[0, 1]
    rms_errors = [r['rms_error'] for r in results]
    colors = ['green' if e <= TARGET_ACCURACY_DB/2 else 'orange' if e <= TARGET_ACCURACY_DB else 'red'
              for e in rms_errors]
    ax2.bar(range(len(results)), rms_errors, color=colors, alpha=0.7, edgecolor='black')
    ax2.axhline(TARGET_ACCURACY_DB/2, color='green', linestyle='--', linewidth=2,
                label=f'Good: <{TARGET_ACCURACY_DB/2} dB')
    ax2.set_xlabel('Room Scenario', fontsize=11)
    ax2.set_ylabel('RMS Error (dB)', fontsize=11)
    ax2.set_title('RMS Error per Room', fontsize=12, fontweight='bold')
    ax2.set_xticks(range(len(results)))
    ax2.set_xticklabels(room_names, rotation=45, ha='right', fontsize=9)
    ax2.grid(True, axis='y', alpha=0.3)
    ax2.legend(loc='best', fontsize=9)

    # Plot 3: Residual Error Distribution (Histogram)
    ax3 = axes[1, 0]
    all_residuals = np.concatenate([r['residual_error'] for r in results])
    ax3.hist(all_residuals, bins=20, color='skyblue', edgecolor='black', alpha=0.7)
    ax3.axvline(-TARGET_ACCURACY_DB, color='green', linestyle='--', linewidth=2)
    ax3.axvline(TARGET_ACCURACY_DB, color='green', linestyle='--', linewidth=2,
                label=f'±{TARGET_ACCURACY_DB} dB Target')
    ax3.axvline(0, color='black', linestyle='-', linewidth=1)
    ax3.set_xlabel('Residual Error (dB)', fontsize=11)
    ax3.set_ylabel('Count', fontsize=11)
    ax3.set_title('Residual Error Distribution (All Rooms)', fontsize=12, fontweight='bold')
    ax3.grid(True, axis='y', alpha=0.3)
    ax3.legend(loc='best', fontsize=9)

    # Plot 4: Pass/Fail Summary
    ax4 = axes[1, 1]
    ax4.axis('off')

    num_passed = sum(r['passed'] for r in results)
    num_failed = len(results) - num_passed
    pass_rate = 100.0 * num_passed / len(results)

    avg_max_error = np.mean([r['max_error'] for r in results])
    avg_rms_error = np.mean([r['rms_error'] for r in results])

    summary_text = f"""
    OVERALL VALIDATION SUMMARY
    {'='*45}

    Total Rooms Tested:     {len(results)}
    Passed:                 {num_passed} ({pass_rate:.0f}%)
    Failed:                 {num_failed}

    Average Max Error:      {avg_max_error:.2f} dB
    Average RMS Error:      {avg_rms_error:.2f} dB

    Target Accuracy:        ±{TARGET_ACCURACY_DB:.1f} dB

    Room Results:
    """

    for r in results:
        status = 'PASS' if r['passed'] else 'FAIL'
        room_short = r['room'].split(':')[0]
        summary_text += f"      {room_short:8s}: {status:4s}  (max: {r['max_error']:.2f} dB)\n"

    summary_text += f"\n    OVERALL STATUS:         {'PASS' if num_failed == 0 else 'PARTIAL' if num_passed > 0 else 'FAIL'}"

    bg_color = 'lightgreen' if num_failed == 0 else 'lightyellow' if num_passed > 0 else 'lightcoral'

    ax4.text(0.1, 0.5, summary_text, fontsize=10, family='monospace',
             verticalalignment='center',
             bbox=dict(boxstyle='round', facecolor=bg_color, alpha=0.3))

    plt.tight_layout()

    # Save plot
    filepath = os.path.join(plot_dir, 'summary.png')
    plt.savefig(filepath, dpi=150, bbox_inches='tight')
    print(f"\nSummary plot saved: {filepath}")
    plt.close()


# ============================================================================
# MAIN
# ============================================================================

def main():
    """Main validation entry point."""
    print("="*70)
    print("QuickTune Goertzel Detection Prototype")
    print("="*70)
    print(f"Sample Rate:        {FS} Hz")
    print(f"Tone Duration:      {TONE_TOTAL_MS} ms ({TONE_SETTLING_MS} ms settling + {TONE_ANALYSIS_MS} ms analysis)")
    print(f"Total Cal Time:     ~{TONE_TOTAL_MS * len(EQ10_BANDS) / 1000:.1f} seconds")
    print(f"EQ10 Bands:         {len(EQ10_BANDS)} bands")
    print(f"Correction Range:   ±{GAIN_RANGE_DB} dB")
    print(f"Fixed Q:            {EQ_Q}")
    print(f"Target Accuracy:    ±{TARGET_ACCURACY_DB} dB")

    # Create room scenarios
    rooms = create_room_scenarios()
    print(f"\nRoom Scenarios:     {len(rooms)} rooms")

    # Run validation for all rooms
    plot_dir = '/Users/jasonho610/Desktop/pg-dsp-studio-monitor/prototypes/quicktune_goertzel/plots'
    results = []

    for room in rooms:
        result = validate_quicktune(room, plot_dir)
        results.append(result)

    # Generate summary plots
    plot_summary(results, plot_dir)

    # Final summary
    print("\n" + "="*70)
    print("VALIDATION COMPLETE")
    print("="*70)

    num_passed = sum(r['passed'] for r in results)
    num_failed = len(results) - num_passed
    pass_rate = 100.0 * num_passed / len(results)

    print(f"\nTotal Rooms:  {len(results)}")
    print(f"Passed:       {num_passed} ({pass_rate:.0f}%)")
    print(f"Failed:       {num_failed}")

    print("\nPer-Room Results:")
    for r in results:
        status = 'PASS' if r['passed'] else 'FAIL'
        print(f"  {r['room']:40s}: {status:4s}  (max error: {r['max_error']:.2f} dB, RMS: {r['rms_error']:.2f} dB)")

    if num_failed == 0:
        print("\nOVERALL STATUS: PASS")
        print("QuickTune algorithm is ready for implementation.")
    elif num_passed > 0:
        print("\nOVERALL STATUS: PARTIAL")
        print("QuickTune algorithm works for most scenarios, but needs refinement.")
    else:
        print("\nOVERALL STATUS: FAIL")
        print("QuickTune algorithm requires significant improvements.")

    print(f"\nPlots saved to: {plot_dir}")
    print("="*70)


if __name__ == '__main__':
    main()
