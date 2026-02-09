# ProcessBlock Interface

**Audio processing interface contract for embedded DSP.**

---

## Function Signature

```cpp
void ProcessBlock(float* input, float* output, int numSamples);
```

---

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `input` | `float*` | Input audio buffer (read-only) |
| `output` | `float*` | Output audio buffer (write-only) |
| `numSamples` | `int` | Number of samples in block (typically 32) |

---

## Constraints

1. **Real-time:** Must complete within 667 µs (32 samples @ 48 kHz)
2. **No malloc:** No dynamic memory allocation
3. **Thread-safe:** May be called from interrupt context
4. **Float32:** Input/output range: -1.0 to +1.0
5. **In-place OK:** `input == output` is allowed

---

## Example: Simple Gain

```cpp
void ProcessBlock(float* input, float* output, int numSamples) {
    const float gain = 0.5f;
    for (int i = 0; i < numSamples; i++) {
        output[i] = input[i] * gain;
    }
}
```

---

## Example: Biquad Filter (CMSIS-DSP)

```cpp
arm_biquad_casd_df1_inst_f32 biquad;
float biquadState[4];  // 2 * numStages
float biquadCoeffs[5]; // Per stage: b0, b1, b2, a1, a2

void ProcessBlock(float* input, float* output, int numSamples) {
    arm_biquad_cascade_df1_f32(&biquad, input, output, numSamples);
}
```

---

## Initialization

All state must be initialized before first `ProcessBlock()` call:

```cpp
void Init() {
    // Initialize biquad
    arm_biquad_cascade_df1_init_f32(&biquad, 1, biquadCoeffs, biquadState);
}
```

---

## Error Handling

- No exceptions allowed (embedded C++)
- Return errors via global status flag
- Critical errors: disable processing, set safe output (silence)

---

*Load this file when implementing ProcessBlock.*
