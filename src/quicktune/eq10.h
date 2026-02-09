/**
 * @file eq10.h
 * @brief 10-Band Parametric EQ using CMSIS-DSP
 *
 * Implements a cascade of 10 biquad parametric EQ stages using
 * ARM CMSIS-DSP library (arm_biquad_cascade_df1_f32).
 *
 * @author DSP Team (Implementation Agent)
 * @date 2026-02-09
 * @target STM32H562 (Cortex-M33, 250 MHz)
 *
 * CONFIDENTIAL - Binary-only delivery
 */

#ifndef EQ10_H
#define EQ10_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* ============================================================================
 * CONSTANTS
 * ============================================================================ */

/** Number of EQ bands */
#define EQ10_NUM_BANDS              10

/** Number of coefficients per biquad (b0, b1, b2, a1, a2) */
#define EQ10_COEFFS_PER_BAND        5

/** Total number of coefficients */
#define EQ10_TOTAL_COEFFS           (EQ10_NUM_BANDS * EQ10_COEFFS_PER_BAND)

/** Number of state variables per biquad */
#define EQ10_STATE_PER_BAND         2

/** Total number of state variables */
#define EQ10_TOTAL_STATE            (EQ10_NUM_BANDS * EQ10_STATE_PER_BAND)

/* ============================================================================
 * PUBLIC API
 * ============================================================================ */

/**
 * @brief Initialize EQ10 with flat (bypass) response
 *
 * Must be called once at startup before processing audio.
 * Initializes all 10 biquad stages with unity gain (0 dB).
 */
void EQ10_Init(void);

/**
 * @brief Set gain for a single EQ band
 *
 * Updates the biquad coefficients for one band using RBJ cookbook formulas.
 * Changes take effect immediately on next ProcessBlock call.
 *
 * @param band Band index (0-9)
 * @param gain_dB Gain in dB (-12 to +12 recommended)
 * @param Q Quality factor (0.5 to 10.0 typical, 2.0 recommended)
 *
 * @return true if successful, false if invalid parameters
 */
bool EQ10_SetBandGain(int band, float gain_dB, float Q);

/**
 * @brief Set gains for all EQ bands at once
 *
 * Batch update of all 10 bands. More efficient than calling
 * EQ10_SetBandGain() 10 times.
 *
 * @param gains_dB Array of 10 gains in dB
 * @param Q Quality factor (same for all bands)
 *
 * @return true if successful, false if invalid parameters
 */
bool EQ10_SetAllGains(const float* gains_dB, float Q);

/**
 * @brief Process audio block through EQ10
 *
 * Apply 10-band parametric EQ to audio block.
 * CPU usage: ~20 cycles/sample/stage = 6400 cycles/block = 3.8% CPU
 *
 * @param input Input audio buffer (float32)
 * @param output Output audio buffer (float32)
 * @param numSamples Number of samples to process (typically 32)
 *
 * @note Input and output can be the same buffer (in-place processing)
 */
void EQ10_ProcessBlock(float* input, float* output, int numSamples);

/**
 * @brief Get current EQ10 coefficients
 *
 * Returns the current 50 biquad coefficients (5 per band × 10 bands).
 * Useful for saving/restoring presets.
 *
 * @param coeffs Output buffer for 50 coefficients
 */
void EQ10_GetCoefficients(float* coeffs);

/**
 * @brief Set EQ10 coefficients directly
 *
 * Allows loading precomputed coefficients (e.g., from preset).
 * Coefficients must be in CMSIS-DSP format: [b0, b1, b2, a1, a2] per stage.
 *
 * @param coeffs Array of 50 coefficients
 *
 * @return true if successful, false if invalid
 */
bool EQ10_SetCoefficients(const float* coeffs);

/**
 * @brief Reset EQ10 state (clear delay lines)
 *
 * Clears the biquad state variables (delay lines).
 * Call this when audio stream stops to prevent clicks on restart.
 */
void EQ10_Reset(void);

/**
 * @brief Get EQ10 band center frequency
 *
 * @param band Band index (0-9)
 * @return Center frequency in Hz, or 0.0 if invalid band
 */
float EQ10_GetBandFrequency(int band);

#ifdef __cplusplus
}
#endif

#endif /* EQ10_H */
