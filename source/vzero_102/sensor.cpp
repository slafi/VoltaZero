
#include "sensor.h"

/* Gets the temperature value from the internal temperature sensor (t0) */
float getOnboardTemperatureValue(const int onboard_ts_pin)
{
  float temp = 0.0;
  
  // Read from A0
  float Vin = 5.0 * analogRead(onboard_ts_pin) / 1024.00;  

#if DEBUG
  Serial.print("Volts (T0)= "); 
  Serial.println(Vin); 
#endif
      
  temp =  (Vin - 0.424) / 0.00625;
  return temp;
}

/* Gets the temperature value from the internal temperature k-type sensor (t1) */
float getExternalTemperatureValue(const int temp_k_type_pin)
{
  float temp1 = 0.0;
  
  // Read from A1
  float Vin1 = 5.0 * analogRead(temp_k_type_pin) / 1024.00;  

#if DEBUG
  Serial.print("Volts (T1)= "); 
  Serial.println(Vin1); 
#endif
           
  temp1 =  (Vin1 - 0.424) / 0.00625;
  return temp1;
}

/* Returns the voltage value of the light sensor */
float getLightSensorValue(const int lightsensor_pin)
{
  // Read from A2
  float sens = 5.0 * analogRead(lightsensor_pin) / 1024.00;  

#if DEBUG
  Serial.print("Volts (LS)= "); 
  Serial.println(sens); 
#endif
               
  return sens;
}

/* Activates the L LED of Arduino Uno according to the status of the Helium message */
void blink(int mode) {
  
  if(mode)    // Error mode: blink frequency = 5 Hz
  {
    for(int i = 0; i < 5; i++)
    {
        digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(200);                       // wait for a second  
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    }
  }
  else    // Normal mode: blink frequency = 1 Hz
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second  
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    //delay(1000);                     // wait for a second
  }  
}
