#include <dht.h>
#include <SFE_BMP180.h>
#include <Wire.h>

#define DHT11_PIN 7
 

dht DHT; 
SFE_BMP180 pressure;

#define ALTITUDE 1309.0 // Altitude ofHanksville, UT. in meters (according to google)

void setup()
{
  Serial.begin(9600);
  Serial.println("REBOOT");

  // Initialize the sensor

  if (pressure.begin())
    Serial.println("init success");
  else
  {
    Serial.println("init fail\n\n");
    while(1); // Pause forever.
  }
}

void loop()
{
  char status;
  double T,P,p0,a;

  // Loop here getting pressure readings

  // If you want sea-level-compensated pressure, as used in weather reports,
  // you will need to know the altitude at which your measurements are taken.
  // We're using a constant called ALTITUDE in this sketch:
  
  Serial.println();
 Serial.print("provided altitude: ");
  Serial.print(ALTITUDE,0);
  Serial.print("; ");
 // Serial.println(" meters ");
  
  // If you want to measure altitude, and not pressure, you will instead need
  // to provide a known baseline pressure. This is shown at the end of the sketch.

  // You must first get a temperature measurement to perform a pressure reading.

  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);

    // Retrieve the completed temperature measurement:
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      Serial.print("temperature: ");
      Serial.print(T,2);
      Serial.print("; ");
      //Serial.println(" deg C");
      
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {

        delay(status);

        // Retrieve the completed pressure measurement:
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
         Serial.print("absolute pressure: ");
          Serial.print(P,2);
          Serial.print("; ");
         // Serial.println(" mb ");

          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

          p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
         Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0,2);
          Serial.print("; ");
          // Serial.println(" mb, ");

          // On the other hand, if you want to determine your altitude from the pressure reading,
          // use the altitude function along with a baseline pressure (sea-level or other).
          // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
          // Result: a = altitude in m.

          a = pressure.altitude(P,p0);
          Serial.print("computed altitude: ");
          Serial.print(a,0);
          //Serial.println(" meters");
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

//humidiy sensor code
 DHT.read11(DHT11_PIN);
    
   Serial.print(" current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("; ");
   // Serial.print("%  ");
  //  Serial.println(" ");

  
  float sensorValue = analogRead(A7);   // read the input on analog pin 0:
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):



 //uv sensor code
 float voltage = (sensorValue/1024*5.0)*1000;  
 // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
 // float voltage = sensorValue * (5.0 / 1023.0);
 
  //Serial.print("UV sensor reading = ");
  //Serial.println(sensorValue);
  Serial.print("sensor voltage = ");
  Serial.print(voltage);
  Serial.print("; ");
  //Serial.println(" mV");
  //Serial.print("\n");


//magnetometer code
      //  int x,y,z; //triple axis data
  float x = analogRead(2); //multiply by 35
  float y = analogRead(1); //gets output in micro teslas
  float z = analogRead(0);
  float reference = analogRead(3);
  float xmag = (x-reference)/204.6;
  float ymag = (y-reference)/204.6;
  float zmag = (z-reference)/204.6;
  
   
  
  //Print out values of each axis
  Serial.print("xmag: ");
  Serial.print(xmag);
  Serial.print("  ymag: ");
  Serial.print(ymag);
  Serial.print("  zmag: ");
  Serial.println(zmag);
  
  delay(2000);
}

