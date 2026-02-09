---
name: dsp-juce-testbed
description: JUCE plugin architecture and build workflow for DSP prototyping
sensitivity: INTERNAL
allowed-tools:
  - Read
  - Write
  - Bash(cmake:*)
  - Bash(make:*)
portable: true
version: "1.0"
owner: DSP Team (Ivan/Derek/Jason)
---

# DSP JUCE Testbed

JUCE plugin architecture for desktop DSP prototyping.

## Contents

| File | Purpose |
|------|---------|
| `plugin-architecture.md` | JUCE AudioProcessor structure, parameter management |
| `build-workflow.md` | CMake, Projucer, build process |

## Usage

This skill is used by:
- **prototype** agent — Build JUCE testbed plugins

## Key Concepts

- **AudioProcessor:** Main processing class (inherits from juce::AudioProcessor)
- **processBlock():** Audio callback (similar to embedded ProcessBlock)
- **Parameters:** APVTS (AudioProcessorValueTreeState) for parameter management
- **Visualization:** Real-time plots, frequency response, level meters

## Progressive Disclosure

1. **Always load:** SKILL.md (this file)
2. **Load JIT:** Specific files based on task
   - Plugin structure → `plugin-architecture.md`
   - Build issues → `build-workflow.md`
