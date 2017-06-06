// include the SevenSegmentTM1637 library
#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"

#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>

const char *ssid     = "you_wifi_ssid";
const char *password = "you_wifi_password";

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

void setup_serial() {
  Serial.begin(115200);         // initializes the Serial connection @ 115200 baud
}

void setup_display() {
  display.begin();            // initializes the display
  display.setBacklight(100);  // set the brightness to 100 %
}


void setup_wifi() {
  if (WiFi.status() == WL_CONNECTED)
    return;
    
  Serial.println(""); // a empty line
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(""); // a empty line
  Serial.println("WiFi connected");
  Serial.println("IP address : ");
  Serial.println(WiFi.localIP());
}


// run setup code
void setup() {
  setup_serial();
  setup_wifi();

  timeClient.begin(); // NTP begin
  setup_display();
 
  delay(1000);                // wait 1000 ms
}


void set_display(byte hours, byte minutes) {
  display.printTime(hours, minutes, true);  // display time
}

// run loop (forever)
void loop() {
  setup_wifi(); // if wifi disconnected, connect again
  
  String time_s = timeClient.getFormattedTime(); // get current time
  timeClient.update(); // udpate ntp
  Serial.println(time_s); // print current time
  
  //getHours(); getMinutes(); also work in NTPClient..
  byte hours    =  time_s.substring(0,2).toInt(); // initialize hours
  byte minutes  = time_s.substring(3,5).toInt();  // initialize minutes
  set_display(hours, minutes); // set content of TM1637 display
  delay(1000); // delay 1second
};
