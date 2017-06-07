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

#include <SimpleDHT.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 





// for DHT11, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT11 = D0;
SimpleDHT11 dht11;

/* Set these to your desired credentials. */
const char *ssid = "ESPap";
const char *password = "blessed1";

const uint16_t port = 80;
const char * host = "192.168.4.1"; // ip or dns

String temp = "";
String humi = "";

unsigned int lastSend = 0;

void setupWIFI() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);                   // STA mode , very improtant - -
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  delay(10);            // a little delay

  setupWIFI();          // We start by connecting to a WiFi network
}

void http() {
Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/dht11";
  url += "?temp=";
  url += temp;
  url += "&humi=";
  url += humi;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}  


void loop() {
  
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  if (dht11.read(pinDHT11, &temperature, &humidity, NULL)) {
    Serial.print("Read DHT11 failed.");
    return;
  }
  


  temp = String((int)temperature);
  humi = String((int)humidity);
  
  // DHT11 sampling rate is 1HZ.
  delay(1000);                        // read dht11 value every second

  
  if ( millis() - lastSend > 5000 ) { // send dht11 value to host every 5 second
    // start working...
    Serial.println("=================================");
    Serial.println("Sample DHT11...");
  
    Serial.print("Sample OK: ");
    Serial.print((int)temperature); Serial.print(" *C, "); 
    Serial.print((int)humidity); Serial.println(" %");
    http();
    lastSend = millis();              // update lastSend
  }
}
