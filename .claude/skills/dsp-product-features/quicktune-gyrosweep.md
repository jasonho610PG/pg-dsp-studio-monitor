# QuickTune Gyro Sweep

**Gyro-based auto-tuning feature.**

---

## Overview

User tilts device → sweep frequency changes → MEMS mic captures response → algorithm suggests EQ correction.

---

## Gyro Mapping

**Tilt angle → Sweep frequency:**

```
Tilt Range: -45° to +45° (device tilted forward/backward)
Frequency Range: 20 Hz to 200 Hz (bass region)

Mapping: freq = 20 * 10^(tilt / 45)
```

Examples:
- Tilt = -45°: freq = 20 Hz
- Tilt = 0°: freq = 63 Hz
- Tilt = +45°: freq = 200 Hz

---

## Process

1. **Activate QuickTune Mode**
2. **Gyro Sweep:** User tilts device, speaker plays sweep
3. **Capture:** MEMS mic records room response
4. **Analyze:** Compute frequency response (apply MEMS calibration)
5. **Suggest EQ:** Generate EQ10 settings for room correction
6. **Apply or Adjust:** User confirms or fine-tunes

---

## Algorithm

```python
# Read gyro tilt angle
tilt = read_gyro()  # -45° to +45°

# Map to frequency
freq = 20 * (10 ** (tilt / 45))

# Generate sine tone at freq
signal = generate_sine(freq, duration=0.5)

# Play through speaker
play_output(signal)

# Capture with MEMS mic
recorded = capture_mems()

# Analyze response
response_dB = analyze_response(recorded, freq)

# Suggest EQ correction
eq_gain = -response_dB  # Invert to flatten
```

---

## Validation Criteria

- **Sweep Range:** 20 Hz - 200 Hz (smooth, no jumps)
- **Gyro Responsiveness:** < 50 ms latency
- **MEMS Accuracy:** ±1 dB after calibration
- **Auto-EQ Accuracy:** ±1 dB from target flat response

---

*Load this file for QuickTune implementation.*
