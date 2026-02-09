# Mix Translation

**Simulate different playback systems.**

---

## Overview

Allows producers to hear how mix translates to common playback systems (car, phone, laptop).

---

## Presets

### 1. Car Speakers

**Target Response:**
- Bass: +6 dB @ 80 Hz (road noise compensation)
- Mids: -2 dB @ 500 Hz (nasal reduction)
- Highs: -3 dB @ 8 kHz (harsh reduction)

**Additional:** Bandpass filter (50 Hz - 12 kHz)

---

### 2. Phone Speaker

**Target Response:**
- Bass: Roll-off below 200 Hz (small driver limitation)
- Mids: +3 dB @ 2 kHz (presence)
- Highs: Roll-off above 10 kHz

**Additional:** Mono summing (phone is mono)

---

### 3. Laptop Speaker

**Target Response:**
- Bass: Roll-off below 150 Hz
- Mids: +2 dB @ 1 kHz (speech clarity)
- Highs: 0 dB (relatively good high-frequency extension)

**Additional:** Narrow stereo image (speakers close together)

---

## Implementation

Each preset = EQ10 + additional processing (bandpass, mono sum, stereo width)

**EQ10 Settings:** Per preset (see above)

**Bandpass Filter:** High-pass + low-pass biquads

**Mono Sum:**
```cpp
output[0] = (input[0] + input[1]) * 0.5f;
output[1] = output[0];
```

**Stereo Width:**
```cpp
float mid = (input[0] + input[1]) * 0.5f;
float side = (input[0] - input[1]) * 0.5f * width;
output[0] = mid + side;
output[1] = mid - side;
```

---

## Validation Criteria

- **Frequency Response:** Match target (±2 dB)
- **A/B Comparison:** Clear difference between presets
- **CPU Usage:** < 5% per preset

---

*Load this file for Mix Translation implementation.*
