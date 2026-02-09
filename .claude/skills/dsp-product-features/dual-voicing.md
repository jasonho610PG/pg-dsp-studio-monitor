# Dual Voicing

**Flat vs. Enhanced voicing modes.**

---

## Overview

Two tuning profiles:
- **Flat:** Reference monitor tuning (accurate, neutral)
- **Enhanced:** Consumer-friendly tuning (boosted bass, smooth highs)

---

## Flat Mode

**Target Response:** Flat (±2 dB, 40 Hz - 16 kHz)

**Use Case:** Mixing, mastering, critical listening

**EQ10 Settings:** Minimal correction (room-dependent)

---

## Enhanced Mode

**Target Response:**
- Bass: +3 dB @ 60 Hz (gentle boost)
- Mids: Flat
- Highs: +2 dB @ 10 kHz (airiness), gentle roll-off above 16 kHz

**Use Case:** Music playback, casual listening

**EQ10 Settings:**

| Band | Frequency | Gain | Q |
|------|-----------|------|---|
| 2    | 40 Hz     | +2 dB | 1.0 |
| 3    | 63 Hz     | +3 dB | 1.0 |
| 4    | 100 Hz    | +2 dB | 1.0 |
| 9    | 1 kHz     | 0 dB  | - |
| 10   | 1.6 kHz   | 0 dB  | - |

Additional high-shelf: +2 dB @ 10 kHz, Q = 0.7

---

## Switching

**Method:** Button press or app control

**Transition:** Smooth crossfade (500 ms) to avoid clicks

**Implementation:**

```cpp
// Interpolate between Flat and Enhanced coefficients
for (int band = 0; band < 10; band++) {
    float* currentCoeffs = (mode == FLAT) ? flatCoeffs[band] : enhancedCoeffs[band];
    interpolate_coeffs(biquadCoeffs[band], currentCoeffs, transitionTime);
}
```

---

## Validation Criteria

- **Flat Mode:** ±2 dB, 40 Hz - 16 kHz
- **Enhanced Mode:** Match target response (±1 dB)
- **Switching:** No clicks, smooth transition

---

*Load this file for Dual Voicing implementation.*
