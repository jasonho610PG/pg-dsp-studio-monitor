# /validate

**Trigger validation agent for measurement campaigns.**

```yaml
command: /validate
agent: validation
model: sonnet
description: Measurement campaigns and TRD compliance validation
```

## Usage

```
/validate [feature] [trd-version]
```

Examples:
- `/validate EQ10 frequency response v1.2`
- `/validate BassGuard protection v1.0`
- `/validate QuickTune sweep accuracy v1.1`

## What It Does

1. Spawns validation agent (sonnet)
2. Loads relevant skills JIT (dsp-measurement, TRD framework, feature-specific)
3. References legacy measurement tools if applicable
4. Designs and executes measurement campaign
5. Compares results to TRD pass/fail criteria
6. Returns validation report with plots

## Output

- Validation report
- Measurement data
- Plots (frequency response, THD+N, etc.)
- Pass/fail status
- Milestone review readiness
