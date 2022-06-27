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
  Serial.printf_P(PSTR("\n#############\nAdrilighting MultiStrip\n#############\n\n"));

  #ifdef ALML_TFT
    almlTft::begin(3);  
  #endif

  #ifdef FILESYSTEM
    #if defined(ESP8266)
      FILESYSTEM.begin();
      // FILESYSTEM.format();
      // FILESYSTEM.remove("/outputs/lamp.txt");
      // al_tools::SPIFFS_deleteRecursive(FILESYSTEM, "/eff");
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
  WCEVO_managerPtrGet()->credentials_add("free020FB5_EXT", "phcaadax");
  WCEVO_managerPtrGet()->credentials_add("free3C3786_EXT", "phcaadax");
  #ifdef FILESYSTEM
    // WCEVO_managerPtrGet()->credentials_from_fs();  
  #endif
  WCEVO_managerPtrGet()->start();
  // WCEVO_managerPtrGet()->print();

  #ifdef DEBUG_KEYBOARD
    _Sr_menu.add("alml_api", "y", []() { keyboard_print(); });
    _Sr_menu.add("alml_getter", "!", [](const String & v1, const String & v2) {  
      Serial.printf_P(PSTR("CMD: %s - VAL: %s\n"), v1.c_str(), v2.c_str());
      keyboard_getter(v1);    
    }, SR_MM::SRMM_KEYVAL);  
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


  JsonArray exclued = DeviceUserConfig.createNestedArray("list_lb_exclued");
  exclued.add("Armoire");
  // exclued.add("Pride");
  // exclued.add("Syncro");
  JsonObject  segment       = DeviceUserConfig.createNestedObject("segment");
  segment[F("op")] = 0;
  JsonArray   segmentArray  = segment.createNestedArray("items");
  JsonObject  segment_t     = segmentArray.createNestedObject();
  segment_t[F("s")] = 0;
  segment_t[F("e")] = 14;
  segment_t[F("d")] = false;
  segment_t = segmentArray.createNestedObject();
  segment_t[F("s")] = 15;
  segment_t[F("e")] = 29;
  segment_t[F("d")] = true;

  new Device(ADS_NAME, ADS_OC);

}

void loop() {
  DevicePtrGet()->loop();
}
