/**
 * @file quicktune_example.cpp
 * @brief QuickTune Integration Example
 *
 * Example code showing how to integrate QuickTune into your audio system.
 * This file demonstrates:
 * - Initialization
 * - Calibration workflow
 * - Progress monitoring
 * - Error handling
 * - Preset management
 *
 * @author DSP Team (Implementation Agent)
 * @date 2026-02-09
 * @target STM32H562 (Cortex-M33, 250 MHz)
 *
 * CONFIDENTIAL - Binary-only delivery
 */

#include "quicktune.h"
#include "eq10.h"
#include <stdio.h>

/* ============================================================================
 * EXAMPLE 1: Basic Integration
 * ============================================================================ */

/**
 * @brief Initialize audio system at startup
 */
void Example1_Init(void)
{
    // Initialize QuickTune (must be called once at startup)
    QuickTune_Init();

    printf("QuickTune initialized\n");
}

/**
 * @brief Start calibration (e.g., from button press)
 */
void Example1_StartCalibration(void)
{
    if (QuickTune_Start())
    {
        printf("Calibration started...\n");
    }
    else
    {
        printf("Failed to start calibration (already running?)\n");
    }
}

/**
 * @brief Main audio callback (called every 32 samples @ 48 kHz = 667 µs)
 *
 * @param micInput Microphone input buffer (32 samples)
 * @param speakerOutput Speaker output buffer (32 samples)
 */
void Example1_AudioCallback(float* micInput, float* speakerOutput)
{
    const int numSamples = 32;

    // Always call QuickTune_ProcessBlock
    // During calibration, it generates tones and analyzes mic input
    // When idle, it does nothing (outputs silence)
    QuickTune_ProcessBlock(micInput, speakerOutput, numSamples);

    // Check state
    QuickTuneState state = QuickTune_GetState();

    if (state == QUICKTUNE_STATE_DONE)
    {
        // Calibration complete!
        printf("Calibration complete!\n");

        // Get correction gains
        const float* gains = QuickTune_GetCorrectionGains();
        if (gains != NULL)
        {
            printf("Correction gains (dB):\n");
            for (int i = 0; i < 10; i++)
            {
                printf("  Band %d: %+.2f dB\n", i + 1, gains[i]);
            }
        }

        // Acknowledge completion (return to IDLE)
        QuickTune_Stop();
    }
}

/* ============================================================================
 * EXAMPLE 2: Progress Monitoring
 * ============================================================================ */

/**
 * @brief Update UI with calibration progress
 *
 * Call this periodically (e.g., every 100 ms) to update progress bar/LED.
 */
void Example2_UpdateProgress(void)
{
    QuickTuneState state = QuickTune_GetState();

    if (state == QUICKTUNE_STATE_MEASURING)
    {
        // Get overall progress (0.0 to 1.0)
        float progress = QuickTune_GetProgress();

        // Get current band being measured (0-9)
        int band = QuickTune_GetCurrentBand();

        printf("Progress: %.0f%% (measuring band %d/10)\n",
               progress * 100.0f, band + 1);

        // Update LED/LCD/GUI
        // LED_SetBrightness((uint8_t)(progress * 255));
        // LCD_UpdateProgressBar(progress);
    }
    else if (state == QUICKTUNE_STATE_COMPUTING)
    {
        printf("Computing correction gains...\n");
    }
    else if (state == QUICKTUNE_STATE_APPLYING)
    {
        printf("Applying correction to EQ10...\n");
    }
    else if (state == QUICKTUNE_STATE_DONE)
    {
        printf("Calibration complete!\n");
    }
}

/* ============================================================================
 * EXAMPLE 3: Preset Management
 * ============================================================================ */

/**
 * @brief Save calibration to flash memory
 *
 * @param gains Array of 10 correction gains (dB)
 * @return true if saved successfully
 */
bool Example3_SavePreset(const float* gains)
{
    // Example: save to flash at address 0x08080000
    // In real implementation, use STM32 HAL flash functions

    printf("Saving preset to flash:\n");
    for (int i = 0; i < 10; i++)
    {
        printf("  Band %d: %+.2f dB\n", i + 1, gains[i]);
    }

    // Flash_Write(0x08080000, gains, 10 * sizeof(float));

    return true;
}

/**
 * @brief Load calibration from flash memory
 *
 * @return true if loaded successfully
 */
bool Example3_LoadPreset(void)
{
    float gains[10];

    // Example: read from flash
    // Flash_Read(0x08080000, gains, 10 * sizeof(float));

    // Simulate loaded preset
    gains[0] = -2.5f;  // 25 Hz
    gains[1] = -1.8f;  // 40 Hz
    gains[2] = 0.5f;   // 63 Hz
    gains[3] = 1.2f;   // 100 Hz
    gains[4] = -0.3f;  // 160 Hz
    gains[5] = -1.5f;  // 250 Hz
    gains[6] = 0.8f;   // 400 Hz
    gains[7] = 0.2f;   // 630 Hz
    gains[8] = -0.5f;  // 1000 Hz
    gains[9] = 0.0f;   // 1600 Hz

    printf("Loading preset from flash:\n");
    for (int i = 0; i < 10; i++)
    {
        printf("  Band %d: %+.2f dB\n", i + 1, gains[i]);
    }

    // Apply gains to EQ10
    QuickTune_ApplyGains(gains);

    return true;
}

/* ============================================================================
 * EXAMPLE 4: Error Handling
 * ============================================================================ */

/**
 * @brief Check for errors and handle appropriately
 */
void Example4_CheckErrors(void)
{
    int error = QuickTune_GetLastError();

    if (error != 0)
    {
        printf("QuickTune error: ");

        switch (error)
        {
            case 1:
                printf("Invalid state transition (already calibrating?)\n");
                break;

            case 2:
                printf("Buffer overflow (check sample rate/block size)\n");
                break;

            case 3:
                printf("Invalid parameters\n");
                break;

            default:
                printf("Unknown error code %d\n", error);
                break;
        }

        // Handle error (e.g., reset, notify user, log)
        // LED_SetColor(RED);
        // UART_SendString("QuickTune error\n");
    }
}

/* ============================================================================
 * EXAMPLE 5: CPU Usage Monitoring
 * ============================================================================ */

/**
 * @brief Monitor CPU usage
 *
 * Call this periodically to verify system is within budget.
 */
void Example5_MonitorCpuUsage(void)
{
    float cpu = QuickTune_GetCpuUsage();

    printf("QuickTune CPU usage: %.2f%%\n", cpu);

    // During calibration: ~0.13% (tone + Goertzel)
    // Post-calibration: ~3.8% (EQ10 processing)

    if (cpu > 10.0f)
    {
        printf("WARNING: CPU usage higher than expected!\n");
    }
}

/* ============================================================================
 * EXAMPLE 6: Complete Application Flow
 * ============================================================================ */

/**
 * @brief Complete application example
 *
 * This shows a typical workflow from startup to calibration to normal operation.
 */
void Example6_CompleteApplication(void)
{
    printf("\n=== QuickTune Complete Application Example ===\n\n");

    // Step 1: Initialize at startup
    printf("Step 1: Initializing...\n");
    QuickTune_Init();

    // Step 2: Try to load saved preset from flash
    printf("\nStep 2: Loading saved preset...\n");
    if (Example3_LoadPreset())
    {
        printf("Preset loaded successfully\n");
    }
    else
    {
        printf("No saved preset, will use flat response\n");
    }

    // Step 3: User presses "Calibrate" button
    printf("\nStep 3: User presses calibrate button...\n");
    if (QuickTune_Start())
    {
        printf("Calibration started\n");
    }

    // Step 4: Audio loop runs (simulated)
    printf("\nStep 4: Running audio loop during calibration...\n");
    float micInput[32];
    float speakerOutput[32];

    // Simulate audio loop (in real system, this runs in interrupt)
    int loops = 0;
    while (QuickTune_GetState() != QUICKTUNE_STATE_DONE && loops < 10000)
    {
        // Simulate microphone input (in real system, from ADC)
        for (int i = 0; i < 32; i++)
        {
            micInput[i] = 0.0f;  // Would be actual mic data
        }

        // Process block
        Example1_AudioCallback(micInput, speakerOutput);

        // Update progress every 100 loops (~67 ms @ 48 kHz)
        if (loops % 100 == 0)
        {
            Example2_UpdateProgress();
        }

        loops++;
    }

    // Step 5: Calibration complete
    printf("\nStep 5: Calibration complete!\n");
    const float* gains = QuickTune_GetCorrectionGains();
    if (gains != NULL)
    {
        // Save to flash
        Example3_SavePreset(gains);
    }

    // Step 6: Normal operation
    printf("\nStep 6: Normal operation (EQ10 active)\n");
    printf("Audio processing continues with room correction applied\n");

    // Check diagnostics
    Example4_CheckErrors();
    Example5_MonitorCpuUsage();

    printf("\n=== Example Complete ===\n");
}

/* ============================================================================
 * EXAMPLE 7: Multi-Room Presets
 * ============================================================================ */

#define NUM_PRESETS 3

/**
 * @brief Preset storage structure
 */
typedef struct
{
    char name[32];
    float gains[10];
} CalibrationPreset;

static CalibrationPreset s_presets[NUM_PRESETS] = {
    {
        "Studio",
        {-2.5f, -1.8f, 0.5f, 1.2f, -0.3f, -1.5f, 0.8f, 0.2f, -0.5f, 0.0f}
    },
    {
        "Living Room",
        {-5.0f, -3.2f, -1.5f, 0.8f, 1.2f, 0.5f, 0.0f, -0.3f, -0.8f, -1.0f}
    },
    {
        "Bedroom",
        {-1.0f, -0.5f, 0.2f, 0.8f, 0.3f, -0.5f, -1.2f, -0.8f, 0.0f, 0.5f}
    }
};

/**
 * @brief Load preset by index
 *
 * @param index Preset index (0 to NUM_PRESETS-1)
 * @return true if loaded successfully
 */
bool Example7_LoadPresetByIndex(int index)
{
    if (index < 0 || index >= NUM_PRESETS)
    {
        printf("Invalid preset index: %d\n", index);
        return false;
    }

    printf("Loading preset: %s\n", s_presets[index].name);

    QuickTune_ApplyGains(s_presets[index].gains);

    return true;
}

/**
 * @brief Save current calibration as preset
 *
 * @param index Preset index (0 to NUM_PRESETS-1)
 * @param name Preset name
 * @return true if saved successfully
 */
bool Example7_SaveAsPreset(int index, const char* name)
{
    if (index < 0 || index >= NUM_PRESETS)
    {
        printf("Invalid preset index: %d\n", index);
        return false;
    }

    const float* gains = QuickTune_GetCorrectionGains();
    if (gains == NULL)
    {
        printf("No calibration data available\n");
        return false;
    }

    // Copy name
    snprintf(s_presets[index].name, sizeof(s_presets[index].name), "%s", name);

    // Copy gains
    for (int i = 0; i < 10; i++)
    {
        s_presets[index].gains[i] = gains[i];
    }

    printf("Saved preset %d: %s\n", index, name);

    return true;
}

/* ============================================================================
 * EXAMPLE 8: Real-Time Audio Processing
 * ============================================================================ */

/**
 * @brief Main audio processing loop
 *
 * This shows how to integrate QuickTune with your existing audio processing.
 */
void Example8_RealTimeProcessing(float* input, float* output, int numSamples)
{
    // Temporary buffers
    static float micBuffer[32];
    static float speakerBuffer[32];

    // Copy input to mic buffer
    for (int i = 0; i < numSamples; i++)
    {
        micBuffer[i] = input[i];
    }

    // During calibration, QuickTune controls the output
    if (QuickTune_GetState() == QUICKTUNE_STATE_MEASURING)
    {
        // QuickTune generates tones and analyzes mic input
        QuickTune_ProcessBlock(micBuffer, speakerBuffer, numSamples);

        // Copy to output
        for (int i = 0; i < numSamples; i++)
        {
            output[i] = speakerBuffer[i];
        }
    }
    else
    {
        // Normal audio processing with EQ10 correction

        // 1. Apply input gain/processing
        for (int i = 0; i < numSamples; i++)
        {
            output[i] = input[i] * 1.0f;  // Unity gain
        }

        // 2. Apply EQ10 (room correction)
        EQ10_ProcessBlock(output, output, numSamples);

        // 3. Apply output limiter/volume
        for (int i = 0; i < numSamples; i++)
        {
            output[i] *= 0.8f;  // Volume control
        }
    }
}

/* ============================================================================
 * MAIN (for testing on desktop)
 * ============================================================================ */

#ifdef QUICKTUNE_EXAMPLE_STANDALONE

/**
 * @brief Main entry point (for desktop testing)
 */
int main(void)
{
    printf("QuickTune Integration Examples\n");
    printf("==============================\n\n");

    // Run complete application example
    Example6_CompleteApplication();

    return 0;
}

#endif /* QUICKTUNE_EXAMPLE_STANDALONE */
