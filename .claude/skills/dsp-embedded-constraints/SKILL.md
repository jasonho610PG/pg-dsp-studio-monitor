---
name: dsp-embedded-constraints
description: STM32H562 hardware envelope, CMSIS-DSP patterns, ProcessBlock interface
sensitivity: CONFIDENTIAL
allowed-tools:
  - Read
portable: false
version: "1.0"
owner: DSP Team (Ivan/Derek/Jason)
---

# DSP Embedded Constraints

Real-time DSP constraints for STM32H562 (Cortex-M33, 250 MHz).

## Contents

| File | Purpose |
|------|---------|
| `stm32h562-spec.md` | MCU specifications, memory map, peripherals |
| `processblock-interface.md` | Audio processing interface contract |
| `cmsis-dsp-patterns.md` | Common CMSIS-DSP usage patterns |
| `binary-delivery.md` | Binary-only delivery requirements |

## Usage

This skill is used by:
- **research** agent — Feasibility checks (CPU/memory estimates)
- **prototype** agent — Validate prototype against embedded constraints
- **implementation** agent — CMSIS-DSP implementation patterns

## Key Constraints

- **Sample Rate:** 48 kHz
- **Block Size:** 32 samples (667 µs)
- **CPU Budget:** < 60% (allows headroom)
- **Memory:** 640 KB SRAM, 2 MB Flash
- **Math Library:** CMSIS-DSP (ARM-optimized)

## Progressive Disclosure

1. **Always load:** SKILL.md (this file)
2. **Load JIT:** Specific files based on task
   - Feasibility checks → `stm32h562-spec.md`
   - Implementation → `cmsis-dsp-patterns.md` + `processblock-interface.md`
   - Delivery → `binary-delivery.md`
