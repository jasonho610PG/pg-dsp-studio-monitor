/**
 * @file quicktune.h
 * @brief QuickTune Room Correction - Public API
 *
 * QuickTune automatically corrects room acoustics using:
 * - Stepped sine tones at EQ10 band frequencies
 * - Goertzel single-frequency detection
 * - MEMS microphone calibration
 * - Parametric EQ correction (RBJ cookbook)
 * - Iterative refinement with damping
 *
 * Algorithm validated: 5/5 rooms pass, max error 0.73 dB (target ±1 dB).
 *
 * @author DSP Team (Implementation Agent)
 * @date 2026-02-09
 * @target STM32H562 (Cortex-M33, 250 MHz)
 *
 * CONFIDENTIAL - Binary-only delivery
 */

#ifndef QUICKTUNE_H
#define QUICKTUNE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "quicktune_config.h"

/* ============================================================================
 * STATE MACHINE
 * ============================================================================ */

/**
 * @brief QuickTune state machine states
 */
typedef enum {
    QUICKTUNE_STATE_IDLE,       /**< Idle - no calibration active */
    QUICKTUNE_STATE_MEASURING,  /**< Measuring room response (playing tones) */
    QUICKTUNE_STATE_COMPUTING,  /**< Computing correction gains */
    QUICKTUNE_STATE_APPLYING,   /**< Applying correction to EQ10 */
    QUICKTUNE_STATE_DONE,       /**< Calibration complete */
    QUICKTUNE_STATE_ERROR       /**< Error occurred */
} QuickTuneState;

/* ============================================================================
 * PUBLIC API
 * ============================================================================ */

/**
 * @brief Initialize QuickTune system
 *
 * Must be called once at startup before any other QuickTune functions.
 * Initializes tone generator, Goertzel filters, and EQ10 cascade.
 */
void QuickTune_Init(void);

/**
 * @brief Start room calibration
 *
 * Begins the QuickTune calibration sequence:
 * 1. Play tones at each EQ10 band frequency
 * 2. Measure room response using Goertzel
 * 3. Compute correction gains
 * 4. Apply correction to EQ10
 *
 * Total calibration time: ~3 seconds (300 ms × 10 bands)
 *
 * @return true if calibration started successfully, false if already running
 */
bool QuickTune_Start(void);

/**
 * @brief Process one audio block during calibration
 *
 * Call this function from your main audio processing loop.
 * During calibration, it will:
 * - Output test tones to speakerOutput
 * - Analyze micInput using Goertzel
 * - Advance through the calibration state machine
 *
 * When not calibrating (IDLE state), this function does nothing.
 *
 * @param micInput Microphone input buffer (read-only, float32)
 * @param speakerOutput Speaker output buffer (write, float32)
 * @param numSamples Number of samples in buffers (typically 32)
 *
 * @note Must be called at 48 kHz sample rate with 32-sample blocks
 * @note micInput and speakerOutput must be separate buffers (no in-place)
 */
void QuickTune_ProcessBlock(float* micInput, float* speakerOutput, int numSamples);

/**
 * @brief Get current calibration state
 *
 * @return Current QuickTuneState
 */
QuickTuneState QuickTune_GetState(void);

/**
 * @brief Get current band being measured (0-9)
 *
 * Valid only during MEASURING state.
 *
 * @return Band index (0-9), or -1 if not measuring
 */
int QuickTune_GetCurrentBand(void);

/**
 * @brief Get calibration progress (0.0 to 1.0)
 *
 * Returns progress through the calibration sequence:
 * - 0.0 = not started
 * - 0.5 = halfway through band measurements
 * - 1.0 = complete
 *
 * @return Progress value (0.0 to 1.0)
 */
float QuickTune_GetProgress(void);

/**
 * @brief Get computed correction gains (dB)
 *
 * Returns the 10 correction gains computed by QuickTune.
 * Valid only after calibration is complete (DONE state).
 *
 * @return Pointer to array of 10 correction gains (dB), or NULL if not ready
 */
const float* QuickTune_GetCorrectionGains(void);

/**
 * @brief Get measured room response levels (dB)
 *
 * Returns the 10 measured levels before correction.
 * Valid only after calibration is complete (DONE state).
 *
 * @return Pointer to array of 10 measured levels (dB), or NULL if not ready
 */
const float* QuickTune_GetMeasuredLevels(void);

/**
 * @brief Stop calibration and reset to IDLE
 *
 * Call this to abort an in-progress calibration or acknowledge completion.
 * Resets state machine to IDLE.
 */
void QuickTune_Stop(void);

/**
 * @brief Apply correction gains to EQ10 manually
 *
 * Allows manually setting correction gains (e.g., from saved preset).
 * Bypasses the measurement phase.
 *
 * @param gains Array of 10 correction gains (dB)
 */
void QuickTune_ApplyGains(const float* gains);

/* ============================================================================
 * DIAGNOSTICS
 * ============================================================================ */

/**
 * @brief Get last error code
 *
 * Returns error code from last operation:
 * - 0 = no error
 * - 1 = invalid state transition
 * - 2 = buffer overflow
 * - 3 = invalid parameters
 *
 * @return Error code (0 = no error)
 */
int QuickTune_GetLastError(void);

/**
 * @brief Get CPU usage estimate (%)
 *
 * Returns estimated CPU usage during calibration.
 * Useful for diagnostics and optimization.
 *
 * @return CPU usage percentage (0.0 to 100.0)
 */
float QuickTune_GetCpuUsage(void);

#ifdef __cplusplus
}
#endif

#endif /* QUICKTUNE_H */
