---
name: dsp-product-features
description: Studio Monitor product features (QuickTune, Dual Voicing, Mix Translation, SafeSound, Vibe Mode)
sensitivity: CONFIDENTIAL
allowed-tools:
  - Read
portable: false
version: "1.0"
owner: DSP Team (Ivan/Derek/Jason)
---

# DSP Product Features

Product feature specifications for Studio Monitor.

## Contents

| File | Purpose |
|------|---------|
| `quicktune-gyrosweep.md` | Gyro-based auto-tuning (sweep frequency with device tilt) |
| `dual-voicing.md` | Dual voicing system (Flat vs. Enhanced) |
| `mix-translation.md` | Mix translation mode (simulate different playback systems) |
| `safesound-limiter.md` | SafeSound limiter (speaker protection + loudness) |
| `vibe-mode.md` | Vibe mode (music-optimized tuning) |

## Usage

This skill is used by:
- **research** agent — Feature algorithm discovery
- **prototype** agent — Feature testbed implementation
- **implementation** agent — Feature embedded implementation
- **validation** agent — Feature TRD compliance
- **documentation** agent — Feature documentation

## Key Features

- **QuickTune:** Gyro sweep + MEMS mic → auto room correction
- **Dual Voicing:** Switch between Flat (reference) and Enhanced (consumer-friendly)
- **Mix Translation:** Simulate car, phone, laptop speakers
- **SafeSound:** Limiter with speaker protection + perceived loudness
- **Vibe Mode:** Music-optimized tuning (enhanced bass, smooth highs)

## Progressive Disclosure

1. **Always load:** SKILL.md (this file)
2. **Load JIT:** Specific files based on task
