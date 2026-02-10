# Patterns

**Recurring methods and approaches.**

---

## 2026-02-10: 5-phase milestone pipeline for features

**Pattern:** Each feature follows Calvin's 5-phase process via commands:
1. `/research` — Algorithm discovery, compare approaches, recommend one
2. `/prototype` — Python validation prototype, test against room scenarios
3. `/implement` — Embedded C++ for STM32H562, CMSIS-DSP
4. `/validate` — TRD compliance campaign (10-room test suite)
5. `/milestone-review` — Generate validation + documentation package

**Why:** Structured approach ensures each phase builds on the last. Python prototype validates algorithm before committing to C++. Validation catches issues before delivery.

**Examples:**
- QuickTune completed all 5 phases in a single session
- Research compared 6 algorithms, narrowed to Stepped Sine + Goertzel
- Prototype proved 5/5 rooms pass, implementation achieved 10/10

**Impact:** Repeatable pipeline for EQ10, BassGuard, SafeSound, and future features.

---

## 2026-02-10: Parallel agent spawning for milestone-review

**Pattern:** `/milestone-review` spawns two agents in parallel:
1. Validation agent — fixes tests, re-runs validation, generates report
2. Documentation agent — reads validation data, generates milestone report

**Why:** Independent tasks can run concurrently, reducing total time.

**Caveat:** Don't run in background if user needs to see progress.

**Impact:** Milestone packages generated in ~5 minutes instead of ~10.

---

## 2026-02-10: Iterative refinement with damping for room correction

**Pattern:** Single-shot correction achieves ~2 dB accuracy. Iterative refinement with 0.7 damping factor converges to < 1 dB in 2-3 passes.

**Why:** Goertzel measures at discrete frequencies — biquad filter interaction between bands means first correction shifts adjacent bands slightly. Iterative passes clean up residual errors.

**Formula:** `new_gain = old_gain + damping * (target - measured)`

**Impact:** Critical for meeting ±1 dB TRD accuracy requirement. 0.7 damping balances speed vs. stability (tested: 0.5 too slow, 0.9 risks oscillation).

---

## 2026-02-10: Gain clipping priority hierarchy

**Pattern:** When correction requirements conflict, prioritize:
1. **Stability** — Clip gains to ±12 dB (prevent instability)
2. **Accuracy** — Correct as much as possible within limits
3. **Smoothness** — Best effort, secondary to 1 & 2

**Why:** Pathological rooms (Room 10: ±11 dB modes) require corrections near the clipping limit. Allowing unlimited gains risks speaker damage and oscillation.

**Impact:** QT-SMOOTH-001 spec clarified to reflect this hierarchy. Validation tests exempt gain-clamped transitions from smoothness checks.
