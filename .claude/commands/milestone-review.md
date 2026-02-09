# /milestone-review

**Generate milestone review package (validation + documentation).**

```yaml
command: /milestone-review
agents:
  - validation
  - documentation
model: sonnet
description: Auto-generate milestone report combining validation results and technical documentation
```

## Usage

```
/milestone-review [phase] [feature]
```

Examples:
- `/milestone-review validation EQ10`
- `/milestone-review delivery BassGuard`
- `/milestone-review prototype QuickTune`

## What It Does

1. Spawns validation agent → runs measurements, generates validation report
2. Spawns documentation agent → reads validation report, generates milestone report
3. Packages results (reports + plots) for stakeholder review
4. Returns milestone package location

## Output

- Validation report
- Milestone report
- Measurement plots
- Package directory location

## Milestone Phases

1. **Investigation** — Research findings, feasibility
2. **Prototype** — Testbed validation results
3. **Implementation** — Code summary, CPU/memory usage
4. **Validation** — Measurement campaign results
5. **Delivery** — Binary-only release notes
