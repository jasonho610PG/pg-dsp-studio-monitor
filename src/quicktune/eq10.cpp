/**
 * @file eq10.cpp
 * @brief 10-Band Parametric EQ Implementation
 *
 * Implementation of 10-band parametric EQ using CMSIS-DSP biquad cascade.
 * Uses RBJ cookbook formulas for coefficient computation.
 *
 * @author DSP Team (Implementation Agent)
 * @date 2026-02-09
 * @target STM32H562 (Cortex-M33, 250 MHz)
 *
 * CONFIDENTIAL - Binary-only delivery
 */

#include "eq10.h"
#include "quicktune_config.h"
#include "arm_math.h"  // CMSIS-DSP
#include <math.h>
#include <string.h>

/* ============================================================================
 * PRIVATE STATE
 * ============================================================================ */

/** CMSIS-DSP biquad cascade instance */
static arm_biquad_casd_df1_inst_f32 s_eq10_instance;

/** Biquad coefficients: 5 per band × 10 bands = 50 floats */
static float s_eq10_coeffs[EQ10_TOTAL_COEFFS];

/** Biquad state: 2 per band × 10 bands = 20 floats */
static float s_eq10_state[EQ10_TOTAL_STATE];

/** Initialization flag */
static bool s_eq10_initialized = false;

/* ============================================================================
 * PRIVATE FUNCTIONS
 * ============================================================================ */

/**
 * @brief Design parametric EQ biquad using RBJ cookbook
 *
 * Computes biquad coefficients for parametric EQ filter.
 *
 * @param fc Center frequency (Hz)
 * @param gain_dB Gain in dB (positive = boost, negative = cut)
 * @param Q Quality factor
 * @param fs Sample rate (Hz)
 * @param coeffs Output buffer for 5 coefficients [b0, b1, b2, a1, a2]
 */
static void EQ10_DesignBiquad(float fc, float gain_dB, float Q, float fs, float* coeffs)
{
    // Compute intermediate values
    const float A = powf(10.0f, gain_dB / 40.0f);  // Amplitude = 10^(gain/40)
    const float w0 = 2.0f * M_PI * fc / fs;        // Angular frequency

    // Use CMSIS-DSP fast sin/cos if available, otherwise fallback to standard math
    float sin_w0, cos_w0;
    #ifdef ARM_MATH_CM33
    sin_w0 = arm_sin_f32(w0);
    cos_w0 = arm_cos_f32(w0);
    #else
    sin_w0 = sinf(w0);
    cos_w0 = cosf(w0);
    #endif

    const float alpha = sin_w0 / (2.0f * Q);       // Bandwidth parameter

    // RBJ parametric EQ formulas
    const float b0 = 1.0f + alpha * A;
    const float b1 = -2.0f * cos_w0;
    const float b2 = 1.0f - alpha * A;
    const float a0 = 1.0f + alpha / A;
    const float a1 = -2.0f * cos_w0;
    const float a2 = 1.0f - alpha / A;

    // Normalize by a0 (CMSIS-DSP expects normalized coefficients)
    coeffs[0] = b0 / a0;  // b0
    coeffs[1] = b1 / a0;  // b1
    coeffs[2] = b2 / a0;  // b2
    coeffs[3] = a1 / a0;  // a1 (note: already includes sign)
    coeffs[4] = a2 / a0;  // a2
}

/**
 * @brief Validate band index
 *
 * @param band Band index to validate
 * @return true if valid, false otherwise
 */
static inline bool EQ10_IsValidBand(int band)
{
    return (band >= 0 && band < EQ10_NUM_BANDS);
}

/* ============================================================================
 * PUBLIC API IMPLEMENTATION
 * ============================================================================ */

void EQ10_Init(void)
{
    // Initialize all coefficients to unity gain (0 dB)
    for (int band = 0; band < EQ10_NUM_BANDS; band++)
    {
        float* coeffs = &s_eq10_coeffs[band * EQ10_COEFFS_PER_BAND];
        const float fc = QUICKTUNE_BAND_FREQUENCIES[band];
        EQ10_DesignBiquad(fc, 0.0f, QUICKTUNE_EQ_Q, QUICKTUNE_SAMPLE_RATE, coeffs);
    }

    // Clear state
    memset(s_eq10_state, 0, sizeof(s_eq10_state));

    // Initialize CMSIS-DSP biquad cascade
    arm_biquad_cascade_df1_init_f32(
        &s_eq10_instance,
        EQ10_NUM_BANDS,      // Number of stages
        s_eq10_coeffs,       // Coefficients
        s_eq10_state         // State
    );

    s_eq10_initialized = true;
}

bool EQ10_SetBandGain(int band, float gain_dB, float Q)
{
    if (!s_eq10_initialized || !EQ10_IsValidBand(band))
    {
        return false;
    }

    // Clip gain to safe range
    if (gain_dB > QUICKTUNE_MAX_GAIN_DB)
        gain_dB = QUICKTUNE_MAX_GAIN_DB;
    if (gain_dB < QUICKTUNE_MIN_GAIN_DB)
        gain_dB = QUICKTUNE_MIN_GAIN_DB;

    // Clip Q to safe range (avoid instability)
    if (Q < 0.1f)
        Q = 0.1f;
    if (Q > 20.0f)
        Q = 20.0f;

    // Compute new coefficients
    float* coeffs = &s_eq10_coeffs[band * EQ10_COEFFS_PER_BAND];
    const float fc = QUICKTUNE_BAND_FREQUENCIES[band];
    EQ10_DesignBiquad(fc, gain_dB, Q, QUICKTUNE_SAMPLE_RATE, coeffs);

    return true;
}

bool EQ10_SetAllGains(const float* gains_dB, float Q)
{
    if (!s_eq10_initialized || gains_dB == NULL)
    {
        return false;
    }

    // Clip Q to safe range
    if (Q < 0.1f)
        Q = 0.1f;
    if (Q > 20.0f)
        Q = 20.0f;

    // Update all bands
    for (int band = 0; band < EQ10_NUM_BANDS; band++)
    {
        float gain_dB = gains_dB[band];

        // Clip gain to safe range
        if (gain_dB > QUICKTUNE_MAX_GAIN_DB)
            gain_dB = QUICKTUNE_MAX_GAIN_DB;
        if (gain_dB < QUICKTUNE_MIN_GAIN_DB)
            gain_dB = QUICKTUNE_MIN_GAIN_DB;

        // Compute coefficients
        float* coeffs = &s_eq10_coeffs[band * EQ10_COEFFS_PER_BAND];
        const float fc = QUICKTUNE_BAND_FREQUENCIES[band];
        EQ10_DesignBiquad(fc, gain_dB, Q, QUICKTUNE_SAMPLE_RATE, coeffs);
    }

    return true;
}

void EQ10_ProcessBlock(float* input, float* output, int numSamples)
{
    if (!s_eq10_initialized)
    {
        // Pass through if not initialized
        if (input != output)
        {
            memcpy(output, input, numSamples * sizeof(float));
        }
        return;
    }

    // Process through biquad cascade using CMSIS-DSP
    // CPU: ~20 cycles/sample/stage × 10 stages = 200 cycles/sample
    // For 32 samples: 6400 cycles = 25.6 µs @ 250 MHz = 3.8% CPU
    arm_biquad_cascade_df1_f32(&s_eq10_instance, input, output, numSamples);
}

void EQ10_GetCoefficients(float* coeffs)
{
    if (coeffs != NULL)
    {
        memcpy(coeffs, s_eq10_coeffs, sizeof(s_eq10_coeffs));
    }
}

bool EQ10_SetCoefficients(const float* coeffs)
{
    if (!s_eq10_initialized || coeffs == NULL)
    {
        return false;
    }

    // Copy coefficients
    memcpy(s_eq10_coeffs, coeffs, sizeof(s_eq10_coeffs));

    // Reinitialize CMSIS-DSP instance with new coefficients
    arm_biquad_cascade_df1_init_f32(
        &s_eq10_instance,
        EQ10_NUM_BANDS,
        s_eq10_coeffs,
        s_eq10_state
    );

    return true;
}

void EQ10_Reset(void)
{
    // Clear state (delay lines)
    memset(s_eq10_state, 0, sizeof(s_eq10_state));
}

float EQ10_GetBandFrequency(int band)
{
    if (!EQ10_IsValidBand(band))
    {
        return 0.0f;
    }

    return QUICKTUNE_BAND_FREQUENCIES[band];
}
