# Architecture Rules

**Always-on rules for pg-dsp-studio-monitor.**

---

## Team & Ownership

**Team:** DSP (Ivan, Derek, Jason)
**Product:** Studio Monitor Bass Correction Features
**Stakeholders:** Calvin (Program Manager), Andy (Engineering Manager)

---

## Milestone Process

This system supports Calvin's 5-phase milestone process:

1. **Investigation** — Algorithm research, feasibility
2. **Prototype** — JUCE testbed validation
3. **Implementation** — Embedded C++ (STM32H562)
4. **Validation** — Measurement campaigns, TRD compliance
5. **Delivery** — Binary-only release + milestone report

Use `/milestone-review` to auto-generate validation + documentation packages.

---

## IP Protection

**Delivery Format:** Binary-only (.bin files)
**Source Code:** Remains proprietary (not distributed to customers)
**Algorithm Details:** CONFIDENTIAL skills only

---

## Integration with Legacy

**Legacy System:** `/Users/jasonho610/Desktop/studio-monitor`
**Strategy:** Read-only references
- Testbed code → prototype agent references
- Measurement tools → validation agent references
- Validated algorithms → extracted to CONFIDENTIAL skills (methodology only)

DO NOT modify legacy system. New work happens in this repo.

---

*Aligned with platform-context/SPEC.md*
