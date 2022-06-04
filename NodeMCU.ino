/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>

#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);       // (RX, TX)
const int ledPin = 2;
String ledState;               // Stores LED state



// Replace with your network credentials
const char* ssid     = "WiFi_SSID";
const char* password = "WiFi_Password";


AsyncWebServer server(80);     // Create AsyncWebServer object on port 80
int data;



const char* paramEffectID = "EffectID";
const char* paramBrightness = "Brightness";

const char* paramHue = "Hue";
const char* paramSat = "Sat";
const char* paramBri = "Bri";



void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}



void setup(){
  // Serial port for debugging purposes
  s.begin(9600);   
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);    

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());


  // Route to load index.html file
  server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/index.html", String());
  });

  
  // Route to load main.css file
  server.on("/main.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/main.css", "text/css");
  });


  // Route to load logo.png file
  server.on("/icon.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/icon.png", "image/png");
  });


  // Route to load iro.js file
  server.on("/iro.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/iro.js", "text/javascript");
  });

  
  

  // Send a POST request
  server.on("/ledstate", HTTP_POST, [] (AsyncWebServerRequest *request) {
      String returnText = "No colors sent";
      if (request->hasParam(paramEffectID, true)) {
          String EffectID = request->getParam(paramEffectID, true)->value();

          returnText = EffectID + ":";
          
          const char* toReturn = returnText.c_str();
          s.write(toReturn);
          Serial.println(toReturn);
      } 
      
      else if (request->hasParam(paramHue, true)) { 
          String hue = request->getParam(paramHue, true)->value();
          String sat = request->getParam(paramSat, true)->value();
          String bri = request->getParam(paramBri, true)->value();


          returnText = hue + "," + sat + "," + bri;
          
          const char* toReturn = returnText.c_str();
          s.write(toReturn);
          Serial.println(toReturn);
      }
      
      else if (request->hasParam(paramBrightness, true)) { 
          String brightness = request->getParam(paramBrightness, true)->value();


          returnText = brightness + ";";
          
          const char* toReturn = returnText.c_str();
          s.write(toReturn);
          Serial.println(toReturn);
      }

      
      request->send(200, "text/plain", returnText);
  });


  // Start server
  server.begin();
}



 
void loop(){

  if (s.available() > 0) {
    data = s.read();
    Serial.println(data);
  }
  
}
















  // Send a GET request
  /*server.on("/ledstate", HTTP_GET, [] (AsyncWebServerRequest *request) {
      String message;
      if (request->hasParam(paramEffectID)) {
          message = request->getParam(paramEffectID)->value();
      } else {
          message = "No message sent";
      }
      request->send(200, "text/plain", message);
      Serial.println(message);
  });*/
