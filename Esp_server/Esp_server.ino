#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <math.h>
#include <Wire.h>
#include <iostream>
#include <cstdlib>
//const char* ssid = "TP-LINK_37F3";
//const char* password = "70748109";
const char* ssid = "FunBox3-A702-1";
const char* password = "cdn1234567";
//const char* ssid = "MY ASUS";
//const char* password = "arlenaAnaleena";
ESP8266WebServer server(80);
char y[5];
char e[10];
double Tem;
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     13 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#include "iron.h"

void handleRoot() {
  String message;
  char temp[400];
     snprintf(temp, 400,

           "<html>\
  <head>\
    <meta http-equiv='refresh' content='10'/>\
    <title>Termo demo</title>\
    <style>\
      body { background-color: #000000; font-size: 60%; font-family: Arial, Helvetica, Sans-Serif; Color: #00FF00; text-align:center; }\
    </style>\
  </head>\
  <body>\
     <h1>Termometr</h1>\
    <h2>Temperatura: \
     </body>\
</html>"
          );
          message += temp;
          message += Tem;
          message += " &#176;";
          message += "C";
          
  server.send(200, "text/html", message);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup() {
  Serial.begin(9600);
  Wire.begin(4,5); 
   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.drawBitmap(0, 0, iron, 128, 32, 1);
  display.display();
  delay(2000); 

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  int vee = 0;
  while (WiFi.status() != WL_CONNECTED && vee<=20) {
    delay(500);
    Serial.print(".");
    vee++;
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  if (MDNS.begin("Termometr")) {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);
  server.on("/inline", []() {
    server.send(200, "text/plain", "Co ty tutaj robisz");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  pinMode(2, OUTPUT);}

void loop() {
    server.handleClient();

    digitalWrite(2, HIGH);
    
    if(Serial.available() > 0) {
      Serial.readBytes(e, 4);
      //Serial.println(e);
      digitalWrite(2, LOW);
      double w = atoi(e);
      Serial.println(w);

      double OutIn = ((w/1024)*100);
      //Serial.print(OutIn);
      //Serial.println(" %");
      if(OutIn>=15 && OutIn<33){
        Tem = -0.0127*OutIn*OutIn + 1.7135*OutIn-35.003;}
      else if(OutIn<15){ 
        Tem = 0.0229*OutIn*OutIn*OutIn - 0.7435*OutIn*OutIn + 9.430*OutIn - 63;}
      else if(OutIn>=33 && OutIn<55){
        Tem = 0.9046*OutIn-22.2;}
      else if(OutIn>=55 && OutIn<90){
        Tem = 0.0011*OutIn*OutIn*OutIn-0.2123*OutIn*OutIn+14.609*OutIn-317.4;}
      else if(OutIn<90){   
        Tem = 0.5079*OutIn*OutIn*OutIn -143.41*OutIn*OutIn+ 13501*OutIn - 423693;}

      Serial.println("Temperatura: ");
      Serial.print(Tem);
      Serial.println(" C");
      //Serial.println(x);
      //Serial.println(w);
      delay(100);
      //digitalWrite(2, HIGH);
      
      display.clearDisplay();      
      display.setCursor(30,0);
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.println("Temperatura:");
      display.setTextSize(2);
      display.setCursor(24,10);
      display.print(Tem);
      display.println(" C");
      display.setCursor(28,25);
      display.setTextSize(1);
      display.println(WiFi.localIP());
      display.display();
    }
}
