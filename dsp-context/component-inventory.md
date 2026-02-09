# Component Inventory

**Hardware components for Studio Monitor.**

---

## Woofer

- **Size:** 6.5"
- **Fs:** 45 Hz (resonant frequency)
- **X_max:** 10 mm (maximum linear excursion)
- **Voice Coil:** 1.5" (38mm)

---

## Tweeter

- **Type:** 1" dome
- **Frequency Range:** 2 kHz - 20 kHz
- **Crossover:** Active (DSP-based), 2nd-order Butterworth @ 2.5 kHz

---

## Amplifier

- **Woofer Amp:** 50W Class D
- **Tweeter Amp:** 25W Class D
- **THD+N:** < 0.05%

---

## MCU

- **Model:** STM32H562 (Cortex-M33, 250 MHz)
- **Memory:** 640 KB SRAM, 2 MB Flash
- **See:** `dsp-embedded-constraints/stm32h562-spec.md`

---

## MEMS Microphone

- **Model:** [To be specified]
- **Use:** QuickTune auto-calibration
- **See:** `dsp-bass-correction/mems-findings.md`

---

## Gyroscope

- **Model:** [To be specified]
- **Use:** QuickTune gyro sweep
- **Interface:** I2C

---

*Load this file for hardware-specific questions.*
