# Pass/Fail Criteria

**Objective thresholds for validation.**

---

## EQ10 Criteria

| Metric | Specification | Priority |
|--------|---------------|----------|
| Frequency Response | ±0.5 dB from target | MUST |
| Center Frequency Accuracy | ±2% of target | MUST |
| Q Accuracy | ±20% of target | SHOULD |
| THD+N @ 1 kHz, -6 dBFS | < 0.1% | MUST |
| CPU Usage | < 10% | MUST |
| Latency | < 2 ms | SHOULD |

---

## BassGuard Criteria

| Metric | Specification | Priority |
|--------|---------------|----------|
| Cone Excursion | < X_max | MUST |
| Attack Time | < 5 ms | MUST |
| Release Time | 50 ± 10 ms | SHOULD |
| THD+N (when not limiting) | < 0.1% | MUST |
| CPU Usage | < 5% | MUST |
| Transparency | No audible artifacts | SHOULD |

---

## QuickTune Criteria

| Metric | Specification | Priority |
|--------|---------------|----------|
| MEMS Calibration | ±1 dB flat response | MUST |
| Sweep Range | 20 Hz - 200 Hz | MUST |
| Sweep Smoothness | No discontinuities | MUST |
| Auto-EQ Accuracy | ±1 dB from target | SHOULD |
| CPU Usage | < 5% | MUST |

---

## General Criteria

| Metric | Specification | Priority |
|--------|---------------|----------|
| Total CPU Usage | < 60% | MUST |
| Memory Usage | < 100 KB | MUST |
| Stability | No crashes, 24-hour soak test | MUST |
| Compatibility | Works on all target hardware | MUST |

---

## Priority Definitions

- **MUST:** Critical for functionality (blocks release if fail)
- **SHOULD:** Important but not critical (can release with caveats)
- **MAY:** Nice-to-have (informational only)

---

## Pass/Fail Determination

**PASS:** All MUST requirements met, ≥80% of SHOULD requirements met

**CAUTION:** All MUST requirements met, <80% of SHOULD requirements met (requires review)

**FAIL:** Any MUST requirement fails (blocks release)

---

*Load this file for validation criteria.*
