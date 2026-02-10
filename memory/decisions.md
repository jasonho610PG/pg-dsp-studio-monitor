# Decisions

**Architectural choices and rationale.**

---

## 2026-02-10: Stepped Sine + Goertzel for QuickTune (not Swept Sine or NLMS)

**Decision:** Use Stepped Sine + Goertzel detection as the QuickTune room correction algorithm.

**Alternatives considered:**
1. Exponential Swept Sine (Farina) — Full impulse response, ~50 KB SRAM, 2-5% CPU
2. Pink Noise + Welch PSD — Requires FFT, ~130 KB SRAM
3. **Stepped Sine + Goertzel** — Single-frequency detection, ~20 KB SRAM, < 0.1% CPU
4. Adaptive NLMS — Continuous tracking, 5-8% CPU
5. MLS (Maximum Length Sequence) — Fast, but perceptually harsh
6. Chirp-Z Transform — Complex, overkill for 10 bands

**Rationale:**
- Simplest implementation (~5 lines per frequency)
- Lowest resource usage (20 KB SRAM, < 0.1% CPU)
- Fastest calibration (~3 sec/pass, 9 sec for 3 iterations)
- Natural fit with gyroscope-based UX (move monitor, measure)
- Fixed Q=2.0 acceptable for consumer auto-EQ (matches Sonos Trueplay approach)

**Trade-offs:**
- Cannot estimate Q factor (fixed Q=2.0)
- Sequential tone playback (not real-time tracking)
- Limited to 10 discrete frequencies (EQ10 bands)

**Outcome:** 10/10 rooms pass, 0.394 dB avg error, 3.97% CPU, 556 bytes. All targets exceeded.

---

## 2026-02-10: MEMS calibration via static offsets (not adaptive)

**Decision:** Use fixed MEMS calibration offsets (+3.0 dB @ 25 Hz, +1.5 dB @ 40 Hz, 0.0 dB for bands 3-10) rather than adaptive per-unit calibration.

**Rationale:**
- MEMS microphone roll-off is consistent across units (manufacturing spec)
- Static offsets require zero additional memory or CPU
- Avoids complexity of adaptive calibration loop
- Can be updated per-unit via factory calibration if needed

**Trade-offs:**
- Won't compensate for unit-to-unit MEMS variation (mitigated by factory cal)
- Fixed offsets may drift with temperature (low risk, < 0.5 dB)

**Outcome:** Low-freq bands achieve 0.254 dB avg error (equivalent to mid-freq bands at 0.277 dB).

---

## 2026-02-10: ±12 dB gain clipping limit for EQ10

**Decision:** Clip all correction gains to ±12 dB maximum.

**Rationale:**
- Prevents over-correction in pathological rooms
- Avoids biquad instability at extreme gains
- Standard practice in consumer auto-EQ (Sonos: ±10 dB, Dirac: ±12 dB)
- Speaker protection (excessive boost at resonance risks damage)

**Trade-offs:**
- Rooms with > 12 dB deviations get partial correction only
- Creates smoothness jumps when adjacent bands are clamped differently

**Outcome:** QT-SMOOTH-001 spec updated to reflect stability > smoothness hierarchy. 10/10 rooms still meet ±1 dB accuracy target.

---

## 2026-02-10: Python prototype before C++ implementation

**Decision:** Always build a Python prototype with validation suite before writing embedded C++.

**Rationale:**
- Python iteration is 10x faster than embedded C++ debug cycles
- NumPy/SciPy provide reference implementations for verification
- Matplotlib generates validation plots automatically
- Prototype proves algorithm feasibility before hardware commitment
- Bit-accurate comparison validates C++ implementation

**Trade-offs:**
- Extra development step (~1-2 hours)
- Python != embedded (float precision, fixed-point differences)

**Outcome:** Python prototype caught algorithm issues early. C++ implementation matched prototype results within 0.01 dB.
