# SafeSound Limiter

**Speaker protection + perceived loudness.**

---

## Overview

Combines speaker protection (like BassGuard) with perceptual loudness enhancement.

---

## Algorithm

1. **RMS Limiting:** Protect speaker from over-excursion (see BassGuard)
2. **Perceptual Weighting:** Apply A-weighting to emphasize perceptually loud frequencies
3. **Soft Clipping:** Gentle saturation at high levels (avoids hard clipping)
4. **Make-Up Gain:** Compensate for limiting, maintain perceived loudness

---

## Perceptual Weighting

A-weighting filter emphasizes 2-4 kHz (most sensitive frequency range):

**A-Weighting Approximation:**
```
Boost: +3 dB @ 2-4 kHz
Roll-off: -10 dB/decade below 200 Hz, -5 dB/decade above 10 kHz
```

**Implementation:** 2-stage biquad (high-pass + peak)

---

## Soft Clipping

Gentle saturation using tanh or cubic function:

```cpp
float softClip(float x, float threshold) {
    if (abs(x) < threshold) {
        return x;
    } else {
        float sign = (x > 0) ? 1.0f : -1.0f;
        float excess = abs(x) - threshold;
        return sign * (threshold + tanh(excess));
    }
}
```

---

## Make-Up Gain

Automatically adjust output gain to maintain perceived loudness:

```cpp
float targetLoudness = computeLUFS(input);  // Target LUFS
float currentLoudness = computeLUFS(output);
float makeupGain = targetLoudness - currentLoudness;
output *= dBToLinear(makeupGain);
```

---

## CPU Usage

- RMS limiting: ~1% (reuse BassGuard)
- A-weighting: ~1% (2 biquads)
- Soft clipping: negligible
- Make-up gain: negligible
- **Total: ~2-3% CPU**

---

## Validation Criteria

- **Protection:** Cone excursion < X_max (same as BassGuard)
- **Loudness:** Perceived loudness maintained (LUFS measurement)
- **THD+N:** < 0.5% (soft clipping introduces harmonics, but acceptable)
- **Transparency:** No audible artifacts when not limiting

---

*Load this file for SafeSound implementation.*
