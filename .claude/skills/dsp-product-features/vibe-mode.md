# Vibe Mode

**Music-optimized tuning.**

---

## Overview

Preset tuning optimized for music playback (not critical listening). Enhanced bass, smooth highs, wide soundstage.

---

## Target Response

**Frequency Response:**
- Bass: +5 dB @ 50 Hz (deep bass boost)
- Mids: Slight reduction @ 500 Hz (-1 dB, reduces boxiness)
- Highs: +3 dB @ 12 kHz (airiness), gentle roll-off above 16 kHz

**Stereo Width:** Widened (120%)

**Dynamic Processing:** Light compression (3:1 ratio, -20 dB threshold)

---

## EQ10 Settings

| Band | Frequency | Gain | Q |
|------|-----------|------|---|
| 1    | 25 Hz     | +3 dB | 1.0 |
| 2    | 40 Hz     | +4 dB | 1.0 |
| 3    | 63 Hz     | +5 dB | 1.0 |
| 4    | 100 Hz    | +3 dB | 1.0 |
| 6    | 250 Hz    | 0 dB  | - |
| 7    | 400 Hz    | -1 dB | 1.5 |
| 10   | 1.6 kHz   | 0 dB  | - |

Additional high-shelf: +3 dB @ 12 kHz, Q = 0.7

---

## Stereo Width

Widen stereo image (120% width):

```cpp
float mid = (input[0] + input[1]) * 0.5f;
float side = (input[0] - input[1]) * 0.5f * 1.2f;  // 120% width
output[0] = mid + side;
output[1] = mid - side;
```

---

## Light Compression

Gentle compression to smooth dynamics:

```cpp
// Simple compressor
float rms = computeRMS(input);
float gain = 1.0f;

if (rms > threshold) {
    float excess_dB = 20 * log10(rms / threshold);
    float reduction_dB = excess_dB * (1.0f - 1.0f / ratio);
    gain = dBToLinear(-reduction_dB);
}

output = input * gain;
```

---

## CPU Usage

- EQ10: ~4% (reuse existing)
- Stereo width: negligible
- Compression: ~1%
- **Total: ~5% CPU**

---

## Validation Criteria

- **Frequency Response:** Match target (±1 dB)
- **Stereo Width:** 120% verified via correlation meter
- **Compression:** 3:1 ratio, -20 dB threshold
- **Subjective:** Pleasant, non-fatiguing sound

---

*Load this file for Vibe Mode implementation.*
