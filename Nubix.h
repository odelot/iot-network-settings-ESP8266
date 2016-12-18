#ifndef __NUBIX_H_
#define __NUBIX_H_

#include "Flash.h"
#include <WiFiManager.h>

#define DEFAULT_FLASH_OFFSET 0
#define DEFAULT_CONNECTION_TIMEOUT 30000 //30 seconds

#define DEBUG_NUBIX_WM(...) os_printf( __VA_ARGS__ )

#ifndef DEBUG_NUBIX_WM
#define DEBUG_NUBIX_WM(...)
#define NODEBUG_NUBIX_WM
#endif


class Nubix {

public:
  Nubix (WiFiManager *wifiManager) : flash (DEFAULT_FLASH_OFFSET), connTimeout (DEFAULT_CONNECTION_TIMEOUT), _wifiManager(wifiManager) {};
  Nubix (WiFiManager *wifiManager, int eeprom_offset) : flash(eeprom_offset), connTimeout (DEFAULT_CONNECTION_TIMEOUT), _wifiManager(wifiManager) {};

  const static byte NUBIX_NOT_CONFIGURED = 1;
  const static byte NUBIX_CONNECTING = 2;
  const static byte NUBIX_VALID = 3;
  const static byte NUBIX_CONNECTED = 4;
 
  void setup ();
  void loop ();
  void reset ();
  byte getState ();
  void setConnectionTimeout (long connectionTimeout);
  
  void  setOnStateChange( void (*func)(byte,byte) );
  void  setGenerateSSID ( String (*func)() );
    

protected:  
  void changeState (byte _old, byte _new);
  void setState (byte state);
  String getServerSSID ();    
  void initConnectionState ();
  bool waitConnection (long timeout);
  
  Flash flash;
  byte state;
  long connTimeout;

  void (*_onStateChange)(byte,byte) = NULL;
  String (*_generateSSID)() = NULL;  
 
  WiFiManager *_wifiManager;

};

#endif
