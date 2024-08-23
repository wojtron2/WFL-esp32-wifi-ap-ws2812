/*
 WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA2 encryption. For insecure
 WEP or WPA, change the Wifi.begin() call and use Wifi.setMinSecurity() accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 5

 created for arduino 25 Nov 2012
 by Tom Igoe

ported for sparkfun esp32 
31.01.2017 by Jan Hendrik Berlin
 
 */

#include <WiFi.h>
#include <ESPmDNS.h>
#include "Freenove_WS2812_Lib_for_ESP32.h"
#include "luxdef_strobo.h"
//#include "globalvars.h"


#define LEDS_COUNT 18
#define LEDS_PIN 27
#define CHANNEL 0
#define STROBO_PIN 25

//const char* ssid     = "esp32apnewnew23";
const char* ssid     = "ESP32_wojtron";
const char* password = "12345678";  //mozna dac NULL
const int   channel        = 10;                        // WiFi Channel number between 1 and 13
const bool  hide_SSID      = false;                     // To disable SSID broadcast -> SSID will not appear in a basic WiFi scan
const int   max_connection = 2;                         // Maximum simultaneous connected clients on the AP

Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);

IPAddress local_ip(192,168,0,1);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

WiFiServer server(80);

void setup()
{
    Serial.begin(115200);

    strip.begin();
    strip.setBrightness(100);

    pinMode(25, OUTPUT);      // set the STROBO pin mode

    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    WiFi.softAP(ssid, password, channel, hide_SSID, max_connection);
    //WiFi.begin(ssid, password);

    /*
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    */

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    if (!MDNS.begin("esp32")) {
        Serial.println("Error setting up MDNS responder!");
        while(1) {
            delay(1000);
        }
    }

    server.begin();
    MDNS.addService("http", "tcp", 80);
    setAll(5, 0, 10);

}

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">ON</a> to turn the LED on pin 27 ON.<br>");
            client.print("Click <a href=\"/L\">OFF</a> to turn the LED on pin 27 OFF.<br>");
            client.print("Click <a href=\"/purple\">PURPLE</a> to turn the LED on pin 27 PURPLE.<br>");
            client.print("Click <a href=\"/red\">RED</a> to turn the LED on pin 27 RED.<br>");
            client.print("Click <a href=\"/blue\">BLUE</a> to turn the LED on pin 27 BLUE.<br>");
            client.print("Click <a href=\"/green\">GREEN</a> to turn the LED on pin 27 GREEN.<br><br>");

            client.print("Click <a href=\"/brightness100\">BRIGHTNESS 100%</a> to set BRIGHTNESS 100.<br>");
            client.print("Click <a href=\"/brightness50\">BRIGHTNESS 50%</a> to set BRIGHTNESS 50.<br>");
            client.print("Click <a href=\"/brightness25\">BRIGHTNESS 25%</a> to set BRIGHTNESS 25.<br>");
            client.print("Click <a href=\"/brightness15\">BRIGHTNESS 15%</a> to set BRIGHTNESS 15.<br>");
            client.print("Click <a href=\"/brightness5\">BRIGHTNESS 5%</a> to set BRIGHTNESS 5.<br>");
            client.print("Click <a href=\"/brightness0\">BRIGHTNESS 0%</a> to set BRIGHTNESS 0.<br><br><br>");


            client.print("Click <a href=\"/strobo1on50off50loops2\">STROBO1 50 ON 50 OFF 2 SEC</a>turn on STROBO1 50 ON 50 OFF 2 SEC <br>");
            client.print("Click <a href=\"/strobo1on100off100loops2\">STROBO1 100 ON 100 OFF 2 SEC</a>turn on STROBO1 100 ON 100 OFF 2 SEC <br>");
            client.print("Click <a href=\"/strobo_light_on\">STROBO LIGHT ON</a><br>");
            client.print("Click <a href=\"/strobo_light_of\">STROBO LIGHT OFF</a><br>");
            

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        if (currentLine.endsWith("GET /purple")) {
          setAll(150, 0, 255);
        }
        if (currentLine.endsWith("GET /red")) {
          setAll(255, 0, 0);
        }
        if (currentLine.endsWith("GET /blue")) {
          setAll(0, 0, 255);
        }
        if (currentLine.endsWith("GET /green")) {
          setAll(0, 255, 0);
        }
        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          //digitalWrite(5, HIGH);               // GET /H turns the LED on
          setAll(255, 255, 255);
        }
        if (currentLine.endsWith("GET /L")) {
          //digitalWrite(5, LOW);                // GET /L turns the LED off
          setAll(0, 0, 0);
        }
        if (currentLine.endsWith("GET /brightness100")) strip.setBrightness(100);
        if (currentLine.endsWith("GET /brightness50")) strip.setBrightness(50);
        if (currentLine.endsWith("GET /brightness25")) strip.setBrightness(25);
        if (currentLine.endsWith("GET /brightness15")) strip.setBrightness(15);
        if (currentLine.endsWith("GET /brightness5")) strip.setBrightness(5);
        if (currentLine.endsWith("GET /brightness0")) strip.setBrightness(0);
        if (currentLine.endsWith("GET /strobo1on50off50loops2")) strobo1(50, 50, 2);

        if (currentLine.endsWith("GET /strobo1on100off100loops2")) strobo1(100, 100, 1);
        if (currentLine.endsWith("GET /strobo_light_on")) digitalWrite(STROBO_PIN, HIGH);
        if (currentLine.endsWith("GET /strobo_light_off")) digitalWrite(STROBO_PIN, LOW);
        

      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}


void setPixel(int Pixel, byte red, byte green, byte blue) {

  strip.set_pixel(Pixel, green, red, blue);
  // ledstrip.setPixelColor(Pixel, ledstrip.Color(red, green, blue));
  // ledstrip.setPixelColor(Pixel, ledstrip.Color(red, green, blue));
}

// Set all LEDs to a given color and apply it (visible)
void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < LEDS_COUNT; i++ ) {
    setPixel(i, red, green, blue);
  }
 strip.show();
}
