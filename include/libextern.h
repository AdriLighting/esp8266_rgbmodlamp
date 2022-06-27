#ifndef LIBEXTERN_H
  #define LIBEXTERN_H
    #include <Arduino.h>
    #include <ArduinoJson.h>
    #include <Ticker.h>

    #ifdef FILESYSTEM 
      #if defined USE_LITTLEFS
        #if defined(ESP8266)
          #include <LittleFS.h> 
        #elif defined(ESP32)
          #include <FS.h>
          #include <LITTLEFS.h>
        #endif
      #elif defined USE_SPIFFS
        #include <FS.h>
      #endif
    #endif  

  #include "def.h"
  #include "defleds.h"
  #include "constleds.h"
  #include "constlp.h"
  
  #include <arduinoPatternList.h>

  #include <wificonnectevo.h>

  #include <adri_timer.h>

  #include <littlefs_helper.h>


  #include <IRremoteESP8266.h>
  #include <IRrecv.h>
  #include <IRutils.h>


  #include <resizeBuff.h>
  #include <LinkedList.h>

  #if defined(ESP32)
    #include <ESPmDNS.h>
    #include <WiFi.h>
  #elif defined(ESP8266)
    #include <ESP8266WiFi.h>
    #include <ESP8266HTTPClient.h>
  #endif


  #include <WiFiUdp.h>

  #ifdef ALML_TFT
    #include <SD.h>
    #include <SPI.h>
    #if defined(ESP8266)
      #include <Adafruit_ILI9341esp.h>
    #elif defined(ESP32)
      #include <Adafruit_ILI9341.h>
    #else
    #endif   
    #include <Adafruit_I2CDevice.h>
    #include <U8g2_for_Adafruit_GFX.h>
    #include <Adafruit_GFX.h>
    #include <XPT2046.h>

    // #include <adri_tft_ILI9341_driver.h>
    // #include <adri_tft_ILI9341_ui.h>
    // #include <adri_tft_ILI9341_txt.h>
    // #include <adri_tft_ILI9341_extrafunc.h>  
  #endif
#endif // _LIBEXTERN_H
