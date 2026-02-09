# /research

**Trigger research agent for algorithm discovery.**

```yaml
command: /research
agent: research
model: opus
description: Algorithm discovery and literature review for DSP features
```

## Usage

```
/research [feature] [context]
```

Examples:
- `/research bass correction for 6.5" woofer`
- `/research speaker protection limiter`
- `/research gyro-based auto-tuning`

## What It Does

1. Spawns research agent (opus)
2. Loads relevant skills JIT (dsp-bass-correction, dsp-product-features, dsp-embedded-constraints)
3. Searches literature for IP-safe algorithms
4. Validates feasibility against STM32H562 constraints
5. Returns structured recommendation

## Output

- Algorithm recommendation
- CPU/memory estimates
- IP risk assessment
- References
- Next steps for prototype agent
