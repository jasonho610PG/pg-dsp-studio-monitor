# CMSIS-DSP Patterns

**Common usage patterns for ARM CMSIS-DSP library.**

---

## Biquad Cascade (IIR Filters)

**Use for:** Parametric EQ, low/high-pass filters, shelves

```cpp
// State: 4 floats per stage (2 * numStages)
arm_biquad_casd_df1_inst_f32 S;
float state[8];  // 2 stages
float coeffs[10]; // 5 coeffs per stage: b0, b1, b2, a1, a2

// Init
arm_biquad_cascade_df1_init_f32(&S, 2, coeffs, state);

// Process
arm_biquad_cascade_df1_f32(&S, input, output, numSamples);
```

**CPU:** ~20 cycles per sample per stage

---

## FIR Filter

**Use for:** Linear-phase filtering

```cpp
arm_fir_instance_f32 S;
float state[numTaps + blockSize - 1];
float coeffs[numTaps];

// Init
arm_fir_init_f32(&S, numTaps, coeffs, state, blockSize);

// Process
arm_fir_f32(&S, input, output, blockSize);
```

**CPU:** ~5 cycles per tap per sample

---

## RMS Calculation

**Use for:** Level detection, limiters

```cpp
float rms;
arm_rms_f32(input, numSamples, &rms);
```

**CPU:** ~10 cycles per sample

---

## Vector Operations

**Multiply by scalar:**
```cpp
arm_scale_f32(input, gain, output, numSamples);
```

**Add vectors:**
```cpp
arm_add_f32(input1, input2, output, numSamples);
```

**Multiply vectors element-wise:**
```cpp
arm_mult_f32(input1, input2, output, numSamples);
```

---

## Optimization Tips

1. Use CMSIS-DSP instead of manual loops (ARM-optimized)
2. Align buffers to 4-byte boundaries
3. Use block processing (not sample-by-sample)
4. Preallocate all state arrays
5. Profile with actual hardware (not simulation)

---

*Load this file for implementation guidance.*
