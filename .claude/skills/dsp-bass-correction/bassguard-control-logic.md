# BassGuard Control Logic

**Speaker protection via RMS-based limiting.**

---

## Overview

Adaptive RMS limiter that reduces gain when bass energy exceeds safe threshold, protecting speaker from over-excursion.

---

## Algorithm

1. **RMS Calculation:** Compute RMS of input signal (short time window, e.g., 10ms)
2. **Threshold Check:** Compare RMS to speaker-dependent threshold
3. **Gain Reduction:** If RMS > threshold, reduce gain smoothly
4. **Attack/Release:** Fast attack (protect quickly), slow release (avoid pumping)

---

## Parameters

| Parameter | Value | Notes |
|-----------|-------|-------|
| RMS Window | 10 ms | 480 samples @ 48 kHz |
| Threshold | Speaker-dependent | Set based on X_max |
| Attack Time | 5 ms | Fast protection |
| Release Time | 50 ms | Smooth recovery |
| Max Reduction | -12 dB | Limit maximum attenuation |

---

## Pseudocode

```cpp
float rms = ComputeRMS(input, windowSize);
float targetGain = 1.0f;

if (rms > threshold) {
    targetGain = threshold / rms;
    targetGain = max(targetGain, minGain); // Limit to -12 dB
}

// Smooth gain transition
float alpha = (targetGain < currentGain) ? alphaAttack : alphaRelease;
currentGain = alpha * targetGain + (1 - alpha) * currentGain;

// Apply gain
for (int i = 0; i < numSamples; i++) {
    output[i] = input[i] * currentGain;
}
```

---

## CMSIS-DSP Implementation

```cpp
float rms;
arm_rms_f32(input, numSamples, &rms);

// Compute target gain
float targetGain = (rms > threshold) ? (threshold / rms) : 1.0f;
targetGain = fmax(targetGain, minGain);

// Smooth with exponential filter
float alpha = (targetGain < currentGain) ? alphaAttack : alphaRelease;
currentGain = alpha * targetGain + (1.0f - alpha) * currentGain;

// Apply gain
arm_scale_f32(input, currentGain, output, numSamples);
```

---

## CPU Usage

- RMS calculation: ~10 cycles/sample
- Gain calculation: negligible
- Gain application: ~1 cycle/sample
- **Total: ~1% CPU**

---

## Validation Criteria

- **Protection:** Cone excursion < X_max under all conditions
- **Transparency:** No audible artifacts when not limiting
- **Response Time:** Attack < 5 ms, Release ~50 ms
- **THD+N:** < 0.1% when not limiting

---

*Load this file for BassGuard implementation.*
