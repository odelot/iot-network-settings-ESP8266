#include "Nubix.h"


void  Nubix::setOnStateChange( void (*func)(byte,byte) ) {
	_onStateChange = func;
	
}

void  Nubix::setGenerateSSID ( String (*func)() ){
	_generateSSID = func;
}

//creates a random ssid with nubix prefix
String Nubix::getServerSSID () {
  String ret="";
  if (_generateSSID!=NULL) {
	  ret +=  _generateSSID ();
  } else {
	  String ssid = "nubix_";
	  randomSeed(analogRead(A0));
	  for (int i= 0; i< 5; i+=1) {
		  int r = random (0,10);
		  ssid+= r;
	  }
	  ret += ssid;
  }
  DEBUG_NUBIX_WM (ret.c_str());
  return ret;
    
}

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}

//config esp to STA mode and try to connect to wifi with the storaged info
void Nubix::initConnectionState () {
  char ssid[32];
  char password[63];
  flash.getSSID (ssid);
  flash.getPassword (password);
  WiFi.disconnect();
  delay(100);
  WiFi.mode(WIFI_STA);  
  WiFi.begin(ssid, password);
  delay(500);
}

void Nubix::reset () {	
	DEBUG_NUBIX_WM("\nNubix::reset");
	flash.saveState (0);
	flash.init ();
	setState (Nubix::NUBIX_NOT_CONFIGURED); 	
	WiFi.mode(WIFI_AP);  
	WiFi.disconnect();
}


//initialize nubix
void Nubix::setup () {  
  WiFi.disconnect();
  flash.init ();
  DEBUG_NUBIX_WM("\nNubix::setup");  
  byte memState = flash.readState ();
  state = Nubix::NUBIX_NOT_CONFIGURED;
  changeState (memState,state);
  
}

void Nubix::loop () {
  byte memState = flash.readState ();
  byte state = getState ();
  if (state == Nubix::NUBIX_NOT_CONFIGURED) {    
    if (memState == Nubix::NUBIX_NOT_CONFIGURED) {
	  yield ();
      if (_wifiManager->startConfigPortal(getServerSSID ().c_str()) == true) {        
		setState (Nubix::NUBIX_CONNECTING);		
		flash.setSSID ((char*)WiFi.SSID().c_str());
		flash.setPassword((char*)WiFi.psk().c_str());		   
        //flash.saveState (Nubix::NUBIX_CONNECTING);
      }
    } else {
      setState (Nubix::NUBIX_CONNECTING);      
    }
  } else 
  if (state == Nubix::NUBIX_CONNECTING) {
    initConnectionState ();
    if (waitConnection (connTimeout)) {
      byte currentState = flash.readState ();
      if (currentState == Nubix::NUBIX_NOT_CONFIGURED) {
        flash.saveState (Nubix::NUBIX_VALID);
      }
      setState (Nubix::NUBIX_VALID);      
    } else {
      //não se conectou
      byte currentState = flash.readState ();
      if (currentState == Nubix::NUBIX_NOT_CONFIGURED) {
        //never connected... back to initial state
        setState (Nubix::NUBIX_NOT_CONFIGURED);        
      }
	  //wait to try again
	  delay (500);      
    }    
  } else if (state == Nubix::NUBIX_VALID) {
    setState (Nubix::NUBIX_CONNECTED);
  } else if (state == Nubix::NUBIX_CONNECTED) {
    if (WiFi.status() != WL_CONNECTED) {
      setState (Nubix::NUBIX_CONNECTING);     
    } 
  }
  yield ();

  
  
}

bool Nubix::waitConnection (long timeout) {
  long init = millis ();
  //Log::d(String("WC"));  
  while (WiFi.status() != WL_CONNECTED && ((millis () - init) < timeout)) {
    delay (500);
    //Log::d(String("."));
  }
  if (WiFi.status() == WL_CONNECTED)
    return true;
  else
    return false;

}

//change connection timeout
void Nubix::setConnectionTimeout (long connectionTimeout){
  connTimeout = connectionTimeout;
}

byte Nubix::getState () {
  return state;
}



void Nubix::changeState (byte _old, byte _new) {
  if (_onStateChange != NULL) 
	  _onStateChange (_old,_new);
}

void Nubix::setState (byte _state) {
  if (state != _state) {
    changeState (state,_state);    
  }
  state = _state;
}


