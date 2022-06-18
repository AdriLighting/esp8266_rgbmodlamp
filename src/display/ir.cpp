
#include "../../include/display/ir.h"
#include "../../include/libextern.h"
#include "../../include/display/lamp.h"
#include "../../include/display/remote.h"
#include "../../include/device/device.h"



// RemoteControl::RemoteControl(){

// }



void RemoteControl::irSend(RemoteControl_ir * ptr, DynamicJsonDocument & doc) {
  RemoteControlIrMod_t irMod = ptr->get_mod();
  switch (irMod) {
      case RemoteControlIrMod_t::RIRMOD_INTERN:
        DevicePtrGet()->parseJson_device(doc);
      break;
      case RemoteControlIrMod_t::RIRMOD_TFT:
      {
        String json;
        doc[FPSTR(ALMLPT_OP)] = 1;
        doc[FPSTR(ALMLPT_DN)] = "almlbeta";  
        serializeJson(doc, json);
        serializeJsonPretty(doc, Serial);Serial.println();
        RemoteControl_udp::send_toIp(json, {8,8,8,8}, 9100);
      } 
      break;
      default:break;
  }
}


void changeEffectByName(RemoteControl_ir * ptr, const char * name){
  // uint8_t cnt = ARRAY_SIZE(T_EFFNAMEID);
  for (uint8_t i = 0; i < _effeNamIDList_cnt; ++i) {
    String find = FPSTR(_effeNamIDList[i]._nameId);
    if (find == al_tools::ch_toString(name)) {
      DynamicJsonDocument doc(500);
      doc[FPSTR(ALMLPT_REQ)]   = FPSTR(req_eff_f);
      JsonObject val = doc.createNestedObject(FPSTR(ALMLPT_VAL));
      val[FPSTR(ALMLPT_V)]     = i;
      val[FPSTR(ALMLPT_C)]     = FPSTR(req_eff_load);
      // DevicePtrGet()->parseJson_device(doc);
      RemoteControlPtrGet()->irSend(ptr, doc);
      return;
    }
  }
}
/*
void changeEffectByName(const char * name){
  // uint8_t cnt = ARRAY_SIZE(T_EFFNAMEID);
  for (uint8_t i = 0; i < _effeNamIDList_cnt; ++i) {
    String find = FPSTR(_effeNamIDList[i]._nameId);
    if (find == al_tools::ch_toString(name)) {
      DynamicJsonDocument doc(500);
      doc[FPSTR(ALMLPT_REQ)]   = FPSTR(req_eff_f);
      JsonObject val = doc.createNestedObject(FPSTR(ALMLPT_VAL));
      val[FPSTR(ALMLPT_V)]     = i;
      val[FPSTR(ALMLPT_C)]     = FPSTR(req_eff_load);
      // DevicePtrGet()->parseJson_device(doc);

      return;
    }
  }
}
*/

void RemoteControl_ir::changeHue(int8_t amount) {
  DynamicJsonDocument doc(1024);
  doc[FPSTR(ALMLPT_REQ)] = "hcIncr";
  JsonObject val = doc.createNestedObject(FPSTR(ALMLPT_VAL));
  val[FPSTR(ALMLPT_V)] = amount;
  // DevicePtrGet()->parseJson_device(doc);
  RemoteControlPtrGet()->irSend(this, doc);
  if(amount > 0) lastRepeatableAction = ACTION_HUE_UP;
  if(amount < 0) lastRepeatableAction = ACTION_HUE_DOWN;
  lastRepeatableValue = amount; 
}


void RemoteControl_ir::changeBrightness(int8_t amount) {
  DynamicJsonDocument doc(255);
  doc[FPSTR(ALMLPT_REQ)]   = FPSTR(req_ledBriIncr);
  JsonObject val = doc.createNestedObject(FPSTR(ALMLPT_VAL));
  val[FPSTR(ALMLPT_V)]     = amount;
  // DevicePtrGet()->parseJson_device(doc);  
  RemoteControlPtrGet()->irSend(this, doc);
  if(amount > 0) lastRepeatableAction = ACTION_BRIGHT_UP;
  if(amount < 0) lastRepeatableAction = ACTION_BRIGHT_DOWN;
  lastRepeatableValue = amount; 
}
void RemoteControl_ir::changeEffSpeed(int8_t amount){
  DynamicJsonDocument doc(255);
  doc[FPSTR(ALMLPT_REQ)]   = FPSTR(req_eff_speedIncr);
  JsonObject val = doc.createNestedObject(FPSTR(ALMLPT_VAL));
  val[FPSTR(ALMLPT_V)]     = amount;
  // DevicePtrGet()->parseJson_device(doc);
  RemoteControlPtrGet()->irSend(this, doc);
  if(amount > 0) lastRepeatableAction = ACTION_SPEED_UP;
  if(amount < 0) lastRepeatableAction = ACTION_SPEED_DOWN;
  lastRepeatableValue = amount;  
}


void changeEffect(RemoteControl_ir * ptr, uint8_t p){
  DynamicJsonDocument doc(255);
  doc[FPSTR(ALMLPT_REQ)]   = FPSTR(req_eff_f);
  JsonObject val = doc.createNestedObject(FPSTR(ALMLPT_VAL));
  val[FPSTR(ALMLPT_V)]     = p;
  val[FPSTR(ALMLPT_C)]     = FPSTR(req_eff_load);
  // DevicePtrGet()->parseJson_device(doc);
  RemoteControlPtrGet()->irSend(ptr, doc);
}

void changeColor(RemoteControl_ir * ptr, uint32_t c){
  DynamicJsonDocument doc(255);
  ledsTools           _ledsTools;
  CRGB                rgb = _ledsTools.rgb32_crgb(c);  
  doc[FPSTR(ALMLPT_REQ)]   = FPSTR(req_lampSetColor_f);
  JsonObject val = doc.createNestedObject(FPSTR(ALMLPT_VAL));
  val[F("r")]     = rgb.r;
  val[F("g")]     = rgb.g;
  val[F("b")]     = rgb.b;
  // DevicePtrGet()->parseJson_device(doc);
  RemoteControlPtrGet()->irSend(ptr, doc);
}

void changeoNoFF(RemoteControl_ir * ptr, uint8_t v){
  DynamicJsonDocument doc(255);
  doc[FPSTR(ALMLPT_REQ)]   = FPSTR(req_lampOnOff);
  JsonObject val = doc.createNestedObject(FPSTR(ALMLPT_VAL));
  val[FPSTR(ALMLPT_V)]     = String(v);
  // DevicePtrGet()->parseJson_device(doc);
  RemoteControlPtrGet()->irSend(ptr, doc);
}

void changeTW(RemoteControl_ir * ptr){
  DynamicJsonDocument doc(255);
  doc[FPSTR(ALMLPT_REQ)] = FPSTR(req_lampWhite_f);
  doc.createNestedObject(FPSTR(ALMLPT_VAL));
  // DevicePtrGet()->parseJson_device(doc);
  RemoteControlPtrGet()->irSend(ptr, doc);
}

void changeEffectNext(RemoteControl_ir * ptr, boolean dir){
  RemoteControlIrMod_t irMod = ptr->get_mod();
  if (irMod==RemoteControlIrMod_t::RIRMOD_TFT){
    DynamicJsonDocument doc(1024);
    doc[FPSTR(ALMLPT_REQ)]  = FPSTR(req_eff_f);
    JsonObject val = doc.createNestedObject(FPSTR(ALMLPT_VAL));
    val[F("c")]           = dir ? FPSTR(req_eff_next) : FPSTR(req_eff_prev); 
    // JsonSend(doc); 
    RemoteControlPtrGet()->irSend(ptr, doc);    
  }
  else if (irMod==RemoteControlIrMod_t::RIRMOD_INTERN){
    if (dir)  EffectslistPtrGet()->eff_next();
    else      EffectslistPtrGet()->eff_prev();   
  }
}
void changeAutoPLay(RemoteControl_ir * ptr){
  RemoteControlIrMod_t irMod = ptr->get_mod();
  if (irMod==RemoteControlIrMod_t::RIRMOD_TFT){
 
  }
  else if (irMod==RemoteControlIrMod_t::RIRMOD_INTERN){
    EffectslistPtrGet()->toggle();  
  }
}

void RemoteControl_ir::decode44(uint32_t code) {
  // boolean result = false;
  switch (code) {
    case IR44_BPLUS       : /*Serial.println("IR44_BPLUS");*/ 			changeBrightness(8); 				    break;
    case IR44_BMINUS      : /*Serial.println("IR44_BMINUS");*/ 			changeBrightness(-8); 				  break;
    case IR44_OFF         : /*Serial.println("IR44_OFF");*/ 				changeoNoFF(this, 1); 								break;
    case IR44_ON          : /*Serial.println("IR44_ON");*/ 					changeoNoFF(this, 0); 								break;
    case IR44_RED         : /*Serial.println("IR44_RED");*/ 				changeColor(this, COLOR_RED); 				break;
    case IR44_REDDISH     : /*Serial.println("IR44_REDDISH");*/ 		changeColor(this, COLOR_REDDISH); 		break;
    case IR44_ORANGE      : /*Serial.println("IR44_ORANGE");*/ 			changeColor(this, COLOR_ORANGE); 			break;
    case IR44_YELLOWISH   : /*Serial.println("IR44_YELLOWISH");*/		changeColor(this, COLOR_YELLOWISH);		break;
    case IR44_YELLOW      : /*Serial.println("IR44_YELLOW");*/ 			changeColor(this, COLOR_YELLOW);			break;
    case IR44_GREEN       : /*Serial.println("IR44_GREEN");*/ 			changeColor(this, COLOR_GREEN);				break;
    case IR44_GREENISH    : /*Serial.println("IR44_GREENISH");*/ 		changeColor(this, COLOR_GREENISH);		break;
    case IR44_TURQUOISE   : /*Serial.println("IR44_TURQUOISE");*/ 	changeColor(this, COLOR_TURQUOISE);		break;
    case IR44_CYAN        : /*Serial.println("IR44_CYAN");*/ 				changeColor(this, COLOR_CYAN); 				break;
    case IR44_AQUA        : /*Serial.println("IR44_AQUA");*/ 				changeColor(this, COLOR_AQUA); 				break;
    case IR44_BLUE        : /*Serial.println("IR44_BLUE");*/				changeColor(this, COLOR_BLUE); 				break;
    case IR44_DEEPBLUE    : /*Serial.println("IR44_DEEPBLUE");*/ 		changeColor(this, COLOR_DEEPBLUE); 		break;
    case IR44_PURPLE      : /*Serial.println("IR44_PURPLE");*/ 			changeColor(this, COLOR_PURPLE); 			break;
    case IR44_MAGENTA     : /*Serial.println("IR44_MAGENTA");*/ 		changeColor(this, COLOR_MAGENTA); 		break;
    case IR44_PINK        : /*Serial.println("IR44_PINK");*/ 				changeColor(this, COLOR_PINK); 				break;
    case IR44_WHITE       : /*Serial.println("IR44_WHITE");*/       changeTW(this);                       break;
    case IR44_WARMWHITE2  : /*Serial.println("IR44_WARMWHITE2");*/ 	break;
    case IR44_WARMWHITE   : /*Serial.println("IR44_WARMWHITE");*/ 	break;
    case IR44_COLDWHITE   : /*Serial.println("IR44_COLDWHITE");*/ 	break;
    case IR44_COLDWHITE2  : /*Serial.println("IR44_COLDWHITE2");*/ 	break;
    case IR44_REDPLUS     : /*Serial.println("IR44_REDPLUS");*/ 		changeEffectNext(this, true);         break;
    case IR44_REDMINUS    : /*Serial.println("IR44_REDMINUS");*/ 		changeEffectNext(this, false);        break;
    case IR44_GREENPLUS   : /*Serial.println("IR44_GREENPLUS");*/   changeHue(4);break;
    case IR44_GREENMINUS  : /*Serial.println("IR44_GREENMINUS");*/  changeHue(-4);break;
    case IR44_BLUEPLUS    : /*Serial.println("IR44_BLUEPLUS");*/ 		break;
    case IR44_BLUEMINUS   : /*Serial.println("IR44_BLUEMINUS");*/ 	break;
    case IR44_QUICK       : /*Serial.println("IR44_QUICK");*/ 			changeEffSpeed(16);  													 break;
    case IR44_SLOW        : /*Serial.println("IR44_SLOW");*/ 				changeEffSpeed(-16); 													 break;
    case IR44_DIY1        : /*Serial.println("IR44_DIY1");*/ 				changeEffectByName(this, TEFF_Colortwinkle); 				 break;
    case IR44_DIY2        : /*Serial.println("IR44_DIY2");*/ 				changeEffectByName(this, TEFF_RainbowCycleSyncro); 	 break;
    case IR44_DIY3        : /*Serial.println("IR44_DIY3");*/ 				changeEffectByName(this, TEFF_Breath); 							 break;
    case IR44_DIY4        : /*Serial.println("IR44_DIY4");*/ 				changeEffectByName(this, TEFF_RainbowSyncro);	 			 break;
    case IR44_DIY5        : /*Serial.println("IR44_DIY5");*/ 				changeEffectByName(this, TEFF_RainbowWave); 				 break;
    case IR44_DIY6        : /*Serial.println("IR44_DIY6");*/ 				changeEffectByName(this, TEFF_Commets); 						 break;
    case IR44_AUTO        : /*Serial.println("IR44_AUTO");*/ 				changeAutoPLay(this);                          break;
    case IR44_FLASH       : /*Serial.println("IR44_FLASH");*/ 			break;
    case IR44_JUMP3       : /*Serial.println("IR44_JUMP3");*/ 			break;
    case IR44_JUMP7       : /*Serial.println("IR44_JUMP7");*/ 			break;
    case IR44_FADE3       : /*Serial.println("IR44_FADE3");*/ 			break;
    case IR44_FADE7       : /*Serial.println("IR44_FADE7");*/ 			break;
    default: return;
  }
  lastValidCode = code;
}

void RemoteControl_ir::decode24(uint32_t code) {
  switch (code) {
    case IR24_BRIGHTER  : changeBrightness(16);               break;
    case IR24_DARKER    : changeBrightness(-16);              break;
    case IR24_OFF       : changeoNoFF(this, 1);                     break;
    case IR24_ON        : changeoNoFF(this, 0);                     break;
    case IR24_RED       : changeColor(this, COLOR_RED);             break;
    case IR24_REDDISH   : changeColor(this, COLOR_REDDISH);         break;
    case IR24_ORANGE    : changeColor(this, COLOR_ORANGE);          break;
    case IR24_YELLOWISH : changeColor(this, COLOR_YELLOWISH);       break;
    case IR24_YELLOW    : changeColor(this, COLOR_YELLOW);          break;
    case IR24_GREEN     : changeColor(this, COLOR_GREEN);           break;
    case IR24_GREENISH  : changeColor(this, COLOR_GREENISH);        break;
    case IR24_TURQUOISE : changeColor(this, COLOR_TURQUOISE);       break;
    case IR24_CYAN      : changeColor(this, COLOR_CYAN);            break;
    case IR24_AQUA      : changeColor(this, COLOR_AQUA);            break;
    case IR24_BLUE      : changeColor(this, COLOR_BLUE);            break;
    case IR24_DEEPBLUE  : changeColor(this, COLOR_DEEPBLUE);        break;
    case IR24_PURPLE    : changeColor(this, COLOR_PURPLE);          break;
    case IR24_MAGENTA   : changeColor(this, COLOR_MAGENTA);         break;
    case IR24_PINK      : changeColor(this, COLOR_PINK);            break;
    case IR24_WHITE     : changeTW(this);                           break;
    // case IR24_WHITE     : changeEffectByName(TEFF_ColorWipe);           break;
    case IR24_FLASH     : changeEffectByName(this, TEFF_Colortwinkle);        break; // 27 EffectFx_mode_colortwinkle
    case IR24_STROBE    : changeEffectByName(this, TEFF_RainbowCycleSyncro);  break; // 15 EffectFx_mode_rainbow_cycle
    case IR24_FADE      : changeEffectByName(this, TEFF_BreathSyncro);        break; // 28 EffectFx_mode_breath
    case IR24_SMOOTH    : changeEffectByName(this, TEFF_RainbowSyncro);       break; // 26 EffectFx_mode_rainbow
    default: return;
  }
  
  lastValidCode = code;
}

void RemoteControl_ir::applyRepeatActions() {
  switch (lastRepeatableAction) {
  case ACTION_BRIGHT_UP :      changeBrightness(lastRepeatableValue);  return;
  case ACTION_BRIGHT_DOWN :    changeBrightness(lastRepeatableValue); return;
  case ACTION_SPEED_UP :       changeEffSpeed(lastRepeatableValue);     return;
  case ACTION_SPEED_DOWN :     changeEffSpeed(lastRepeatableValue);     return;
  // case ACTION_INTENSITY_UP :   changeEffectIntensity(lastRepeatableValue); return;
  // case ACTION_INTENSITY_DOWN : changeEffectIntensity(lastRepeatableValue); return;
  default: break;
  }    
}


uint32_t lastRepeatableActionTimer = 0;
boolean RemoteControl_ir::decode(uint32_t code)
{
  if (code == 0xFFFFFFFF) {
    //repeated code, continue brightness up/down
    irTimesRepeated++;
    applyRepeatActions();
    lastRepeatableActionTimer = millis();
    return false;
  }
  lastValidCode = 0; irTimesRepeated = 0;
  lastRepeatableAction = ACTION_NONE;
  lastRepeatableActionTimer = millis();

  if (code > 0xFFFFFF) return false; //invalid code
  decode44(code);
  decode24(code);
  // Serial.printf_P(PSTR("IR recv: 0x%lX\n"), (unsigned long)code);
  return true;
}

boolean RemoteControl_ir::handle() {
  if (irEnabled > 0 && millis() - irCheckedTime > 120) {
    irCheckedTime = millis();
    if (irEnabled > 0) {
      if (irrecv == NULL) { 
        init(); 
        return false;
      }
      if (irrecv->decode(&results)) {
        boolean result = decode(results.value);
        if (_irMod == RemoteControlIrMod_t::RIRMOD_INTERN && result) DevicePtrGet()->outputs_savForce();
        if (_irMod == RemoteControlIrMod_t::RIRMOD_TFT && result) {
          DynamicJsonDocument doc(255);
          doc[F("op")] = 8;
          doc[F("from")] = 1;
          String out;
          serializeJson(doc, out);
          RemoteControlPtrGet()->send_toIp(out, {8,8,8,8}, 9100);          
        }        
        irrecv->resume();
        return result;
      }
    } else if (irrecv != NULL) {
      irrecv->disableIRIn();
      delete irrecv; irrecv = NULL;
    }
  }
  if (_irMod == RemoteControlIrMod_t::RIRMOD_TFT && lastRepeatableActionTimer!=0 && (millis()-lastRepeatableActionTimer) > 1000  ) {
    DynamicJsonDocument doc(255);
    doc[F("op")] = 8;
    doc[F("from")] = 1;
    String out;
    serializeJson(doc, out);
    RemoteControlPtrGet()->send_toIp(out, {8,8,8,8}, 9100);  
    lastRepeatableActionTimer= 0;        
  }   
  return false;
}

void RemoteControl_ir::begin() {
  ALT_TRACEC("main", "-\n");
  if (irEnabled > 0) {
    irrecv = new IRrecv(irPin);
    irrecv->enableIRIn();
  }
}