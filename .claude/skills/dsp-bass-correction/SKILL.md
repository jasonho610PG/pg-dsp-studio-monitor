---
name: dsp-bass-correction
description: EQ10 parametric EQ, BassGuard protection, MEMS microphone findings
sensitivity: CONFIDENTIAL
allowed-tools:
  - Read
portable: false
version: "1.0"
owner: DSP Team (Ivan/Derek/Jason)
---

# DSP Bass Correction

Validated algorithms for Studio Monitor bass correction and speaker protection.

## Contents

| File | Purpose |
|------|---------|
| `eq10-algorithm.md` | 10-band parametric EQ design |
| `bassguard-control-logic.md` | Speaker protection via RMS limiting |
| `mems-findings.md` | MEMS microphone calibration and frequency response |
| `trd-validation-framework.md` | TRD structure and pass/fail criteria |

## Usage

This skill is used by:
- **research** agent — Reference validated algorithms
- **prototype** agent — Implement EQ10 or BassGuard testbed
- **implementation** agent — Embedded EQ10 or BassGuard
- **validation** agent — TRD compliance checks
- **documentation** agent — TRD generation

## Key Features

### EQ10
- 10-band parametric EQ
- Biquad cascade (Direct Form I)
- Frequency range: 20 Hz - 20 kHz
- ±12 dB gain per band

### BassGuard
- RMS-based limiter
- Adaptive threshold (speaker-dependent)
- Protects against over-excursion
- Transparent when not protecting

## Progressive Disclosure

1. **Always load:** SKILL.md (this file)
2. **Load JIT:** Specific files based on task
   - EQ work → `eq10-algorithm.md`
   - Protection work → `bassguard-control-logic.md`
   - Measurement work → `mems-findings.md`
   - Validation work → `trd-validation-framework.md`
