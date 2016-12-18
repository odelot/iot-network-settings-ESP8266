#include "Flash.h"


Flash::Flash (int offset) 
{
  _offset = offset;
}


//preparando a memória flash
//(usamos 97 posições)
void Flash::init () {
    EEPROM.begin(_offset+98);    
    DEBUG_FLASH("\nFlash::init");
    
    if (readState () <= 0 || readState () > 4) {
      DEBUG_FLASH("\nresetting flash");
      //estado invalido - vamos resetar tudo-- TUDO!!!
      EEPROM.write (_offset,1); //estado inicial 1
      EEPROM.write (_offset + 1,0); //tipo de segurança. no mock eh a WPA2 
      
      //gravando 32 espaçoes para o SSID
      for (int i=0; i< 32; i+=1) {
         EEPROM.write ((_offset + 2) + i,(byte)'\0');
      }
      //gravando 63 espaçoes para a senha do wifi
      for (int i=0; i< 63; i+=1) {
         EEPROM.write ( (_offset + 34 ) +i,(byte)'\0');
    }
    EEPROM.commit ();
  }
  
}



void Flash::setSSID (char* ssid) {
	//EEPROM.begin(_offset+98); 
  //gravando 32 espaçoes para o SSID
    for (int i=0; i< 32; i+=1) {
       EEPROM.write ((_offset + 2)+i,(byte)ssid[i]);
    }
    EEPROM.commit ();
}

char* Flash::getSSID (char* ssid) {
	//EEPROM.begin(_offset+98); 
  //lendo 32 espaçoes para o SSID
    for (int i=0; i< 32; i+=1) {
       ssid[i] = (char) EEPROM.read ((_offset + 2)+i);
    }
    return ssid;
}

void Flash::setPassword (char* password) {
	//EEPROM.begin(_offset+98); 
  //gravando 63 espaçoes para a senha do wifi
    for (int i=0; i< 63; i+=1) {
       EEPROM.write ((_offset + 34)+i,(byte)password[i]);
    }
    EEPROM.commit ();
}

char*  Flash::getPassword (char* password) {
	//EEPROM.begin(_offset+98); 
	//lendo 63 espaçoes para a senha do wifi
    for (int i=0; i< 63; i+=1) {
       password[i] = (char) EEPROM.read ((_offset + 34)+i);
    }
    return password;
}


/**

Estados
1 - incial
2 - ssid e password pegos
3 - ja se conectou alguma vez (ssid e pass validos)
4 - conectado e pronto para executar rotina
*/
 byte Flash::readState () {
  //EEPROM.begin(_offset+98); 
  byte state = EEPROM.read (_offset);
  DEBUG_FLASH(String (state).c_str());    
  return state;
}

/**
Estados
1 - incial
2 - ssid e password pegos
3 - ja se conectou alguma vez (ssid e pass validos)
4 - conectado e pronto para executar rotina
*/
 void Flash::saveState (byte state) {
  DEBUG_FLASH(String (state).c_str());    
  //EEPROM.begin(_offset+98); 
  EEPROM.write (_offset,state);
  EEPROM.commit ();
}
