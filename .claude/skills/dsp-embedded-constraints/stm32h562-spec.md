# STM32H562 Specifications

**Target MCU for Studio Monitor DSP.**

---

## Core

- **Architecture:** ARM Cortex-M33 with FPU
- **Clock:** 250 MHz
- **ISA:** Thumb-2, DSP extensions
- **FPU:** Single-precision (float32)

---

## Memory

| Type | Size | Use |
|------|------|-----|
| SRAM1 | 256 KB | General purpose |
| SRAM2 | 256 KB | DMA, audio buffers |
| SRAM3 | 128 KB | Backup, low-power |
| Flash | 2 MB | Code, constants |

**Budget:**
- Audio buffers: ~10 KB
- DSP state: ~5 KB
- Remaining: ~625 KB for other tasks

---

## Peripherals

- **I2S:** 2 channels, master/slave
- **SAI:** Serial Audio Interface
- **TIM:** High-resolution timers
- **DMA:** 16 channels (audio buffer management)

---

## Performance Estimates

**At 250 MHz:**
- 1 FLOP = 1 cycle (with FPU)
- 1 block (32 samples) = 667 µs
- 60% budget = 400 µs = 100,000 cycles
- ~3,125 cycles per sample

**Common Operations (cycles/sample):**
- Biquad filter: ~20 cycles
- FIR tap: ~5 cycles
- RMS calculation: ~10 cycles
- Gain multiply: ~1 cycle

---

## Design Guidelines

1. Use CMSIS-DSP functions (ARM-optimized)
2. Avoid dynamic memory allocation
3. Preallocate all buffers at init
4. Use float32 (native FPU type)
5. Profile early, optimize if needed

---

*Load this file for feasibility checks.*
