/**
 * @file quicktune_config.h
 * @brief QuickTune Configuration Constants
 *
 * Configuration parameters for the QuickTune room correction algorithm.
 * Based on validated Python prototype results (5/5 rooms pass, max error 0.73 dB).
 *
 * @author DSP Team (Implementation Agent)
 * @date 2026-02-09
 * @target STM32H562 (Cortex-M33, 250 MHz)
 *
 * CONFIDENTIAL - Binary-only delivery
 */

#ifndef QUICKTUNE_CONFIG_H
#define QUICKTUNE_CONFIG_H

/* ============================================================================
 * AUDIO CONFIGURATION
 * ============================================================================ */

/** Sample rate (Hz) */
#define QUICKTUNE_SAMPLE_RATE           48000

/** Block size (samples per ProcessBlock call) */
#define QUICKTUNE_BLOCK_SIZE            32

/** Number of EQ10 bands */
#define QUICKTUNE_NUM_BANDS             10

/* ============================================================================
 * TIMING CONFIGURATION
 * ============================================================================ */

/** Settling time per tone (ms) - allows transients to decay */
#define QUICKTUNE_TONE_SETTLING_MS      200

/** Analysis time per tone (ms) - Goertzel measurement window */
#define QUICKTUNE_TONE_ANALYSIS_MS      100

/** Total tone duration (ms) */
#define QUICKTUNE_TONE_TOTAL_MS         (QUICKTUNE_TONE_SETTLING_MS + QUICKTUNE_TONE_ANALYSIS_MS)

/** Total calibration time (ms) for all bands */
#define QUICKTUNE_TOTAL_CAL_TIME_MS     (QUICKTUNE_TONE_TOTAL_MS * QUICKTUNE_NUM_BANDS)

/* Convert to samples (integer values, computed via integer-safe expressions) */
#define QUICKTUNE_TONE_SETTLING_SAMPLES 9600   /* 48000 * 200 / 1000 */
#define QUICKTUNE_TONE_ANALYSIS_SAMPLES 4800   /* 48000 * 100 / 1000 */
#define QUICKTUNE_TONE_TOTAL_SAMPLES    14400  /* 48000 * 300 / 1000 */

/* ============================================================================
 * EQ10 BAND FREQUENCIES (Hz)
 * ============================================================================ */

/** EQ10 band center frequencies */
static const float QUICKTUNE_BAND_FREQUENCIES[QUICKTUNE_NUM_BANDS] = {
    25.0f,      // Band 1
    40.0f,      // Band 2
    63.0f,      // Band 3
    100.0f,     // Band 4
    160.0f,     // Band 5
    250.0f,     // Band 6
    400.0f,     // Band 7
    630.0f,     // Band 8
    1000.0f,    // Band 9
    1600.0f     // Band 10
};

/* ============================================================================
 * MEMS MICROPHONE CALIBRATION
 * ============================================================================ */

/**
 * MEMS microphone calibration offsets (dB)
 * Compensate for MEMS roll-off at low frequencies
 * Measured during factory calibration
 */
static const float QUICKTUNE_MEMS_CALIBRATION[QUICKTUNE_NUM_BANDS] = {
    3.0f,       // 25 Hz: +3.0 dB (significant roll-off)
    1.5f,       // 40 Hz: +1.5 dB (moderate roll-off)
    0.0f,       // 63 Hz: 0.0 dB (flat response starts)
    0.0f,       // 100 Hz
    0.0f,       // 160 Hz
    0.0f,       // 250 Hz
    0.0f,       // 400 Hz
    0.0f,       // 630 Hz
    0.0f,       // 1000 Hz
    0.0f        // 1600 Hz
};

/* ============================================================================
 * CORRECTION PARAMETERS
 * ============================================================================ */

/** Maximum correction gain (dB) - clip to ±12 dB range */
#define QUICKTUNE_MAX_GAIN_DB           12.0f

/** Minimum correction gain (dB) */
#define QUICKTUNE_MIN_GAIN_DB           (-12.0f)

/** Fixed Q factor for all EQ10 bands */
#define QUICKTUNE_EQ_Q                  2.0f

/** Target accuracy (dB) - validation threshold */
#define QUICKTUNE_TARGET_ACCURACY_DB    1.0f

/* ============================================================================
 * ITERATIVE REFINEMENT
 * ============================================================================ */

/** Maximum number of iterative refinement passes */
#define QUICKTUNE_MAX_ITERATIONS        3

/** Damping factor for iterative refinement (0.0 to 1.0) */
#define QUICKTUNE_DAMPING_FACTOR        0.7f

/** Enable/disable iterative refinement */
#define QUICKTUNE_ENABLE_ITERATION      1

/* ============================================================================
 * TONE GENERATION
 * ============================================================================ */

/** Tone amplitude (0.0 to 1.0) - moderate level to avoid clipping */
#define QUICKTUNE_TONE_AMPLITUDE        0.5f

/** Fade in/out duration (samples) - 10 ms to reduce clicks */
#define QUICKTUNE_FADE_SAMPLES          480  // 10 ms @ 48 kHz

/* ============================================================================
 * MEMORY ALLOCATION
 * ============================================================================ */

/** Size of Goertzel coefficient array (one per band) */
#define QUICKTUNE_GOERTZEL_COEFF_SIZE   (QUICKTUNE_NUM_BANDS)

/** Size of EQ10 biquad coefficient array (5 coeffs per band) */
#define QUICKTUNE_EQ10_COEFF_SIZE       (QUICKTUNE_NUM_BANDS * 5)

/** Size of EQ10 biquad state array (2 states per band) */
#define QUICKTUNE_EQ10_STATE_SIZE       (QUICKTUNE_NUM_BANDS * 2)

/* ============================================================================
 * CPU BUDGET ESTIMATES
 * ============================================================================ */

/**
 * CPU Usage Estimates (STM32H562 @ 250 MHz):
 *
 * During Calibration (per block):
 * - Tone generation: ~3 cycles/sample = 96 cycles/block = 0.4 µs
 * - Goertzel filter:  ~4 cycles/sample = 128 cycles/block = 0.5 µs
 * - Total:            ~224 cycles/block = 0.9 µs = 0.13% CPU
 *
 * Post-Calibration (EQ10 processing):
 * - 10 biquads:       ~20 cycles/sample/stage = 6,400 cycles/block = 25.6 µs = 3.8% CPU
 *
 * Well within 60% CPU budget.
 */

/* ============================================================================
 * MEMORY USAGE ESTIMATES
 * ============================================================================ */

/**
 * Memory Usage (Static Allocation):
 *
 * QuickTune State:
 * - Tone generator:      12 bytes (3 floats)
 * - Goertzel state:      12 bytes (3 floats)
 * - Sample counter:      4 bytes (uint32_t)
 * - Band index:          4 bytes (int)
 * - Measured levels:     40 bytes (10 floats)
 * - Correction gains:    40 bytes (10 floats)
 * - State enum:          4 bytes
 * - Subtotal:            ~116 bytes
 *
 * EQ10 State:
 * - Biquad instance:     ~40 bytes (CMSIS-DSP struct)
 * - Coefficients:        200 bytes (50 floats)
 * - State array:         80 bytes (20 floats)
 * - Subtotal:            ~320 bytes
 *
 * Configuration (const):
 * - Band frequencies:    40 bytes (10 floats)
 * - MEMS calibration:    40 bytes (10 floats)
 * - Goertzel coeffs:     40 bytes (10 floats)
 * - Subtotal:            ~120 bytes
 *
 * Total:                 ~556 bytes
 *
 * Well within 640 KB SRAM budget.
 */

#endif /* QUICKTUNE_CONFIG_H */
