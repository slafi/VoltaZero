#ifndef CONFIG_H
#define CONFIG_H

/*
 *  Project: Volta_Zero
 *  Creation date: 24/12/2018
 *  Version: 1.0.0 
 *  Last update: 22/09/2019
 *  Author: S. Lafi
 *  License: MIT -- No Warranties
 *

/* Both channels are functional. Only one channel should be enabled.
 * For deployment, the "Google Cloud IoT Core" MUST be active in this code.
 */
//#define CHANNEL_NAME "Google Cloud IoT Core"        // Use the Google Cloud IoT Core channel
#define CHANNEL_NAME "Helium MQTT"                    // Use Helium MQTT channel

// Debugging flags
#define DEBUG       1                  // If DEBUG is enabled, debug information is printed to the serial interface
#define SPEECHLESS  0                  // If SPEECHLESS, the buzzer is intentionnally muted (the sent buzzer status flag is not)
#define MAX_SUBMISSION_ATTEMPTS 5      // Maximum number of data submission failures before autoreset

/* The time period in milliseconds required for each data reading and sending / reset delay */
#define DEVICE_DELAY    15000          // Telemetry interval: 15 seconds
#define RESET_DELAY     60000          // Reset delay: 60 seconds

/* Sensor Thresholds :: Alter these values as required */
#define MAX_TEMPERATURE        30.0        // High temperature threshold (Buzzer ON above this threshold)
#define MIN_TEMPERATURE        0.0         // Low temperature threshold (Buzzer ON below this threshold)
#define DARKNESS_THRESHOLD     2.5         // The darkness threshold :: Light is maximum when the light sensor voltage is close to 0V (Buzzer ON below this threshold)

#endif
