# Measurement Campaign Methodology

**How to design and execute measurement campaigns.**

---

## Campaign Structure

1. **Objectives** — What are we validating? (e.g., "EQ10 frequency response")
2. **Test Signals** — What signals to use? (sweep, sine, noise, music)
3. **Metrics** — What to measure? (frequency response, THD+N, latency)
4. **Criteria** — Pass/fail thresholds (from TRD)
5. **Tools** — Measurement software/hardware (REW, APx, Python)

---

## Test Signal Types

| Signal | Use Case | Notes |
|--------|----------|-------|
| Sine Sweep | Frequency response | 20 Hz - 20 kHz, log sweep |
| Sine Tone | THD+N, specific frequency | 1 kHz typical |
| Pink Noise | Broadband response | Equal energy per octave |
| Music | Subjective evaluation | Representative tracks |

---

## Measurement Tools

### REW (Room EQ Wizard)
- **Use for:** Frequency response, RT60, waterfall
- **Pros:** Free, comprehensive, easy to use
- **Cons:** Limited automation

### Audio Precision APx
- **Use for:** THD+N, IMD, high-precision measurements
- **Pros:** Industry standard, highly accurate
- **Cons:** Expensive

### Python + NumPy/SciPy
- **Use for:** Custom analysis, automation
- **Pros:** Flexible, scriptable, repeatable
- **Cons:** Requires coding

---

## Campaign Phases

1. **Setup** — Configure DUT, connect measurement equipment
2. **Calibration** — Verify measurement chain (loopback test)
3. **Execution** — Run test signals, collect data
4. **Analysis** — Process data, compute metrics
5. **Reporting** — Generate plots, compare to criteria

---

## Example: EQ10 Frequency Response Campaign

**Objective:** Validate EQ10 frequency response (±0.5 dB)

**Test Signal:** Log sweep (20 Hz - 20 kHz, 10 seconds)

**Metrics:** Frequency response per band

**Criteria:** ±0.5 dB from target response

**Tools:** REW or Python

**Process:**
1. Set EQ10 band 1 to +6 dB, all others to 0 dB
2. Play sweep, measure frequency response
3. Check band 1 center frequency: is gain +6 ± 0.5 dB?
4. Repeat for all 10 bands
5. Generate report with plots

---

*Load this file for campaign design.*
