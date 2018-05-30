#include <SDISerial.h>
String VWC;   //volumetric water content
String EC;    // electrical conductivity
String TEMP;  // temperature
char X='+';
char *FTEvalue=new char[20];
/*
I used a few devices with no problem using a MEGA 2560 as well as an UNO.
Sketch was built with Arduino 1.0.4, however I also tested it under 1.0.0
Place the SDISerial folder in    "<ARDUINO_ROOT>/libraries"

with the 5TE 
the WHITE wire is power. 
  - I hooked it up to the arduino 5v output pin, however you could also connect it to a pin and drive power only when you wanted it
the RED wire is the DATA_LINE.
  - you must hook it up to a pin that can process interrupts (see link below)
  
the remaining wire must be connected to ground
*/

//in order to recieve data you must choose a pin that supports interupts
#define DATALINE_PIN 3
#define INVERTED 1
//see:   http://arduino.cc/en/Reference/attachInterrupt
//for pins that support interupts (2 or 3 typically)


SDISerial sdi_serial_connection(DATALINE_PIN, INVERTED);

char* get_measurement(){
  char* service_request = sdi_serial_connection.sdi_query("?M!",150);
  //you can use the time returned above to wait for the service_request_complete
  char* service_request_complete = sdi_serial_connection.wait_for_response(150);
  //dont worry about waiting too long it will return once it gets a response
  return sdi_serial_connection.sdi_query("?D0!",150);
}

void setup(){
 sdi_serial_connection.begin(); // start our SDI connection 
 Serial.begin(9600); // start our uart
 Serial.println("OK INITIALIZED"); // startup string echo'd to our uart
 Serial.println("VWCvalue      EC       TEMP");
 delay(3000); // startup delay to allow sensor to powerup and output its DDI serial string
}


void loop(){

 char* response = get_measurement(); // get measurement data
 String FTE=response;
 char tmp1='+';  
 int tmp2=FTE.indexOf(tmp1);
 while(tmp2!=1)
 {
 char* response = get_measurement(); // get measurement data
 FTE=response;
 tmp2=FTE.indexOf(tmp1);
 }
 int FTElength=FTE.length();

 FTE.toCharArray(FTEvalue,FTElength);
     int plustest=FTEvalue[5];
     if (plustest=43)
       {
        VWC= String(FTEvalue[2])+String(FTEvalue[3])+String(FTEvalue[4]);
        EC= String(FTEvalue[6])+String(FTEvalue[7])+String(FTEvalue[8])+String(FTEvalue[9]);
        TEMP= String(FTEvalue[11])+String(FTEvalue[12])+String(FTEvalue[13])+String(FTEvalue[14]);
       }
    else
       {
        VWC= String(FTEvalue[2])+String(FTEvalue[3])+String(FTEvalue[4])+String(FTEvalue[5]);
        EC= String(FTEvalue[7])+String(FTEvalue[8])+String(FTEvalue[9])+String(FTEvalue[10]);
        TEMP= String(FTEvalue[12])+String(FTEvalue[13])+String(FTEvalue[14])+String(FTEvalue[15]);
       }
    float VWCvalue=VWC.toInt()/100;
    Serial.print(VWCvalue);
    Serial.print("         ");
    Serial.print(EC);  
    Serial.print("       ");
    Serial.println(TEMP);
    delay(2000);
}
