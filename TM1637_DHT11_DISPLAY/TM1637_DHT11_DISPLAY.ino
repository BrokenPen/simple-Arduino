/*
	Basic usage example

	Demonstrated some of the basic functionality of the library. Initialize the display, set the backlight brightness, print some text, count from 0 to 100 and print on display and blink some text.

  Note: make sure to set your serial monitor to line end: NEW LINE!

	The circuit:
  * connect TM1637 pin CLK to Arduino pin D4
  * connect TM1637 pin DIO to Arduino pin D5
  * connect TM1637 pin Vcc to Arduino pin 5V
  * connect TM1637 pin GND to Arduino pin GND

	Created 25 September 2015
	By Bram Harmsen

	https://github.com/bremme/arduino-tm1637

*/

// nodemcu pin reference to esp8266 gpio
#define D0 16
#define D1 5   // I2C Bus SCL (clock)
#define D2 4   // I2C Bus SDA (data)
#define D3 0
#define D4 2   // Same as "LED_BUILTIN", but inverted logic
#define D5 14 // SPI Bus SCK (clock)
#define D6 12 // SPI Bus MISO 
#define D7 13 // SPI Bus MOSI
#define D8 15 // SPI Bus SS (CS)
#define D9 3  // RX0 (Serial console)
#define D10 1 // TX0 (Serial console)

// include the SevenSegmentTM1637 library
#include "SevenSegmentTM1637.h"

// include the esp8266 wifi library
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

#define TEMP 0
#define HUMI 1

/* initialize global TM1637 Display object
*  The constructor takes two arguments, the number of the clock pin and the digital output pin:
* SevenSegmentTM1637(byte pinCLK, byte pinDIO);
*/
const byte PIN_CLK = D0;   // define CLK pin (any digital pin)
const byte PIN_DIO = D1;   // define DIO pin (any digital pin)
SevenSegmentTM1637    display(PIN_CLK, PIN_DIO);

ESP8266WebServer server(80);

/* Set these to your desired credentials. */
const char *ssid = "ESPap";
const char *password = "blessed1";

String temp = "0";
String humi = "0";

int turn = TEMP;


unsigned int lastUpdate = 0;


void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void handle_dht11() {
  
    server.sendHeader("Connection", "close");
    for (uint8_t i = 0; i < server.args(); i++) {
      if (server.argName(i) == "temp")
        temp = server.arg(i);
      if (server.argName(i) == "humi")
        humi = server.arg(i);
    }

    String message = "{\"temp\":\"" + temp + "\",\"humi\":\""+humi+"\"}";
    server.send(200, "text/html", message);
    Serial.println(message);
  
}

void serverEvent(){
  server.on("/", handleRoot);      //http://192.168.4.1
  server.on("/dht11",handle_dht11);     //http://192.168.4.1/dht11?temp=12&humi=55
}

void setupWebserver() {
  serverEvent();
  server.begin();
}

void setupWIFI() {
 Serial.println();
 Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}

void updateTM1637() {
   display.clear();            // clear the display
   
   String temp_str = "C-";     // temperatuere display prefix
   String humi_str = "H-";     // humiday display prefix
   if (turn == TEMP) {
     temp_str.concat(temp);    // prepare string
     display.print(temp_str);  // display
     turn = HUMI;              // next update -> HUMI
   } else if (turn == HUMI){
     humi_str.concat(humi);    // prepare string
     display.print(humi_str);  // display
     turn = TEMP;              // next update -> TEMP
   }
}


// run setup code
void setup() {
  delay(1000);
  Serial.begin(115200);
  display.begin();            // initializes the display

  setupWIFI();                // initializes wifi ap
  
  display.setBacklight(100);  // set the brightness to 100 %
  display.print("INIT");      // display INIT on the display
  delay(1000);                // wait 1000 ms

  setupWebserver();            // start web server
};



// run loop (forever)
void loop() {
   if(  millis() - lastUpdate > 2500 ) { // each 2.5 seconds 
      updateTM1637();                    // update TM1637 content
      lastUpdate = millis();             // record udpate time
   }
   
   server.handleClient();
};
