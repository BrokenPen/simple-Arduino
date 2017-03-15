// include the SevenSegmentTM1637 library
#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"

#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>

const char *ssid     = "your_ssid";
const char *password = "your_password";

WiFiUDP ntpUDP;

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
//3600*8 = UTF+8
NTPClient timeClient(ntpUDP, "hk.pool.ntp.org", 3600*8, 60000);

/* initialize global TM1637 Display object
*  The constructor takes two arguments, the number of the clock pin and the digital output pin:
* SevenSegmentTM1637(byte pinCLK, byte pinDIO);
*/
const byte PIN_CLK = 16; //D0   // define CLK pin (any digital pin)
const byte PIN_DIO = 5; //D1  // define DIO pin (any digital pin)
SevenSegmentExtended      display(PIN_CLK, PIN_DIO);

// run setup code
void setup() {
  WiFi.begin(ssid, password);
  Serial.begin(115200);         // initializes the Serial connection @ 9600 baud
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
  
  display.begin();            // initializes the display
  display.setBacklight(100);  // set the brightness to 100 %
  delay(1000);                // wait 1000 ms
};

// run loop (forever)
void loop() {
  String time_s = timeClient.getFormattedTime();
  timeClient.update();
  Serial.println(time_s); 
  
  byte hours    =  time_s.substring(0,2).toInt();                     // initialize hours
  byte minutes  = time_s.substring(3,5).toInt();                   // initialize minutes
  display.printTime(hours, minutes, true);  // display time
  delay(1000);
};
