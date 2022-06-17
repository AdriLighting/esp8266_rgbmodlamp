
#include "../../include/display/ir.h"
#include "../../include/libextern.h"
#include "../../include/display/lamp.h"
#include "../../include/display/remote.h"
#include "../../include/device/device.h"



// RemoteControl::RemoteControl(){

// }




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
      DevicePtrGet()->parseJson_device(doc);
      return;
    }
  }
}
// void RemoteControl_ir::changeEffBri(int8_t amount){
//   DynamicJsonDocument doc(255);
//   doc[FPSTR(ALMLPT_REQ)]   = FPSTR(req_ledBriIncr);
//   JsonObject val = doc.createNestedObject(FPSTR(ALMLPT_VAL));
//   val[FPSTR(ALMLPT_V)]     = amount;
//   DevicePtrGet()->parseJson_device(doc);
//   if(amount > 0) lastRepeatableAction = ACTION_SPEED_UP;
//   if(amount < 0) lastRepeatableAction = ACTION_SPEED_DOWN;
//   lastRepeatableValue = amount;  
// }
void RemoteControl_ir::changeBrightness(int8_t amount) {
  // DevicePtrGet()->get_outputArray(0)->set_IRbriLoop(dir, 0);
  DynamicJsonDocument doc(255);
  doc[FPSTR(ALMLPT_REQ)]   = FPSTR(req_ledBriIncr);
  JsonObject val = doc.createNestedObject(FPSTR(ALMLPT_VAL));
  val[FPSTR(ALMLPT_V)]     = amount;
  DevicePtrGet()->parseJson_device(doc);  
  if(amount > 0) lastRepeatableAction = ACTION_BRIGHT_UP;
  if(amount < 0) lastRepeatableAction = ACTION_BRIGHT_DOWN;
  lastRepeatableValue = amount; 
}
void RemoteControl_ir::changeEffSpeed(int8_t amount){
  DynamicJsonDocument doc(255);
  doc[FPSTR(ALMLPT_REQ)]   = FPSTR(req_eff_speedIncr);
  JsonObject val = doc.createNestedObject(FPSTR(ALMLPT_VAL));
  val[FPSTR(ALMLPT_V)]     = amount;
  DevicePtrGet()->parseJson_device(doc);
  if(amount > 0) lastRepeatableAction = ACTION_SPEED_UP;
  if(amount < 0) lastRepeatableAction = ACTION_SPEED_DOWN;
  lastRepeatableValue = amount;  
}


void changeEffect(uint8_t p){
  DynamicJsonDocument doc(255);
  doc[FPSTR(ALMLPT_REQ)]   = FPSTR(req_eff_f);
  JsonObject val = doc.createNestedObject(FPSTR(ALMLPT_VAL));
  val[FPSTR(ALMLPT_V)]     = p;
  val[FPSTR(ALMLPT_C)]     = FPSTR(req_eff_load);
  DevicePtrGet()->parseJson_device(doc);
}

void changeColor(uint32_t c){
  DynamicJsonDocument doc(255);
  ledsTools           _ledsTools;
  CRGB                rgb = _ledsTools.rgb32_crgb(c);  
  doc[FPSTR(ALMLPT_REQ)]   = FPSTR(req_lampSetColor_f);
  JsonObject val = doc.createNestedObject(FPSTR(ALMLPT_VAL));
  val[F("r")]     = rgb.r;
  val[F("g")]     = rgb.g;
  val[F("b")]     = rgb.b;
  DevicePtrGet()->parseJson_device(doc);
}

void changeoNoFF(uint8_t v){
  DynamicJsonDocument doc(255);
  doc[FPSTR(ALMLPT_REQ)]   = FPSTR(req_lampOnOff);
  JsonObject val = doc.createNestedObject(FPSTR(ALMLPT_VAL));
  val[FPSTR(ALMLPT_V)]     = String(v);
  DevicePtrGet()->parseJson_device(doc);
}

void changeTW(){
  DynamicJsonDocument doc(255);
  doc[FPSTR(ALMLPT_REQ)] = FPSTR(req_lampWhite_f);
  doc.createNestedObject(FPSTR(ALMLPT_VAL));
  DevicePtrGet()->parseJson_device(doc);
}




void RemoteControl_ir::begin() {
	ALT_TRACEC("main", "-\n");
  if (irEnabled > 0) {
    irrecv = new IRrecv(irPin);
    irrecv->enableIRIn();
  }
}



void RemoteControl_ir::decode44(uint32_t code) {
  // boolean result = false;
  switch (code) {
    case IR44_BPLUS       : /*Serial.println("IR44_BPLUS");*/ 			changeBrightness(8); 				    break;
    case IR44_BMINUS      : /*Serial.println("IR44_BMINUS");*/ 			changeBrightness(-8); 				  break;
    case IR44_OFF         : /*Serial.println("IR44_OFF");*/ 				changeoNoFF(1); 								break;
    case IR44_ON          : /*Serial.println("IR44_ON");*/ 					changeoNoFF(0); 								break;
    case IR44_RED         : /*Serial.println("IR44_RED");*/ 				changeColor(COLOR_RED); 				break;
    case IR44_REDDISH     : /*Serial.println("IR44_REDDISH");*/ 		changeColor(COLOR_REDDISH); 		break;
    case IR44_ORANGE      : /*Serial.println("IR44_ORANGE");*/ 			changeColor(COLOR_ORANGE); 			break;
    case IR44_YELLOWISH   : /*Serial.println("IR44_YELLOWISH");*/		changeColor(COLOR_YELLOWISH);		break;
    case IR44_YELLOW      : /*Serial.println("IR44_YELLOW");*/ 			changeColor(COLOR_YELLOW);			break;
    case IR44_GREEN       : /*Serial.println("IR44_GREEN");*/ 			changeColor(COLOR_GREEN);				break;
    case IR44_GREENISH    : /*Serial.println("IR44_GREENISH");*/ 		changeColor(COLOR_GREENISH);		break;
    case IR44_TURQUOISE   : /*Serial.println("IR44_TURQUOISE");*/ 	changeColor(COLOR_TURQUOISE);		break;
    case IR44_CYAN        : /*Serial.println("IR44_CYAN");*/ 				changeColor(COLOR_CYAN); 				break;
    case IR44_AQUA        : /*Serial.println("IR44_AQUA");*/ 				changeColor(COLOR_AQUA); 				break;
    case IR44_BLUE        : /*Serial.println("IR44_BLUE");*/				changeColor(COLOR_BLUE); 				break;
    case IR44_DEEPBLUE    : /*Serial.println("IR44_DEEPBLUE");*/ 		changeColor(COLOR_DEEPBLUE); 		break;
    case IR44_PURPLE      : /*Serial.println("IR44_PURPLE");*/ 			changeColor(COLOR_PURPLE); 			break;
    case IR44_MAGENTA     : /*Serial.println("IR44_MAGENTA");*/ 		changeColor(COLOR_MAGENTA); 		break;
    case IR44_PINK        : /*Serial.println("IR44_PINK");*/ 				changeColor(COLOR_PINK); 				break;
    case IR44_WHITE       : /*Serial.println("IR44_WHITE");*/ 			break;
    case IR44_WARMWHITE2  : /*Serial.println("IR44_WARMWHITE2");*/ 	break;
    case IR44_WARMWHITE   : /*Serial.println("IR44_WARMWHITE");*/ 	break;
    case IR44_COLDWHITE   : /*Serial.println("IR44_COLDWHITE");*/ 	break;
    case IR44_COLDWHITE2  : /*Serial.println("IR44_COLDWHITE2");*/ 	break;
    case IR44_REDPLUS     : /*Serial.println("IR44_REDPLUS");*/ 		_EffectslistPtr->eff_next(); 		break;
    case IR44_REDMINUS    : /*Serial.println("IR44_REDMINUS");*/ 		_EffectslistPtr->eff_prev(); 		break;
    case IR44_GREENPLUS   : /*Serial.println("IR44_GREENPLUS");*/ 	break;
    case IR44_GREENMINUS  : /*Serial.println("IR44_GREENMINUS");*/ 	break;
    case IR44_BLUEPLUS    : /*Serial.println("IR44_BLUEPLUS");*/ 		break;
    case IR44_BLUEMINUS   : /*Serial.println("IR44_BLUEMINUS");*/ 	break;
    case IR44_QUICK       : /*Serial.println("IR44_QUICK");*/ 			changeEffSpeed(16);  													break;
    case IR44_SLOW        : /*Serial.println("IR44_SLOW");*/ 				changeEffSpeed(-16); 													break;
    case IR44_DIY1        : /*Serial.println("IR44_DIY1");*/ 				changeEffectByName(TEFF_Colortwinkle); 				break;
    case IR44_DIY2        : /*Serial.println("IR44_DIY2");*/ 				changeEffectByName(TEFF_RainbowCycleSyncro); 	break;
    case IR44_DIY3        : /*Serial.println("IR44_DIY3");*/ 				changeEffectByName(TEFF_Breath); 							break;
    case IR44_DIY4        : /*Serial.println("IR44_DIY4");*/ 				changeEffectByName(TEFF_RainbowSyncro);	 			break;
    case IR44_DIY5        : /*Serial.println("IR44_DIY5");*/ 				changeEffectByName(TEFF_RainbowWave); 				break;
    case IR44_DIY6        : /*Serial.println("IR44_DIY6");*/ 				changeEffectByName(TEFF_Commets); 						break;
    case IR44_AUTO        : /*Serial.println("IR44_AUTO");*/ 				_EffectslistPtr->toggle();										break;
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
    case IR24_OFF       : changeoNoFF(1);                     break;
    case IR24_ON        : changeoNoFF(0);                     break;
    case IR24_RED       : changeColor(COLOR_RED);             break;
    case IR24_REDDISH   : changeColor(COLOR_REDDISH);         break;
    case IR24_ORANGE    : changeColor(COLOR_ORANGE);          break;
    case IR24_YELLOWISH : changeColor(COLOR_YELLOWISH);       break;
    case IR24_YELLOW    : changeColor(COLOR_YELLOW);          break;
    case IR24_GREEN     : changeColor(COLOR_GREEN);           break;
    case IR24_GREENISH  : changeColor(COLOR_GREENISH);        break;
    case IR24_TURQUOISE : changeColor(COLOR_TURQUOISE);       break;
    case IR24_CYAN      : changeColor(COLOR_CYAN);            break;
    case IR24_AQUA      : changeColor(COLOR_AQUA);            break;
    case IR24_BLUE      : changeColor(COLOR_BLUE);            break;
    case IR24_DEEPBLUE  : changeColor(COLOR_DEEPBLUE);        break;
    case IR24_PURPLE    : changeColor(COLOR_PURPLE);          break;
    case IR24_MAGENTA   : changeColor(COLOR_MAGENTA);         break;
    case IR24_PINK      : changeColor(COLOR_PINK);            break;
    // case IR24_WHITE     : changeTW();                         break;
    case IR24_WHITE     : changeEffectByName(TEFF_ColorWipe);           break;
    case IR24_FLASH     : changeEffectByName(TEFF_Colortwinkle);        break; // 27 EffectFx_mode_colortwinkle
    case IR24_STROBE    : changeEffectByName(TEFF_RainbowCycleSyncro);  break; // 15 EffectFx_mode_rainbow_cycle
    case IR24_FADE      : changeEffectByName(TEFF_BreathSyncro);        break; // 28 EffectFx_mode_breath
    case IR24_SMOOTH    : changeEffectByName(TEFF_RainbowSyncro);       break; // 26 EffectFx_mode_rainbow
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

boolean RemoteControl_ir::decode(uint32_t code)
{
  if (code == 0xFFFFFFFF) {
    //repeated code, continue brightness up/down
    irTimesRepeated++;
    applyRepeatActions();
    return false;
  }
  lastValidCode = 0; irTimesRepeated = 0;
  lastRepeatableAction = ACTION_NONE;

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
        if (result) DevicePtrGet()->outputs_savForce();
        irrecv->resume();
        return result;
      }
    } else if (irrecv != NULL) {
      irrecv->disableIRIn();
      delete irrecv; irrecv = NULL;
    }
  }
  return false;
}
