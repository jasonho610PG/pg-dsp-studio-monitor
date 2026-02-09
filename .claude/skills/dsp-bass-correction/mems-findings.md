# MEMS Microphone Findings

**Calibration and frequency response characterization.**

---

## Overview

MEMS microphone used for QuickTune auto-calibration. Requires calibration curve to compensate for frequency response deviations.

---

## Hardware

- **Model:** [To be specified based on actual hardware]
- **Frequency Range:** 20 Hz - 20 kHz
- **Sensitivity:** -38 dBFS @ 94 dB SPL
- **SNR:** 64 dB

---

## Frequency Response

**Typical deviations from flat response:**

| Frequency Band | Deviation | Notes |
|----------------|-----------|-------|
| 20 Hz - 40 Hz  | -3 dB     | Roll-off at low end |
| 40 Hz - 10 kHz | ±1 dB     | Relatively flat |
| 10 kHz - 20 kHz | +2 dB    | High-frequency boost |

---

## Calibration Curve

Inverse of measured frequency response, applied as EQ correction:

```
Cal_Gain(f) = 1 / H_mems(f)
```

**Implementation:** Small parametric EQ (3-5 bands) to flatten response.

---

## Validation

1. Measure MEMS frequency response (reference microphone + REW)
2. Generate calibration EQ curve
3. Apply calibration EQ to MEMS signal
4. Re-measure → should be ±1 dB flat

---

## QuickTune Integration

1. Play sweep through speaker
2. Capture with MEMS microphone
3. Apply MEMS calibration EQ
4. Analyze frequency response
5. Suggest EQ10 settings for room correction

---

*Load this file for QuickTune MEMS work.*
