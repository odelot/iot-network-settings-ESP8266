//simplest-sample

#include <Flash.h>
#include <Log.h>
#include <Nubix.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <DNSServer.h>            
#include <ESP8266WebServer.h>    

WiFiManager wifiManager;
Nubix nubix (&wifiManager);

//You can change the eeprom offset 
//int eeprom_offset = 0;
//Nubix nubix(eeprom_offset);

//use it to reset the network credentials
void reset () {
  nubix.reset ();
}

void setup() {
  Serial.begin (115200);
  //Serial.setDebugOutput(1);
  wifiManager.setDebugOutput (false);
  nubix.setup ();
}

void loop() {
  nubix.loop ();
  if (nubix.getState() == Nubix::NUBIX_CONNECTED) {
     //do your stuff here
  }
}