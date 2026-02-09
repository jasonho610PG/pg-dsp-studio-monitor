# DSP Constraints

**Hardware envelope and real-time requirements.**

---

## Target Platform

**MCU:** STM32H562 (Cortex-M33, 250 MHz)
**Memory:** 640 KB SRAM, 2 MB Flash
**Peripherals:** I2S, SAI, TIM
**Math Library:** CMSIS-DSP (ARM-optimized)

---

## Real-Time Requirements

**Sample Rate:** 48 kHz
**Block Size:** 32 samples (667 µs)
**Latency Budget:** < 10 ms round-trip
**CPU Budget:** < 60% (allows headroom for other tasks)

---

## Audio Interface

**Function:** `void ProcessBlock(float* input, float* output, int numSamples)`
**Channels:** Mono or stereo (configurable)
**Format:** Float32 (-1.0 to +1.0)

See `dsp-embedded-constraints/processblock-interface.md` for details.

---

## Development Tools

- **Testbed:** JUCE (desktop prototyping)
- **Validation:** Python + NumPy/SciPy
- **Embedded:** STM32CubeIDE + CMSIS-DSP
- **Measurement:** REW, APx, custom Python scripts

---

*Load details JIT via skills when needed.*
