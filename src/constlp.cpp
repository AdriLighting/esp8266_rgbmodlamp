#include "../include/constlp.h"

const char ALMLPT_REQ_001[] PROGMEM = "{\"arg\":[\"v\"]}";
const char ALMLPT_REQ_002[] PROGMEM = "{\"arg\":[]}";
const char ALMLPT_REQ_003[] PROGMEM = "{\"arg\":[\"r\",\"g\",\"b\",\"h\",\"s\",\"v\"]}";
const char ALMLPT_REQ_004[] PROGMEM = "{\"arg\":[\"c\",\"v\"]}";
const char ALMLPT_REQ_005[] PROGMEM = "{\"arg\":[\"v\",\"id\"]}";

PROGMEM reqNamIDList _reqNamIDList [] = {
  {req_s_output,          ALMLPT_REQ_001},
  {req_toglle,            ALMLPT_REQ_002},
  {req_lampOnOff,         ALMLPT_REQ_001},
  {req_ledBri,            ALMLPT_REQ_001},
  {req_ledBriIncr,        ALMLPT_REQ_001},
  {req_lampHue,           ALMLPT_REQ_001},
  {req_lampHueIncr,       ALMLPT_REQ_001},
  {req_lampSat,           ALMLPT_REQ_001},
  {req_lampBri,           ALMLPT_REQ_001},
  {req_lampWhite,         ALMLPT_REQ_001},
  {req_lampWhite_f,       ALMLPT_REQ_002},
  {req_lampWhite_v,       ALMLPT_REQ_001},
  {req_lampSetColor,      ALMLPT_REQ_003},
  {req_lampSetColor_f,    ALMLPT_REQ_003},
  {req_etage,             ALMLPT_REQ_001},
  {req_etages,            ALMLPT_REQ_001},
  {req_eff_autoplay,      ALMLPT_REQ_001},
  {req_eff_activ,         ALMLPT_REQ_001},
  {req_eff_next,          ALMLPT_REQ_002},
  {req_eff_prev,          ALMLPT_REQ_002},
  {req_eff_load,          ALMLPT_REQ_001},
  {req_eff_f,             ALMLPT_REQ_004},
  {req_eff_speed,         ALMLPT_REQ_001},
  {req_eff_speedIncr,     ALMLPT_REQ_001},
  {req_eff_bri,           ALMLPT_REQ_001},
  // {req_eff_briIncr,        "{\"arg\":[]}"},
  {req_eff_scale,         ALMLPT_REQ_001},
  {req_eff_id,            ALMLPT_REQ_005},
  {req_eff_timerCol,      ALMLPT_REQ_001},
  // {req_eff_timerCol2,      "{\"arg\":[]}"},
  {req_eff_timerPal,      ALMLPT_REQ_001},
  {req_eff_timerPalGrad,  ALMLPT_REQ_001},
  {req_eff_effColorMod,   ALMLPT_REQ_001}
};

uint8_t _reqNamIDList_cnt = ARRAY_SIZE(_reqNamIDList);

void reqNamIDList_json(const String & search, DynamicJsonDocument & doc) { 
  JsonObject obj ;  
  
  if (!doc.containsKey(F("requestnameid"))){
    obj = doc.createNestedObject(F("requestnameid"));
  } else {
    obj = doc[F("requestnameid")];
  }

  for (uint8_t i = 0; i < _reqNamIDList_cnt; i++){
    if (search == FPSTR(_reqNamIDList[i]._nameId)) {
      JsonObject obj_2 = obj.createNestedObject(F("arg_search"));
      obj_2[FPSTR(ALMLPT_REQ)] = search;
      JsonArray arr = obj_2.createNestedArray(FPSTR(ALMLPT_ARG));

          DynamicJsonDocument arg(255);
          deserializeJson(arg, FPSTR(_reqNamIDList[i]._arg));

          JsonArray argArr = arg[F("arg")].as<JsonArray>();
          if (argArr.size()<=0)continue;
          for (String item : argArr) {
            if (!item)continue;
            arr.add(item);
          }
    }
    
  } 
}
void reqNamIDList_json(uint8_t mod, DynamicJsonDocument & doc) {
  JsonArray arr;

  JsonObject obj ;  
  if (!doc.containsKey(F("requestnameid"))){
    obj = doc.createNestedObject(F("requestnameid"));
  } else {
    obj = doc[F("requestnameid")];
  }

  switch (mod) {
      case 0:
        arr = obj.createNestedArray(F("list"));
        for (uint8_t i = 0; i < _reqNamIDList_cnt; i++){
          arr.add(FPSTR(_reqNamIDList[i]._nameId));
        }
      break;
      case 1:
        arr = obj.createNestedArray(F("listarg"));
        for (uint8_t i = 0; i < _reqNamIDList_cnt; i++){
          JsonArray arr_2 = arr.createNestedArray();

          DynamicJsonDocument arg(255);
          deserializeJson(arg, FPSTR(_reqNamIDList[i]._arg));

          arr_2.add(FPSTR(_reqNamIDList[i]._nameId));
          JsonArray arr_3 = arr_2.createNestedArray();

          JsonArray argArr = arg[F("arg")].as<JsonArray>();
          for (String item : argArr) {
            if (!item)continue;
            Serial.println(item);
            arr_3.add(item);
          }
        }       
      break;
      default:
      break;
  }
}