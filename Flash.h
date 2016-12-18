#ifndef __FLASH_H_
#define __FLASH_H_

#include <Arduino.h>
#include <Stream.h>
#include <EEPROM.h>

//#define DEBUG_FLASH(...) os_printf( __VA_ARGS__ )

#ifndef DEBUG_FLASH
#define DEBUG_FLASH(...)
#define NODEBUG_FLASH
#endif


class Flash {
  
public:
  Flash (int offset);
  
  void init ();
  void setSSID (char* ssid);
  char* getSSID (char* ssid);
  void setPassword (char* password);
  char*  getPassword (char* password);
  byte readState ();
  void saveState (byte state);

private :
  int _offset;
  
};

#endif
