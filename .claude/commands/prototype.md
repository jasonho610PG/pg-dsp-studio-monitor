# /prototype

**Trigger prototype agent for JUCE testbed or Python validation.**

```yaml
command: /prototype
agent: prototype
model: sonnet
description: JUCE testbed development and Python validation for DSP algorithms
```

## Usage

```
/prototype [feature] [environment]
```

Examples:
- `/prototype EQ10 frequency response in JUCE`
- `/prototype QuickTune gyro sweep in Python`
- `/prototype BassGuard RMS limiter in JUCE`

## What It Does

1. Spawns prototype agent (sonnet)
2. Loads relevant skills JIT (dsp-juce-testbed, dsp-embedded-constraints, feature-specific)
3. References legacy testbed code if applicable
4. Implements algorithm in JUCE or Python
5. Runs tests and validates results
6. Returns code summary and validation status

## Output

- Test results
- Code files created/modified
- Validation status (PASS/FAIL)
- Next steps for implementation agent
