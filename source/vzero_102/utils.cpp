
#include "utils.h"


/* This function formats the data sent to the cloud as a JSON string. 
 * ONLY onboard temperature, the light sensor voltage and buzzer status flag are accounted for.
 * The other sensors are intentionally deactivated (value null)
 *    {
 *       "id": "101",       // DEVICE ID
 *       "t0": 15.0,        // INTERNAL TEMPERATURE SENSOR VALUE
 *       "t1": "null",      // EXTERNAL TEMPERATURE SENSOR VALUE
 *       "th": "null",      // THERMOCOUPLE SENSOR VALUE
 *       "ir": "null",      // INFRARED SENSOR STATUS FLAG
 *       "bz": 0,           // BUZZER SENSOR STATUS FLAG
 *       "lg": 0            // LIGHT SENSOR STATUS FLAG
 *    }
 */
char* formatMessage(int id, float temp0,float temp1, int sensStat, int bzStatus)
{
    char str_id[4] = {};
    char str_temp0[8] = {};
    char str_temp1[8] = {};
    char str_sensStat[2] = {};
    char str_bzStatus[2] = {};
    
    itoa(id, str_id, 10);

    dtostrf(temp0, 4, 3, str_temp0);
    dtostrf(temp1, 4, 3, str_temp1);
    
    itoa(sensStat, str_sensStat, 10); //itoa(integer, buffer, base)
    itoa(bzStatus, str_bzStatus, 10); 
    
    // char *strcat(char *dest, const char *src)
    char* data = strdup("");
    const char* prefix = "{\"id\":\"";
    
    data = strcat(data, prefix);
    data = strcat(data, str_id); 
    data = strcat(data, "\",\"t0\":");
    data = strcat(data, str_temp0);
    
    // Intentionally not reporting t1 for now
    //data = strcat(data, str_temp1);
    data = strcat(data, ",\"t1\":\"null\",\"th\":\"null\",\"ir\":\"null\",\"bz\":");
    data = strcat(data, str_bzStatus);
    data = strcat(data, ",\"lg\":");
    data = strcat(data, str_sensStat);
    data = strcat(data, "}");

    return data;
}

char* formatMessage(int id, float temp0,float temp1, double sensitivity, int bzStatus)
{
    char str_id[4] = {};
    char str_temp0[8] = {};
    char str_temp1[8] = {};
    char str_sens[8] = {};
    char str_bzStatus[2] = {};
    
    itoa(id, str_id, 10);

    dtostrf(temp0, 4, 3, str_temp0);
    dtostrf(temp1, 4, 3, str_temp1);
    dtostrf(sensitivity, 4, 3, str_sens);
    
    itoa(bzStatus, str_bzStatus, 10);  //itoa(integer, buffer, base)
    
    // char *strcat(char *dest, const char *src)
    char* data = strdup("");
    const char* prefix = "{\"id\":\"";
    
    data = strcat(data, prefix);
    data = strcat(data, str_id); 
    data = strcat(data, "\",\"t0\":");
    data = strcat(data, str_temp0);
    
    // Intentionally not reporting t1 for now
    //data = strcat(data, str_temp1);
    data = strcat(data, ",\"t1\":\"null\",\"th\":\"null\",\"ir\":\"null\",\"bz\":");
    data = strcat(data, str_bzStatus);
    data = strcat(data, ",\"lg\":");
    data = strcat(data, str_sens);
    data = strcat(data, "}");

    return data;
}

/* Prints the available memory size to the serial output */
void checkMemory() {

  Serial.print("Free Memory (Bytes) = ");
  Serial.println(freeMemory());
}
