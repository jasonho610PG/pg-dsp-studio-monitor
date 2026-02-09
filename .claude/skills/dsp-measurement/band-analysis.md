# Band Analysis

**Per-band frequency response analysis for parametric EQ.**

---

## Overview

Validate each EQ band independently to ensure accurate frequency, gain, and Q.

---

## Test Procedure

For each band (1-10):

1. **Set EQ:**
   - Target band: +6 dB (or -6 dB for cut)
   - All other bands: 0 dB (bypass)
   - Q: nominal value (e.g., 1.0)

2. **Measure:**
   - Play log sweep (20 Hz - 20 kHz)
   - Capture frequency response

3. **Analyze:**
   - Identify peak (boost) or notch (cut) frequency
   - Measure gain at center frequency
   - Compute -3 dB bandwidth (for Q validation)

4. **Compare to TRD:**
   - Center frequency: within ±2% of target
   - Gain: within ±0.5 dB of target
   - Q: within ±20% of target

---

## Example: Band 3 (63 Hz)

**Target:**
- Center frequency: 63 Hz
- Gain: +6 dB
- Q: 1.0

**Measured:**
- Peak at 62.8 Hz (within ±2%)
- Gain at peak: +5.9 dB (within ±0.5 dB)
- -3 dB bandwidth: 63 Hz (Q ≈ 1.0, within ±20%)

**Result:** PASS

---

## Frequency Resolution

For accurate band analysis, use sufficient frequency resolution:

- **Sweep length:** ≥10 seconds (more is better)
- **FFT size:** ≥8192 (higher for low frequencies)
- **Smoothing:** 1/12 octave (matches typical EQ resolution)

---

## Automation

Python script example:

```python
import numpy as np
import scipy.signal as signal

# Generate sweep
fs = 48000
sweep = signal.chirp(t, 20, 10, 20000, method='logarithmic')

# Measure response
output = measure_dut(sweep)

# Compute frequency response
H = np.fft.rfft(output) / np.fft.rfft(sweep)
freqs = np.fft.rfftfreq(len(sweep), 1/fs)

# Find peak near target frequency
idx = np.argmin(np.abs(freqs - target_freq))
gain_dB = 20 * np.log10(np.abs(H[idx]))

print(f"Gain at {target_freq} Hz: {gain_dB:.2f} dB")
```

---

*Load this file for EQ band validation.*
