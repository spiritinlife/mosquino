#include <ChipCap2.h>
#include <Wire.h>

/*
Example for ChipCap2 library for Mosquino/Arduino

*/

// First create an instance of the library. Doing so with no arguments uses the ChipCap2's factory default I2C address of 0x28.
// You can optionally specify an alternative address if your sensor does not use the default for some reason, e.g.:
// ChipCap2 cc = ChipCap2(0x42); // use alternate address
ChipCap2 cc = ChipCap2();

void setup()
{
  float humidity;
  float temperature;
  int hum_int;
  int temp_int;
  
  Serial.begin(9600);
  
  // If desired, verify the sensor is present on the bus and responding to I2C commands
  if(!cc.present())
  {
    Serial.print("Warning: no ChipCap2 found\r\n");
  }
  
  while(1)
  {
    // The chip is asleep by default; a new measurement must be explicitly triggered to read the new values.
    // The update takes approximately 50msec.
    cc.triggerMeasurement();
    
    // Values are available with fractional degree/percent resolution
    humidity = cc.readHumidity();
    temperature = cc.readTemperature();
    
    // If only integer precision is required, these alternate integer read functions are available.
    // This will avoid pulling in software floating point support if not needed in your sketch.
    hum_int = cc.readHumidity_int();
    temp_int = cc.readTemperature_int();

    Serial.print("Temp=");
    Serial.print(temperature);
    Serial.print(" ... humidity=");
    Serial.println(humidity);
    Serial.print("Int Temp=");
    Serial.print(temp_int);
    Serial.print(" ... humidity=");
    Serial.println(hum_int);
    
    // Note: Frequent reads will result in slight self-heating of the sensor and thus reduce the accuracy.
    // For most applications a longer interval between readings is recommended.
    delay(1000);
  }
}

void loop()
{
  
}

