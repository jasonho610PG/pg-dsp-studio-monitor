# Milestone Process

**Calvin's 5-phase milestone process for DSP feature development.**

---

## Phase 1: Investigation

**Goal:** Validate feasibility, select algorithm

**Activities:**
- Literature review (research agent)
- Algorithm selection
- Feasibility check (CPU/memory)
- IP risk assessment

**Deliverable:** Research summary

**Command:** `/research [feature]`

---

## Phase 2: Prototype

**Goal:** Validate algorithm in testbed

**Activities:**
- JUCE plugin or Python implementation (prototype agent)
- Test with representative signals
- Measure frequency response, THD+N
- Compare to specification

**Deliverable:** Prototype code + validation report

**Command:** `/prototype [feature]`

---

## Phase 3: Implementation

**Goal:** Embedded C++ for STM32H562

**Activities:**
- CMSIS-DSP implementation (implementation agent)
- CPU profiling
- Memory optimization
- Unit tests

**Deliverable:** Embedded code + performance report

**Command:** `/implement [feature]`

---

## Phase 4: Validation

**Goal:** Hardware measurement campaign

**Activities:**
- Design measurement campaign (validation agent)
- Execute measurements (REW, APx, Python)
- Compare to TRD
- Generate validation report

**Deliverable:** Validation report + plots

**Command:** `/validate [feature] [trd-version]`

---

## Phase 5: Delivery

**Goal:** Binary release + documentation

**Activities:**
- Compile binary (no source code distribution)
- Generate milestone report (documentation agent)
- Review with stakeholders
- Release

**Deliverable:** Binary + milestone report

**Command:** `/milestone-review delivery [feature]`

---

## Milestone Review Command

Auto-generate milestone package (validation + documentation):

```
/milestone-review [phase] [feature]
```

Examples:
- `/milestone-review validation EQ10`
- `/milestone-review delivery BassGuard`

---

*Load this file for milestone planning.*
