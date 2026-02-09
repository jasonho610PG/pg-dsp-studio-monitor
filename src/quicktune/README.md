# QuickTune Room Correction - Embedded Implementation

**Automatic room correction using stepped sine tones + Goertzel detection**

---

## Overview

QuickTune automatically corrects room acoustics in ~3 seconds using:

1. **Tone Generation** — Recursive sine oscillator (no trig per sample)
2. **Frequency Detection** — Goertzel algorithm (single-frequency power measurement)
3. **MEMS Calibration** — Compensate for microphone roll-off at low frequencies
4. **Parametric EQ** — 10-band correction using RBJ cookbook biquads
5. **Iterative Refinement** — Optional 2-3 passes with damping for convergence

**Validation:** 5/5 test rooms pass, max error 0.73 dB (target ±1 dB)

---

## Files

| File | Purpose |
|------|---------|
| `quicktune_config.h` | Configuration constants (sample rate, bands, calibration) |
| `quicktune.h` | Public API (init, start, process, status) |
| `quicktune.cpp` | Implementation (tone generator, Goertzel, state machine) |
| `eq10.h` | 10-band parametric EQ API |
| `eq10.cpp` | EQ10 implementation (CMSIS-DSP biquad cascade) |
| `README.md` | This file |

---

## Performance

### CPU Usage (STM32H562 @ 250 MHz)

| Phase | CPU % | Details |
|-------|-------|---------|
| Calibration | 0.13% | Tone gen (3 cyc/samp) + Goertzel (4 cyc/samp) |
| Post-Cal (EQ10) | 3.8% | 10 biquads × 20 cyc/samp/stage = 200 cyc/samp |

**Well within 60% CPU budget**

### Memory Usage (Static Allocation)

| Component | Bytes | Details |
|-----------|-------|---------|
| QuickTune state | ~116 | Tone gen, Goertzel, counters, gains |
| EQ10 state | ~320 | Biquad instance, coefficients, state |
| Configuration (const) | ~120 | Band freqs, MEMS cal, Goertzel coeffs |
| **Total** | **~556** | **Well within 640 KB SRAM** |

### Timing

| Phase | Duration |
|-------|----------|
| Per-band measurement | 300 ms (200 ms settling + 100 ms analysis) |
| Total calibration | ~3 seconds (10 bands × 300 ms) |
| Iterative refinement | +3 seconds per pass (optional, 2-3 passes typical) |

---

## Integration Example

### Basic Usage

```cpp
#include "quicktune.h"

// Initialize at startup
void Audio_Init(void)
{
    QuickTune_Init();
}

// Start calibration (e.g., button press)
void OnCalibrateButtonPressed(void)
{
    if (QuickTune_Start())
    {
        // Calibration started
        LED_SetBlinking();
    }
}

// Main audio loop (called every 32 samples @ 48 kHz)
void Audio_ProcessBlock(float* micInput, float* speakerOutput, int numSamples)
{
    // Always call QuickTune_ProcessBlock during calibration
    QuickTune_ProcessBlock(micInput, speakerOutput, numSamples);

    // Check if calibration complete
    if (QuickTune_GetState() == QUICKTUNE_STATE_DONE)
    {
        // Calibration done
        LED_SetSolid();

        // Get results
        const float* gains = QuickTune_GetCorrectionGains();
        if (gains != NULL)
        {
            // Optionally save to flash for persistence
            SaveGainsToFlash(gains);
        }

        // Acknowledge completion (returns to IDLE)
        QuickTune_Stop();
    }

    // After calibration, process audio through EQ10
    if (QuickTune_GetState() == QUICKTUNE_STATE_IDLE)
    {
        // Normal audio processing (with EQ10 correction applied)
        // ... your audio processing here ...
    }
}
```

### Progress Indication

```cpp
void UpdateProgressBar(void)
{
    if (QuickTune_GetState() == QUICKTUNE_STATE_MEASURING)
    {
        float progress = QuickTune_GetProgress();  // 0.0 to 1.0
        int band = QuickTune_GetCurrentBand();     // 0-9

        printf("Measuring band %d/10... %.0f%%\n", band + 1, progress * 100.0f);

        // Update LED/LCD/GUI
        ProgressBar_Update(progress);
    }
}
```

### Manual Gain Application (Preset)

```cpp
// Load saved preset
void LoadCalibrationPreset(void)
{
    float gains[10] = {
        -2.5f,  // 25 Hz
        -1.8f,  // 40 Hz
        0.5f,   // 63 Hz
        1.2f,   // 100 Hz
        -0.3f,  // 160 Hz
        -1.5f,  // 250 Hz
        0.8f,   // 400 Hz
        0.2f,   // 630 Hz
        -0.5f,  // 1000 Hz
        0.0f    // 1600 Hz
    };

    QuickTune_ApplyGains(gains);
}
```

### Diagnostics

```cpp
void CheckQuickTuneStatus(void)
{
    int error = QuickTune_GetLastError();
    if (error != 0)
    {
        printf("QuickTune error: %d\n", error);
    }

    float cpu = QuickTune_GetCpuUsage();
    printf("CPU usage: %.2f%%\n", cpu);
}
```

---

## Algorithm Details

### Tone Generator (Recursive Oscillator)

**Formula:** `y[n] = 2*cos(w0)*y[n-1] - y[n-2]`

**Initialization:**
- `coeff = 2*cos(w0)`
- `y[-1] = -sin(w0)`
- `y[-2] = -sin(2*w0)`

**Per-sample:** 2 multiplies + 1 add (no trig functions!)

**CPU:** ~3 cycles/sample

### Goertzel Filter

**Formula:** `s[n] = coeff*s[n-1] - s[n-2] + x[n]`

**Coefficient:** `coeff = 2*cos(2*pi*k/N)`

**Final Power:** `power = s1^2 + s2^2 - coeff*s1*s2`

**Magnitude:** `mag = sqrt(2*power) / N`

**CPU:** ~4 cycles/sample

### Parametric EQ (RBJ Cookbook)

**Pre-gain:** `A = 10^(gain_dB / 40)`

**Angular frequency:** `w0 = 2*pi*fc/fs`

**Bandwidth:** `alpha = sin(w0) / (2*Q)`

**Coefficients:**
```
b0 = (1 + alpha*A) / a0
b1 = (-2*cos(w0)) / a0
b2 = (1 - alpha*A) / a0
a1 = (-2*cos(w0)) / a0
a2 = (1 - alpha/A) / a0

where a0 = 1 + alpha/A
```

**CPU:** ~20 cycles/sample/stage

---

## EQ10 Band Frequencies

| Band | Frequency (Hz) | MEMS Cal (dB) |
|------|----------------|---------------|
| 1 | 25 | +3.0 |
| 2 | 40 | +1.5 |
| 3 | 63 | 0.0 |
| 4 | 100 | 0.0 |
| 5 | 160 | 0.0 |
| 6 | 250 | 0.0 |
| 7 | 400 | 0.0 |
| 8 | 630 | 0.0 |
| 9 | 1000 | 0.0 |
| 10 | 1600 | 0.0 |

---

## Validation Results (Python Prototype)

| Room Scenario | Max Error (dB) | RMS Error (dB) | Result |
|---------------|----------------|----------------|--------|
| Room 1: Strong Bass Buildup | 0.30 | 0.15 | PASS |
| Room 2: Bass Null | 0.61 | 0.32 | PASS |
| Room 3: Moderate Room | 0.37 | 0.20 | PASS |
| Room 4: Flat Room | 0.33 | 0.18 | PASS |
| Room 5: Severe Room | 0.73 | 0.41 | PASS |
| **Average** | **0.47** | **0.25** | **PASS** |

**Target:** ±1.0 dB accuracy
**Result:** All rooms pass, average max error 0.47 dB

---

## Configuration

Edit `quicktune_config.h` to customize:

| Parameter | Default | Description |
|-----------|---------|-------------|
| `QUICKTUNE_SAMPLE_RATE` | 48000 | Sample rate (Hz) |
| `QUICKTUNE_BLOCK_SIZE` | 32 | Samples per ProcessBlock call |
| `QUICKTUNE_TONE_SETTLING_MS` | 200 | Settling time per tone (ms) |
| `QUICKTUNE_TONE_ANALYSIS_MS` | 100 | Analysis window per tone (ms) |
| `QUICKTUNE_MAX_GAIN_DB` | 12.0 | Maximum correction gain (±dB) |
| `QUICKTUNE_EQ_Q` | 2.0 | Fixed Q for all bands |
| `QUICKTUNE_MAX_ITERATIONS` | 3 | Max iterative refinement passes |
| `QUICKTUNE_DAMPING_FACTOR` | 0.7 | Damping for iterative refinement |
| `QUICKTUNE_ENABLE_ITERATION` | 1 | Enable iterative refinement (0/1) |

---

## Dependencies

### Required Libraries

- **CMSIS-DSP** — ARM DSP library (arm_math.h)
  - `arm_biquad_cascade_df1_f32()` — Biquad cascade processing
  - `arm_biquad_cascade_df1_init_f32()` — Biquad initialization
  - `arm_sin_f32()` / `arm_cos_f32()` — Fast trig (coefficient computation only)

### Standard Libraries

- `<stdint.h>` — Fixed-width integers
- `<stdbool.h>` — Boolean type
- `<math.h>` — sin, cos, log10, sqrt, pow (coefficient computation only)
- `<string.h>` — memset, memcpy

---

## Build Instructions

### STM32CubeIDE

1. Add source files to project:
   - `src/quicktune/*.cpp`
   - `src/quicktune/*.h`

2. Add include path:
   - `src/quicktune/`

3. Enable CMSIS-DSP:
   - Project Properties → C/C++ Build → Settings → MCU Settings
   - Check "Use float with printf"
   - Add define: `ARM_MATH_CM33`

4. Link CMSIS-DSP library:
   - Add to linker: `libarm_cortexM33l_math.a`

5. Compile and flash

### Makefile

```makefile
# Source files
SOURCES += \
    src/quicktune/quicktune.cpp \
    src/quicktune/eq10.cpp

# Include paths
INCLUDES += \
    -Isrc/quicktune

# Defines
DEFINES += \
    -DARM_MATH_CM33

# Libraries
LIBS += \
    -larm_cortexM33l_math
```

---

## Testing

### Unit Tests (Desktop)

See `tests/quicktune_test.cpp` for unit test examples using Google Test framework.

### Hardware Validation

1. Flash firmware to STM32H562
2. Connect MEMS microphone to ADC input
3. Connect speaker to DAC output
4. Press calibrate button
5. Wait ~3 seconds for completion
6. Verify correction gains via UART/debug interface
7. Measure frequency response with APx or REW

---

## Troubleshooting

### Calibration Fails (High Error)

- Check MEMS microphone calibration offsets
- Verify sample rate is 48 kHz
- Ensure room is reasonably quiet (SNR > 40 dB)
- Check speaker/mic placement (avoid near-field effects)

### CPU Overload

- Reduce block size (e.g., 16 samples)
- Disable iterative refinement
- Profile actual CPU usage (cycle counter)

### Memory Issues

- All memory is statically allocated (~556 bytes)
- No malloc/new used
- Check stack size (recommend ≥8 KB for audio thread)

### Unstable Filters

- Check Q values (should be 0.1 to 20.0)
- Verify gains are clipped to ±12 dB
- Call `EQ10_Reset()` if discontinuities occur

---

## Future Enhancements

- [ ] Adaptive MEMS calibration (learn from measurements)
- [ ] Gyro-triggered sweep (QuickTune Gyro variant)
- [ ] Multi-room preset storage
- [ ] Real-time visualization (frequency response plot)
- [ ] Automatic error detection (ambient noise, mic failure)

---

## References

1. **Goertzel Algorithm**
   "Generalized Goertzel Algorithm" by Pavel Rajmic, 2012

2. **RBJ Audio EQ Cookbook**
   "Cookbook formulae for audio EQ biquad filter coefficients"
   by Robert Bristow-Johnson, 1994

3. **CMSIS-DSP Documentation**
   ARM CMSIS-DSP Software Library
   https://www.keil.com/pack/doc/CMSIS/DSP/html/index.html

4. **Prototype Validation**
   `prototypes/quicktune_goertzel/quicktune_goertzel.py`
   Python validation with 5 room scenarios

---

## License

**CONFIDENTIAL - Binary-only delivery**

Source code is proprietary. Customers receive compiled binary only.

---

*Implementation by DSP Team (Implementation Agent), 2026-02-09*
