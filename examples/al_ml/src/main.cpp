/*
 *
DEBUG_ALMLREMOTE
DEBUG_WEBSERVER
DEBUG_TASK


*/

#include <almllib.h>
void setup() {
  Serial.begin(115200);

  for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime > 5000); ) { }
  delay(1000);

  Serial.println();
  Serial.println("STARTUP");

  #ifdef FILESYSTEM
    #if defined(ESP8266)
      FILESYSTEM.begin();
      // FILESYSTEM.format();
      // FILESYSTEM.remove("/outputs/lamp.txt");
      // deleteRecursive(FILESYSTEM, "/eff");
      // deleteRecursive(FILESYSTEM, "/outputs");
    #elif defined(ESP32)
      FILESYSTEM.begin(true);
    #endif
  #endif
  
  #ifdef ALT_DEBUG_TARCE
    ALT_debugBuffer = new char[1024];  
    _DebugPrintList.add("main");  
    #ifdef DEBUG_ALMLREMOTE
      _DebugPrintList.add(ALML_DEBUGREGION_REMOTE);    
    #endif
    #ifdef DEBUG_TASK
      _DebugPrintList.add(ALML_DEBUGREGION_TASK);    
    #endif
    #ifdef DEBUG_WEBSERVER
      _DebugPrintList.add(ALML_DEBUGREGION_WEBSERVER);  
    #endif  
    _DebugPrintList.add(ALML_DEBUGREGION_EFFECT);  
    _DebugPrintList.add(WCEVO_DEBUGREGION_WCEVO);  
    _DebugPrintList.add(WCEVO_DEBUGREGION_AP);  
    _DebugPrintList.add(WCEVO_DEBUGREGION_STA);  
  #endif 

  WCEVO_managerPtrGet()->set_cm(WCEVO_CM_STAAP);
  WCEVO_managerPtrGet()->set_cmFail(WCEVO_CF_NEXTAP);
  WCEVO_managerPtrGet()->credentials_add("SSID_1", "PSK_1");
  WCEVO_managerPtrGet()->credentials_add("SSID_2", "PSK_2");
  #ifdef FILESYSTEM
    // WCEVO_managerPtrGet()->credentials_from_fs();  
  #endif
  WCEVO_managerPtrGet()->start();
  WCEVO_managerPtrGet()->print();

  #ifdef DEBUG_KEYBOARD
    // _Sr_menu.add("api setterr", "|", [](const String & v1, const String & v2) {  
    //  Serial.printf("CMD: %s - VAL: %s\n", v1.c_str(), v2.c_str());
    //  // keyboard_getter(v1);    
    // }, SR_MM::SRMM_KEYVAL);  

  _Sr_menu.add("wifi_api", "r", []() { WCEVO_managerPtrGet()->keyboard_print(); });
  _Sr_menu.add("wifi_getter", "@", [](const String & v1, const String & v2) {  
    Serial.printf("CMD: %s - VAL: %s\n", v1.c_str(), v2.c_str());
    WCEVO_managerPtrGet()->keyboard_getter(v1);    
  }, SR_MM::SRMM_KEYVAL);    
  #endif 

  #ifdef ESP8266
    WiFi.setPhyMode(WIFI_PHY_MODE_11N);
  #endif  
  WiFi.disconnect(false);
  WiFi.mode(WIFI_STA);

  new Device(ADS_NAME, ADS_OC);

  al_tools::SPIFFS_PRINT("/", false, false); 
}

void loop() {
  DevicePtrGet()->loop();
}
