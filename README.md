# iot-network-settings-ESP8266

Library built for ESP8266 (with the Arduino SDK - https://github.com/esp8266/Arduino) that provides a convenient way to set up your IoT device based on ESP8266 with credentials to connect to the wireless router and access Internet.

You can create your own way to send the credential to this library (see customization item below), but there is an android app ready to do this job for you (click on image):
<a href='https://play.google.com/store/apps/details?id=br.com.nubix.cordova.wifisettings&utm_source=global_co&utm_medium=prtnr&utm_content=Mar2515&utm_campaign=PartBadge&pcampaignid=MKT-Other-global-all-co-prtnr-py-PartBadge-Mar2515-1'><img alt='Get it on Google Play' src='https://play.google.com/intl/en_us/badges/images/generic/en_badge_web_generic.png' width="200px"/></a>

## ChangeLog

* 0.1 - Commit Home

## Motivation

When you are developing an IoT wireless device (with ESP8266 for example) it needs to be configured with the necessary information (network SSID and password, for example) to access the wifi network that contains internet.

I suggest a simplified way to make this setting through the smartphone. It's simple:

1. download the ESP8266 library

2. download the Android app *
<a href='https://play.google.com/store/apps/details?id=br.com.nubix.cordova.wifisettings&utm_source=global_co&utm_medium=prtnr&utm_content=Mar2515&utm_campaign=PartBadge&pcampaignid=MKT-Other-global-all-co-prtnr-py-PartBadge-Mar2515-1'><img alt='Get it on Google Play' src='https://play.google.com/intl/en_us/badges/images/generic/en_badge_web_generic.png' width="200px"/></a>

3. focus on programming the main purpose of your device 

\* You can crete our own Android app to config the device using the source code (https://github.com/profFabio/iot-network-settings-cordova) of the ones in the play store  

## Installation

1. Set the Arduino IDE to compile and to upload programs to the ESP8266 (Arduino sdk for ESP8266 - details https://github.com/esp8266/Arduino)
2. Install the iot-network-settings-ESP8266 library as an Arduino library

## Limitations

* The wifi data is received without encryption
* Handle access to wifi networks with WPA2, WPA and WEP security. It does not handle open networks or those that use certificates or login pages.

## States Machine

See the image to understand how the library works.

! [States] (https://github.com/odelot/iot-network-settings-ESP8266/blob/master/img/states.png)

 * Reset the library (reset the state to NUBIX_NOT_CONFIGURED
 
 ```nubix.reset ()```

You may observe that after the esp8266 connects successfully to a router, it records the NUBIX_VALID state on the EEPROM. After this moment evertime you turns the device on it will try to connect with the credentials recorded in the EEPROM (as it has worked once). To erase this info and comeback to the start state (NUBIX_NOT_CONFIGURED) you need to reset the library. You may link the reset to some push button input (if the device turns on with the button pressed, and it stay pressed by 30 seconds, you reset the library) 

## EEPROM

The library uses 97 bytes in the EEPROM memory. You can set an offset to write from a position that is not in use by your application. By default, the starting address is 0.

| (offset) + Position  | Description                                             |
|----------------------|-------------------------------------------------------- |
| 0                    | state (state machine)                                   |
| 1                    | sort of wifi connection security (not used yet)         |
| 2-33                 | SSID (received via wifi - at max 32 positions.)         |
| 34-96                | password wifi (received via wifi - at max 63 positions) |

## Use

### Simplest Use

See example "simplest-sample":

```
#include <Flash.h>
#include <Log.h>
#include <Nubix.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>

Nubix nubix;

void setup () {
  Serial.begin (9600);
  nubix.setup ();
}

void loop () {
  nubix.loop ();
  if (nubix.getState () == :: Nubix NUBIX_CONNECTED) {
    Serial.println ( "do your stuff");
  }
}
```

Use the Android application to send the WiFi network settings that the device should connect.

Once the library to get the credentials and connect, you can run your code (the part "do your stuff").

Watch the video showing this procedure: (soon)

### Suggested Use

See example "suggested-sample".

In this example you'll see that you can control the library and handle some events, as states changes.

### Customization

* Send credentials without using the application android

You do not necessarily need to use the Android application to send the settings. You can connect to the device when it is in NUBIX_NOT_CONFIGURED state, open a TCP socket on the server ip (192.168.4.1 by default) using the port defined in the library (default port: 9402) and send a string to the following pattern (change SSID and PASS values by what you want to send):
```
NI:ssid,pass
```

* Create my own android application to send configuration

The android application source code, made in Cordova, is available at: https://github.com/profFabio/iot-network-settings-cordova
You can customizes it or absorb it into your cordova application.
