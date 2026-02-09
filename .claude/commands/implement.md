# /implement

**Trigger implementation agent for embedded C++ (STM32H562).**

```yaml
command: /implement
agent: implementation
model: sonnet
description: Embedded C++ implementation using CMSIS-DSP for STM32H562
```

## Usage

```
/implement [feature]
```

Examples:
- `/implement EQ10 parametric EQ with CMSIS-DSP`
- `/implement BassGuard RMS limiter`
- `/implement QuickTune gyro interface`

## What It Does

1. Spawns implementation agent (sonnet)
2. Loads relevant skills JIT (dsp-embedded-constraints, CMSIS-DSP patterns, feature-specific)
3. Reviews prototype validation results
4. Writes embedded C++ using CMSIS-DSP
5. Profiles CPU and memory usage
6. Returns implementation summary

## Output

- Code files created/modified
- CMSIS-DSP functions used
- CPU usage estimate
- Memory usage estimate
- Validation readiness status
