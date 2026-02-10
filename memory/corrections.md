# Corrections

**Capture when Claude makes mistakes.**

---

## 2026-02-10: Integer division in C macro definitions

**What happened:** `quicktune_config.h` macros like `#define QUICKTUNE_TONE_ANALYSIS_SAMPLES ((QUICKTUNE_SAMPLE_RATE * 100) / 1000)` caused clang-tidy `bugprone-integer-division` warnings. Initial fix of adding `(float)` casts around macro usages didn't resolve it because division happens inside the macro expansion.

**Why it matters:** Integer division in macros is a subtle bug source. Even when the result is used in a float context, the division truncates before the cast.

**Correction:** Pre-compute macro values as literal integers (`4800`, `9600`, `14400`) with comments showing the formula. Also use intermediate float variables at usage sites.

**Impact:** Always define sample-count macros as literal values, not computed expressions, to avoid integer division issues across the codebase.

---

## 2026-02-10: Validation test logic errors (QT-MEMS-001, QT-SMOOTH-001)

**What happened:** Validation agent's auto-generated `trd_validation.py` had two flawed tests:
1. QT-MEMS-001: Created synthetic room with modes matching MEMS deviations instead of testing the MEMS calibration pathway itself.
2. QT-SMOOTH-001: Applied smoothness requirement (< 6 dB jump) uniformly, even to gain-clamped bands where jumps are expected by design.

**Why it matters:** Flawed tests produce false FAILs, creating confusion in milestone reports and delaying delivery.

**Correction:**
1. QT-MEMS-001: Test now compares low-freq band accuracy (with MEMS cal) to mid-freq band accuracy across all 10 real rooms.
2. QT-SMOOTH-001: Exempts band transitions where either adjacent band has >= 10 dB correction (near clipping limit).

**Impact:** Validation scripts must replicate exact embedded behavior and test intent, not just surface-level checks. Review test methodology before trusting auto-generated validation.

---

## 2026-02-10: Background agents hide progress from user

**What happened:** Spawned validation and documentation agents with `run_in_background: true`, which prevented the user from seeing real-time progress. User was confused about status.

**Why it matters:** User loses visibility into what's happening, leading to frustration and "is it done?" questions.

**Correction:** For milestone-review and other user-facing workflows, run agents in foreground (not background) so the user can see progress. Only use background for truly independent tasks where the user doesn't need to track progress.

**Impact:** Default to foreground agents for user-requested commands.
