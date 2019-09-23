/*
 *  Project: Volta_Zero
 *  Creation date: 24/12/2018
 *  Version: 1.0.0 
 *  Last update: 22/09/2019
 *  Author: S. Lafi
 *  License: MIT -- No Warranties
 *
 * IMPORTANT: The device id is unique and is the sole identifier of the device in the database 
 * Please, change the device_id for each device as following:
 * 
 *   DEVICE_ID       | NAME          |  MAC ADDRESS              |
 * --------------------------------------------------------------|  
 *   101             | VoltaAtom_4   |  6081f9fffe0020e0         |
 *   102             | VoltaAtom_3   |  6081f9fffe0021ba         |
 *   103             | VoltaAtom_2   |  6081f9fffe0008ee         |
 *   104             | VoltaAtom_1   |  6081f9fffe00245f         |
 * 
 * All additional devices should respect this format. 
 * Notice: In this version, device IDs are hardcoded. So they should be compiled in the device code
 * and enetred in the database carefully (and they should match). 
 * Sensor Types are: A, B, C, D, etc.
 * Sensor Type A definition: 
 * 
 * 1- Internal Sensors:   
 * Analog Ch 0: On board temperature sensor  
 * Analog Ch 1: Off board thermo-couple interface (k-type or J-type)
 * Analog Ch 2: Light Sensor 60kOhm@10Lux  
 */

#include "Arduino.h"
#include "Board.h"
#include "Helium.h"

#include "config.h"
#include "utils.h"
#include "sensor.h"

void(* resetFunc) (void) = 0;       // declare reset function @ address 0

/* INPUTS / OUTPUTS */
const int buzzer_pin = 2;           // buzzer attached to Arduino pin 2 (digital)
const int onboard_ts_pin = A0;      // onboard temperature sensor input attached to pin 0 (analog)
const int temp_k_type_pin = A1;     // external temperature sensor input attached to pin 1(analog)
const int lightsensor_pin = A2;     // lightsensor input attached to pin 2 (analog)


/* INITIALIZATIONS */
int buzzerStatus = 0;               // Buzzer status
int lsStatus = 0;                   // Light sensor status
float temperature = 0.0;            // Onboard temperature value
float temperature_k_type = 0.0;     // External temperature value 
float lsSensitivity = 0.0;          // Lihgt sensor voltage value
int attempts = 0;                   // Number of failed submission attempts before automatic reset

/* DEVICE ID */
const int device_id = 102;          // Change the device id as given above

/* MAIN CODE STARTS HERE */
Helium  helium(&atom_serial);
Channel channel(&helium);

/* Report Helium message status */
void report_status(int status)
{
    if (helium_status_OK == status)
    {
        Serial.println("Succeeded");
    }
    else
    {
        Serial.println("Failed");
    }
}

/* Report message status result */
void report_status_result(int status, int result)
{
    if (helium_status_OK == status)
    {
        if (result == 0)
        {
            Serial.println("Sending: Succeeded");
            blink(1);

            // Re-init attempts
            attempts = 0;
        }
        else {
            Serial.print("Sending: Failed - ");
            Serial.println(result);
            blink(0);

            // Record failed communication attempts
            attempts++;
        }
    }
    else
    {
        Serial.println("Failed");
         blink(0);
         
        // Record failed communication attempts
        attempts++;
    }
}


void setup() {
  
    Serial.begin(9600);
    Serial.println("Starting");

    // Begin communication with the Helium Atom
    // The baud rate differs per supported board
    // and is configured in Board.h
    helium.begin(HELIUM_BAUD_RATE);

    // Connect the Atom to the Helium Network
    Serial.print("Connecting - ");
    int status = helium.connect();
    
    // Print status
    report_status(status);

    // Begin communicating with the channel. 
    // This should only need to be done once.
    int8_t result;
    Serial.print("Creating Channel - ");
    status = channel.begin(CHANNEL_NAME, &result);
    
    // Print status and result
    report_status_result(status, result);
    
    pinMode(buzzer_pin, OUTPUT);          // Set buzzer - digital pin 2 as an output
    pinMode(lightsensor_pin, INPUT);      // Set lightsensor - analog pin 2 as an input
    pinMode(onboard_ts_pin, INPUT);       // Set onboard temperature sensor - analog pin 0 as an input
    pinMode(temp_k_type_pin, INPUT);      // Set external temperature sensor - analog pin 1 as an input

    // Setup Arduino LED for status reporting
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

    int8_t result;

#if DEBUG
    Serial.println("-------------------------------"); 
#endif
    
    /* Get temperature value from A0 */
    temperature  = getOnboardTemperatureValue(onboard_ts_pin);

#if DEBUG
    Serial.print("Temperature (t0) = "); 
    Serial.print(temperature);
    Serial.println(" degC"); 

    //float temperature_k_typeF = temperature_k_type * 9 / 5 + 32;                  // conversion to temp Fahrenheit
#endif

    /* Get temperature value from A1 */
    temperature_k_type = getExternalTemperatureValue(temp_k_type_pin);

#if DEBUG
    Serial.print("Temperature k-type (t1) = "); 
    Serial.print(temperature_k_type);
    Serial.println(" degC");
#endif

    lsSensitivity = getLightSensorValue(lightsensor_pin);

#if SPEECHLESS
    Serial.println("NOTICE: Buzzer output is deactivated. Re-program relevant flag to reactivate it.");
#endif

    lsStatus = 0;
    buzzerStatus = 0;
    
    /* Check temperature and light sensitivity thresholds to set light sensor
     * and buzzer status flags
     */
    if(temperature <= MIN_TEMPERATURE || temperature >= MAX_TEMPERATURE)
    {
        buzzerStatus = 1;        
        
#if !SPEECHLESS
        tone(buzzer_pin, 2000);       // Send 2kHz sound signal
        delay(1000);                  // for 1 sec then       
        noTone(buzzer_pin);           // Stop sound
#endif
    }
    
    if(lsSensitivity >= DARKNESS_THRESHOLD)  // Check if ambiant light level is high
    {
        buzzerStatus = 1;
        lsStatus = 1;

#if !SPEECHLESS
        for(int i = 0; i < 5; i++)
        {
          tone(buzzer_pin, 3000, 200);        // Send 3kHz sound signal
          delay(300);                     // for 0.3 sec then        
          noTone(buzzer_pin);                 // Stop sound
        }
#endif
    }    
    
    // Format cloud message in JSON format then send it to Helium gateway
    char* buffer = formatMessage(device_id, temperature, temperature_k_type, lsSensitivity, buzzerStatus);
    int   status = channel.send(buffer, strlen(buffer), &result);

#if DEBUG
    Serial.println(buffer);
#endif
    
    // Print status and result
    report_status_result(status, result);
    
    free(buffer);

#if DEBUG
    checkMemory();
#endif

    delay(DEVICE_DELAY);

#if DEBUG
    Serial.flush();
#endif

    /* After 5 failed communication attempts, invoke automatic MCU reset to attempt recovering the connection */
    if(attempts == MAX_SUBMISSION_ATTEMPTS)
    {
#if DEBUG
      Serial.print("Automatic reset sequence in progress . . .");
#endif
      delay(RESET_DELAY);
      resetFunc();
    }
}
