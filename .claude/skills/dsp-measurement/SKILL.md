---
name: dsp-measurement
description: Measurement campaign methodology, band analysis, pass/fail criteria
sensitivity: CONFIDENTIAL
allowed-tools:
  - Read
  - Write
  - Bash(python:*)
portable: false
version: "1.0"
owner: DSP Team (Ivan/Derek/Jason)
---

# DSP Measurement

Measurement campaign methodology and validation criteria for Studio Monitor features.

## Contents

| File | Purpose |
|------|---------|
| `campaign-methodology.md` | How to design and execute measurement campaigns |
| `band-analysis.md` | Per-band frequency response analysis |
| `pass-fail-criteria.md` | Thresholds for validation |

## Usage

This skill is used by:
- **validation** agent — Design and execute measurement campaigns
- **documentation** agent — Reference validation criteria for TRDs

## Key Concepts

- **Campaign:** Structured set of measurements with defined test signals, metrics, and criteria
- **Band Analysis:** Per-band validation (e.g., EQ10 bands measured independently)
- **Pass/Fail:** Objective thresholds based on TRD requirements

## Progressive Disclosure

1. **Always load:** SKILL.md (this file)
2. **Load JIT:** Specific files based on task
   - Campaign design → `campaign-methodology.md`
   - EQ validation → `band-analysis.md`
   - Criteria → `pass-fail-criteria.md`
