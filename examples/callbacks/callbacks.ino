//callbacks.ino
//Generates custom SSID and handle the status changes
//(using callbacks)

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

//generate SSID using the mac address
String getSSID () {
   String ssid = "";
   byte mac[6];  
   WiFi.macAddress(mac);
   for (int i=5; i>0; i-=1) {
     ssid += String (mac[i],HEX);
     ssid += ":";
   }
   ssid += String (mac[0],HEX);
   return ssid;
}

//use it to give some feedback to the user (like blink led)
void stateChange (byte oldState, byte newState) {
  if (newState == Nubix::NUBIX_NOT_CONFIGURED) {
    //do something when it is not connected
  }
  if (newState == Nubix::NUBIX_CONNECTING) {
    //do something when it is trying to connected
  }
  if (newState == Nubix::NUBIX_CONNECTED) {
    //do something when it is connected
  }
}

//use it to reset the network credentials
void reset () {
  nubix.reset ();
}

void setup() {
  Serial.begin (115200);
  //Serial.setDebugOutput(1);
  wifiManager.setDebugOutput (false);
  nubix.setGenerateSSID (getSSID);
  nubix.setOnStateChange (stateChange);
  nubix.setup ();
}

void loop() {
  nubix.loop ();
  if (nubix.getState() == Nubix::NUBIX_CONNECTED) {
     //do your stuff here
  }
}