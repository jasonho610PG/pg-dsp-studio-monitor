# QuickTune Goertzel Detection Prototype

**Validation prototype for QuickTune room correction using Stepped Sine + Goertzel algorithm.**

## Overview

This prototype validates the QuickTune algorithm for automatic room correction. The system:

1. Plays sine tones at each EQ10 band center frequency
2. Captures room response via simulated MEMS microphone
3. Uses Goertzel algorithm for single-frequency energy detection
4. Applies MEMS calibration compensation
5. Computes corrective EQ10 gains with iterative refinement
6. Validates correction accuracy across multiple room scenarios

## Algorithm Components

### 1. Goertzel Algorithm

Single-frequency DFT for efficient tone detection:

```
k = round(f_target * N / fs)
coeff = 2 * cos(2 * pi * k / N)

For each sample:
  s0 = coeff * s1 - s2 + x[n]
  s2 = s1
  s1 = s0

Power = s1^2 + s2^2 - coeff * s1 * s2
Magnitude = sqrt(2 * Power) / N
Level_dB = 20 * log10(Magnitude)
```

### 2. MEMS Calibration

Compensates for MEMS microphone frequency response deviations:

| Frequency | Compensation |
|-----------|-------------|
| 25 Hz     | +3.0 dB     |
| 40 Hz     | +1.5 dB     |
| 63 Hz+    | 0.0 dB      |

### 3. Parametric EQ (RBJ Cookbook)

Biquad coefficients for room correction:

```
A  = 10^(gain_dB / 40)
w0 = 2 * pi * fc / fs
alpha = sin(w0) / (2 * Q)

b0 =  1 + alpha * A
b1 = -2 * cos(w0)
b2 =  1 - alpha * A
a0 =  1 + alpha / A
a1 = -2 * cos(w0)
a2 =  1 - alpha / A

Normalize all by a0
```

### 4. Iterative Refinement

Achieves target accuracy through iterative correction:

```
1. Measure initial room response
2. Compute correction gains
3. Apply correction, re-measure
4. If error > target:
   - Compute refinement (with damping)
   - Update cumulative gains
   - Repeat until converged (max 3 iterations)
```

**Damping Factor:** 0.7 (prevents over-correction oscillation)

## Test Results

### Room Scenarios

| Room | Description | Max Error | RMS Error | Status |
|------|-------------|-----------|-----------|--------|
| Room 1 | Strong bass buildup (50 Hz, 80 Hz modes) | 0.30 dB | 0.17 dB | PASS |
| Room 2 | Bass null (100 Hz cancellation) | 0.61 dB | 0.32 dB | PASS |
| Room 3 | Moderate room (typical small studio) | 0.37 dB | 0.20 dB | PASS |
| Room 4 | Flat room (minimal correction) | 0.33 dB | 0.21 dB | PASS |
| Room 5 | Severe room (multiple modes) | 0.73 dB | 0.33 dB | PASS |

**Overall Result: 5/5 PASS (100%)**

### Performance Metrics

- **Target Accuracy:** ±1.0 dB at all EQ10 band centers
- **Achieved Accuracy:** ±0.73 dB maximum error (Room 5)
- **Average RMS Error:** 0.25 dB across all rooms
- **Convergence:** All rooms converged within 2-3 iterations

### Timing

- **Tone Duration:** 300 ms per band (200 ms settling + 100 ms analysis)
- **Total Cal Time:** ~3 seconds for 10 bands (single-shot measurement)
- **Iterative Refinement:** ~6-9 seconds total (2-3 iterations with re-measurement)

## Key Findings

### What Works Well

1. **Goertzel Detection:** Highly accurate for single-frequency measurement
2. **MEMS Calibration:** Simple offset-based compensation is effective
3. **Iterative Refinement:** Converges quickly (2-3 iterations) to target accuracy
4. **Robustness:** Handles diverse room scenarios (nulls, peaks, flat)

### Observations

1. **Initial Correction:** Single-shot correction achieves ~1.5-3.5 dB accuracy
2. **Iterative Benefit:** Refinement improves accuracy to <1 dB consistently
3. **Damping Factor:** 0.7 damping prevents oscillation while ensuring convergence
4. **Noise Floor:** -60 dB SNR room noise does not degrade performance

### Limitations Validated

1. **Correction Range:** ±12 dB limit is sufficient for all tested scenarios
2. **Q Factor:** Fixed Q=2.0 provides good correction without excessive ringing
3. **Band Spacing:** 10-band EQ provides adequate resolution for room correction

## Files

| File | Purpose |
|------|---------|
| `quicktune_goertzel.py` | Main prototype script |
| `plots/Room_*.png` | Per-room validation plots |
| `plots/summary.png` | Multi-room comparison summary |
| `README.md` | This documentation |

## Usage

```bash
cd /Users/jasonho610/Desktop/pg-dsp-studio-monitor/prototypes/quicktune_goertzel
python3 quicktune_goertzel.py
```

## Dependencies

- **numpy:** Signal generation, Goertzel computation
- **scipy:** Biquad filtering
- **matplotlib:** Visualization

## Next Steps for Implementation

### Ready for Embedded Implementation

- [x] Algorithm validated across diverse room scenarios
- [x] Accuracy meets target specification (±1 dB)
- [x] Computational complexity is feasible (Goertzel is efficient)
- [x] MEMS calibration approach confirmed

### Implementation Recommendations

1. **Single-Shot vs. Iterative:**
   - **Option A:** Single-shot measurement (~3 seconds, ~2 dB accuracy)
   - **Option B:** Iterative refinement (~9 seconds, <1 dB accuracy)
   - **Recommendation:** Implement both; let user choose (Quick vs. Precise mode)

2. **CPU Budget:**
   - Goertzel per band: ~5N multiplications (N = 4800 samples @ 100ms)
   - Total: ~50k multiplications = negligible CPU @ 250 MHz
   - EQ10 cascade: ~4% CPU (already validated in EQ10 prototype)

3. **Memory:**
   - Goertzel state: 2 floats per band = 80 bytes
   - EQ10 coefficients: 50 floats = 200 bytes
   - Tone buffer: 4800 samples @ 300ms = 19.2 KB
   - **Total:** ~20 KB (well within STM32H562 envelope)

4. **User Experience:**
   - Display progress bar during measurement
   - Show before/after frequency response
   - Allow manual fine-tuning after auto-correction

5. **Robustness:**
   - Add timeout for Goertzel (handle silence or very low SNR)
   - Validate MEMS microphone is functional before calibration
   - Limit correction gains if room response is too extreme

## Validation Criteria

- [x] Goertzel algorithm correctly measures single-frequency energy
- [x] MEMS calibration offsets are applied correctly
- [x] Parametric EQ biquads implement RBJ cookbook accurately
- [x] Iterative refinement converges to target accuracy
- [x] Algorithm handles diverse room scenarios (peaks, nulls, flat)
- [x] Residual error ≤ ±1 dB at all EQ10 band centers
- [x] Correction gains stay within ±12 dB range

## Conclusion

**VALIDATION: PASS**

The QuickTune Goertzel Detection algorithm is **ready for embedded implementation**. The prototype demonstrates:

- Robust performance across all tested room scenarios
- Excellent accuracy (±0.73 dB max error, avg 0.25 dB RMS)
- Fast convergence with iterative refinement
- Feasible computational and memory requirements for STM32H562

No blockers identified. Implementation agent can proceed with embedded C++ development.

---

**Author:** Prototype Agent
**Date:** 2026-02-09
**Status:** VALIDATED - Ready for Implementation
