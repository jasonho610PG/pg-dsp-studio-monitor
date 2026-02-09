# EQ10 Algorithm

**10-band parametric EQ for bass correction.**

---

## Overview

Cascaded biquad filters (Direct Form I) implementing parametric EQ with adjustable frequency, gain, and Q.

---

## Parameters

| Band | Center Frequency | Gain Range | Q Range |
|------|-----------------|------------|---------|
| 1    | 25 Hz           | ±12 dB     | 0.5 - 5.0 |
| 2    | 40 Hz           | ±12 dB     | 0.5 - 5.0 |
| 3    | 63 Hz           | ±12 dB     | 0.5 - 5.0 |
| 4    | 100 Hz          | ±12 dB     | 0.5 - 5.0 |
| 5    | 160 Hz          | ±12 dB     | 0.5 - 5.0 |
| 6    | 250 Hz          | ±12 dB     | 0.5 - 5.0 |
| 7    | 400 Hz          | ±12 dB     | 0.5 - 5.0 |
| 8    | 630 Hz          | ±12 dB     | 0.5 - 5.0 |
| 9    | 1 kHz           | ±12 dB     | 0.5 - 5.0 |
| 10   | 1.6 kHz         | ±12 dB     | 0.5 - 5.0 |

---

## Biquad Coefficients

For parametric EQ (boost/cut at center frequency):

```
A  = 10^(gain/40)
w0 = 2*pi*fc/fs
alpha = sin(w0)/(2*Q)

b0 =  1 + alpha*A
b1 = -2*cos(w0)
b2 =  1 - alpha*A
a0 =  1 + alpha/A
a1 = -2*cos(w0)
a2 =  1 - alpha/A

// Normalize by a0
b0 /= a0
b1 /= a0
b2 /= a0
a1 /= a0
a2 /= a0
```

---

## Implementation

**CMSIS-DSP:**
```cpp
arm_biquad_casd_df1_inst_f32 eq10;
float eq10State[20];   // 2 * 10 stages
float eq10Coeffs[50];  // 5 * 10 stages

arm_biquad_cascade_df1_init_f32(&eq10, 10, eq10Coeffs, eq10State);
arm_biquad_cascade_df1_f32(&eq10, input, output, numSamples);
```

---

## CPU Usage

- 10 biquads × 20 cycles/sample = 200 cycles/sample
- At 48 kHz: 200 × 48000 = 9.6 MCPS
- At 250 MHz: ~4% CPU

---

## Validation Criteria

- **Frequency Response:** ±0.5 dB from target
- **Phase Response:** Linear (biquads introduce minimal phase shift)
- **THD+N:** < 0.1% at 1 kHz, -6 dBFS

---

*Load this file for EQ10 implementation.*
