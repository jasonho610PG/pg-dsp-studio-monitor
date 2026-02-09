# pg-dsp-studio-monitor

**DSP development system for Studio Monitor Bass Correction features (EQ10, BassGuard, QuickTune).**

> Team: DSP (Ivan/Derek/Jason)
> Pattern: Knowledge Twin (team-shared)
> Integration: References legacy `/Users/jasonho610/Desktop/studio-monitor`

---

## Source of Truth

**`platform-context/SPEC.md`** (in pg-agent-dev) is the canonical architecture spec.

## Commands

| Command | Purpose |
|---------|---------|
| `/research` | Algorithm discovery, literature review |
| `/prototype` | JUCE testbed + Python validation |
| `/implement` | Embedded C++ (STM32H562, CMSIS-DSP) |
| `/validate` | Measurement campaigns, TRD compliance |
| `/milestone-review` | Generate milestone package (validation + docs) |
| `/cleanup` | Pre-commit sensitivity scan + lint |
| `/ship` | Git commit + push workflow |
| `/save-memory` | Save session learnings |
| `/feedback` | Capture feedback, propose actions |
| `/refresh-context` | Reload context after propagation |
| `/update-docs` | Sync documentation |

## Agents

| Agent | Purpose | Model |
|-------|---------|-------|
| research | Algorithm discovery, IP-safe solutions | opus |
| prototype | JUCE testbed, Python validation | sonnet |
| implementation | Embedded C++, CMSIS-DSP, binary delivery | sonnet |
| validation | Measurement campaigns, TRD framework | sonnet |
| documentation | TRD, design docs, milestone reports | sonnet |

## Skills (Domain)

- **dsp-embedded-constraints** (CONFIDENTIAL) — STM32H562 envelope, CMSIS-DSP
- **dsp-bass-correction** (CONFIDENTIAL) — EQ10, BassGuard, MEMS findings
- **dsp-juce-testbed** (INTERNAL) — Plugin architecture, build workflow
- **dsp-measurement** (CONFIDENTIAL) — Campaign methodology, pass/fail criteria
- **dsp-product-features** (CONFIDENTIAL) — QuickTune, Dual Voicing, Mix Translation, SafeSound, Vibe Mode

Uses shared skills from `~/.claude/skills/`: dev-context-engineering, dev-governance, dev-memory-protocol, dev-doc-maintenance, dev-context-loader

---

*Built from pg-agent-dev template v1.0*
