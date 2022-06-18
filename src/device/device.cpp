#include "../../include/device/device.h"



#include "../../include/fastled/fastled.h"
#include "../../include/pixel/effectcore.h"
#include "../../include/display/lamp.h"
#include "../../include/alml_webserver.h"

extern AsyncWebServer web_server;


void taskCb_wifi(){
  WCEVO_managerPtrGet()->handleConnection();
}  
void taskCb_program(){
 if (!RemoteControlPtrGet() && !LAMPPTRGET()->isLampOn()) return;
 RemoteControlPtrGet()->get_program()->handle(); 
}
#ifdef ADS_PIN_IR
  void taskCb_ir(){
    if (!RemoteControlPtrGet())  return;
    RemoteControlPtrGet()->handle_ir();  
  }  
#endif
void taskCb_serial(){
  #ifdef DEBUG_KEYBOARD
  _Sr_menu.serialRead();  
  #endif
}
void taskCb_lamp(){
  LAMPPTRGET()->loop();  
}
void taskCb_webserver(){
  _Webserver.handle();
}
void taskCb_udp(){
  if (!RemoteControlPtrGet()) return;
  RemoteControlPtrGet()->handle_udp();  
}
PROGMEM taskCbList _taskCbList [] = { 
  #ifdef ADS_PIN_IR
  {&taskCb_ir         , 50000},  
  #endif
  {&taskCb_udp        , 100000},
  {&taskCb_lamp       , 5000},
  {&taskCb_program    , 100000},
  {&taskCb_wifi       , 50000},
  {&taskCb_webserver  , 100000},
  {&taskCb_serial     , 100000}
};
uint8_t _taskCbListCnt = ARRAY_SIZE(_taskCbList);

/*
  instance de la clase leds
    mise en memoi des info strip
  
*/
#ifdef ADS_PIN_1
  leds <ADS_PIN_1, GRB>myLed_1(0);
#endif
#ifdef ADS_PIN_2
  leds <ADS_PIN_2, GRB>myLed_2(1);
#endif
#ifdef ADS_PIN_3
  leds <ADS_PIN_3, GRB>myLed_3(2);
#endif
#ifdef ADS_PIN_4
  leds <ADS_PIN_4, GRB>myLed_4(3);
#endif
#ifdef ADS_PIN_5
  leds <ADS_PIN_5, GRB>myLed_5(4);
#endif
#ifdef ADS_PIN_6
  leds <ADS_PIN_6, GRB>myLed_6(5);
#endif

#ifdef ADS_PINW_1
  leds <ADS_PINW_1, GRB>myLed_1(0);
#endif
#ifdef ADS_PINW_2
  leds <ADS_PINW_2, GRB>myLed_2(1);
#endif
#ifdef ADS_PINW_3
  leds <ADS_PINW_3, GRB>myLed_3(2);
#endif
#ifdef ADS_PINW_4
  leds <ADS_PINW_4, GRB>myLed_4(3);
#endif
#ifdef ADS_PINW_5
  leds <ADS_PINW_5, GRB>myLed_5(4);
#endif
#ifdef ADS_PINW_6
  leds <ADS_PINW_6, GRB>myLed_6(5);
#endif


TaskSimple * _task_reset = nullptr;


/* 
  doc json statique pour la generalisation des sauvegardes dans la memoir spiff 
  permettre de filtrer les changement effectuer pour chaque stip

  requett (slider, bt etc) en cour
    ajout du strip modifier au doc

  requett (slider, bt etc) terminer
    lecture du doc
      sauvegarde
    reinitialisation du doc  
  
  ... a mod
*/
/** \brief doc json statique pour la generalisation des sauvegardes dans la memoir spiff  */  
StaticJsonDocument<200> OutputSav;

/* 
  ptr ver l'array des instance EffectWorker

    taille = nombre d'output 
      chaque classique sera statique
 
 */
/** \brief ptr ver l'array des instance EffectWorker*/
extern EffectWorker  * _effects[];

Device * _DevicePtr = nullptr;
Device * DevicePtrGet() {return _DevicePtr;};

/**
 *
 * @brief   constructor destinée a etre unique
 * @see     https://github.com/AdriLighting/
 *
 * @author  AdriLighting
 * @date    mar. 28 déc. 2021 16:20:51
 * 
 * @param[in]  name  hostanme
 * @param[in]  oc    nombre de strip
 */
Device::Device(const char * name, uint8_t oc){    
  _DevicePtr          = this;
  
  if(!_task_reset) _task_reset = new TaskSimple();

  /* creation des class Output pour chhaque strip */
  _outputArray        = new Output[oc];

  /* nom unique servant de hostname */
  _name               = name;

  /* nombre total de strip */
  _output_count       = oc;

  /* ip esp */
  // _ip                 = Network.localIP();


  _leds_manage = new leds_manage();

  #ifndef ADS_LEDSFASTLEDS_AUTO
    #ifdef ADS_PIN_1
        FastLED.addLeds<WS2812B, ADS_PIN_1,   GRB>(myLed_1._leds,     myLed_1._num_leds);
    #endif
    #ifdef ADS_PINW_1
        FastLED.addLeds<WS2812B, ADS_PINW_1,  RGB>(myLed_1._ledsRGB,  myLed_1._num_ledsW);
    #endif

    #ifdef ADS_PIN_2
        FastLED.addLeds<WS2812B, ADS_PIN_2,   GRB>(myLed_2._leds,     myLed_2._num_leds);
    #endif
    #ifdef ADS_PINW_2
        FastLED.addLeds<WS2812B, ADS_PINW_2,  RGB>(myLed_2._ledsRGB,  myLed_2._num_ledsW);
    #endif

    #ifdef ADS_PIN_3
        FastLED.addLeds<WS2812B, ADS_PIN_3,   GRB>(myLed_3._leds,     myLed_3._num_leds);
    #endif
    #ifdef ADS_PINW_3
        FastLED.addLeds<WS2812B, ADS_PINW_3,  RGB>(myLed_3._ledsRGB,  myLed_3._num_ledsW);
    #endif

    #ifdef ADS_PIN_4
        FastLED.addLeds<WS2812B, ADS_PIN_4,   GRB>(myLed_4._leds,     myLed_4._num_leds);
    #endif
     #ifdef ADS_PINW_4
        FastLED.addLeds<WS2812B, ADS_PINW_4,  RGB>(myLed_4._ledsRGB,  myLed_4._num_ledsW);
    #endif
      
    #ifdef ADS_PIN_5
        FastLED.addLeds<WS2812B, ADS_PIN_5,   GRB>(myLed_5._leds,     myLed_5._num_leds);
    #endif
    #ifdef ADS_PINW_5
        FastLED.addLeds<WS2812B, ADS_PINW_5,  RGB>(myLed_5._ledsRGB,  myLed_5._num_ledsW);
    #endif

    #ifdef ADS_PIN_6
        FastLED.addLeds<WS2812B, ADS_PIN_6,   GRB>(myLed_6._leds,     myLed_6._num_leds);
    #endif
    #ifdef ADS_PINW_6
        FastLED.addLeds<WS2812B, ADS_PINW_6,  RGB>(myLed_6._ledsRGB,  myLed_6._num_ledsW);
    #endif

  #endif

  #if defined (ADS_PIN_1) || defined (ADS_PINW_1)
    _leds_manage->strip_add(&myLed_1);
  #endif
  #if defined (ADS_PIN_2) || defined (ADS_PINW_2)
    _leds_manage->strip_add(&myLed_2);
  #endif
  #if defined (ADS_PIN_3) || defined (ADS_PINW_3)
    _leds_manage->strip_add(&myLed_3);
  #endif
  #if defined (ADS_PIN_4) || defined (ADS_PINW_4)
    _leds_manage->strip_add(&myLed_4);
  #endif
  #if defined (ADS_PIN_5) || defined (ADS_PINW_5)
    _leds_manage->strip_add(&myLed_5);
  #endif
  #if defined (ADS_PIN_6) || defined (ADS_PINW_6)
    _leds_manage->strip_add(&myLed_6);
  #endif


  ALT_TRACEC("main", "[Device][name: %s][oc: %d]\n", _name, _output_count);

  get_outputArray(0)->_leds_segment.segment_new(0, 14,  false);
  get_outputArray(0)->_leds_segment.segment_new(15, 29, true);

  // get_outputArray(0)->_leds_segment.segment_new(0, 63,  false);
  // get_outputArray(0)->_leds_segment.segment_new(64, 127, true);
  // int     arm_left      = (128/2);
  // int     arm_right     = (128/2);
  // int     mod_left      = arm_left % 4;
  // int     mod_right     = arm_right % 4;
  // uint8_t arm_div_left  = arm_left / 4;
  // uint8_t arm_div_right = arm_right / 4;
  // get_outputArray(0)->_armoire.segemnt_new(0,                             arm_div_left-1,                               false,  aramoire_side_left  );
  // get_outputArray(0)->_armoire.segemnt_new(arm_left  + (arm_div_right*3), arm_left + (((arm_div_right*4)-1)+mod_right), true,   aramoire_side_right );
  // get_outputArray(0)->_armoire.segemnt_new(arm_div_left,                  (arm_div_left*2)-1,                           false,  aramoire_side_left  );
  // get_outputArray(0)->_armoire.segemnt_new(arm_left + (arm_div_right*2),   arm_left + ((arm_div_right*3)-1),            true,   aramoire_side_right );
  // get_outputArray(0)->_armoire.segemnt_new(arm_div_left*2,                (arm_div_left*3)-1,                           false,  aramoire_side_left  );
  // get_outputArray(0)->_armoire.segemnt_new(arm_left + arm_div_right,      arm_left + ((arm_div_right*2)-1),             true,   aramoire_side_right );
  // get_outputArray(0)->_armoire.segemnt_new(arm_div_left*3,                ((arm_div_left*4)-1)+mod_left,                false,  aramoire_side_left  );
  // get_outputArray(0)->_armoire.segemnt_new(arm_left,                      arm_left + (arm_div_right-1),                 true,   aramoire_side_right );  
  // Serial.printf_P(PSTR("[mod_left][%d]"), mod_left);
  // Serial.printf_P(PSTR("[mod_right][%d]"), mod_right);
  // Serial.printf_P(PSTR("[arm_div_left][%d]"), arm_div_left);
  // Serial.printf_P(PSTR("[arm_div_right][%d]\n"), arm_div_right);



  ALT_TRACEC("main", "LAMP new instance\n");
  new LAMP(0);
  delay(2);

  ALT_TRACEC("main", "RemoteControl new instance\n");  
  new RemoteControl();

  ALT_TRACEC("main", "al_taskScheduler new instance\n");
  _TaskScheduler = new al_taskScheduler(8+_output_count);

  uint8_t pos = _output_count+1;
  for(int i = 0; i < _taskCbListCnt; ++i) {
    _TaskScheduler->get_task(pos)->set_callback(_taskCbList[i]._task); 
    _TaskScheduler->get_task(pos)->set_taskDelay(ETD::ETD_TIMER, true, _taskCbList[i]._duration);
    _TaskScheduler->get_task(pos)->set_iteration_max(-1);
    _TaskScheduler->get_task(pos)->setup(true);
    _TaskScheduler->get_task(pos)->set_enabled(true);        
    pos++;
  }
  WCEVO_managerPtrGet()->set_cb_webserveAprEvent([](){
    if (RemoteControlPtrGet()) RemoteControlPtrGet()->begin();
  });  
  WCEVO_managerPtrGet()->set_cb_serverEvent([](){
    if (RemoteControlPtrGet()) RemoteControlPtrGet()->begin();
  });
  WCEVO_managerPtrGet()->set_cb_webserverOn([](){
    _Webserver.setup(); 
  });
  WCEVO_managerPtrGet()->set_cb_webserveAprOn([](){
    _Webserver.setupAp(); 
  });

  if(!FILESYSTEM.exists(F("/outputs"))) {ALT_TRACEC("main", "spiff create folder /outputs\n");FILESYSTEM.mkdir(F("/outputs"));}
  if(!FILESYSTEM.exists(F("/presets"))) {ALT_TRACEC("main", "spiff create folder /presets\n");FILESYSTEM.mkdir(F("/presets"));}
  if (!FILESYSTEM.exists(F("/outputs/lamp.txt"))) {
    ALT_TRACEC("main", "spiff create file /outputs/lamp.txt\n");
    File f;
    LH_file * _LH_file = new LH_file(f, "/outputs/lamp.txt");
    for(int i=0; i<_output_count; i++) _LH_file->newLine("output_"+String(i));
    delete _LH_file;
  } 


  char        buffer[80];
  String      buffer_str;

  /* extraction pour les nom par default de chaque strip */
  sprintf(buffer, "%s", ADS_OLOC);
  buffer_str = String(buffer);  
	int rSize;
	const char** Lines;
	Lines = al_tools::explode(buffer_str, '.', rSize);	

  boolean     isTw;
  uint16_t    nbLeds;
  uint8_t     oPin;

  ALT_TRACEC("main", "configuration des output\n");

  /* initialisation de chaque strip */
  for(int i=0; i<_output_count; i++){

    ALT_TRACEC("main", "&c:1&s:\tpos: %d\n", i);

    /* atrribution de la position du strip */
    _outputArray[i].set_pos(i);

    /* recuperation de info sur le strip */
    leds_managePtrGet()->get_numLeds(i, nbLeds);
    leds_managePtrGet()->get_ledsPin(i, oPin);
    leds_managePtrGet()->get_ledsIsTw(i, isTw);
    
    /* initilalisation de nombre de leds */
    _outputArray[i].set_size(nbLeds);
    
    /* initialisation de la pin utiiser par le strip */
    _outputArray[i].set_pin(oPin);

    /* initialisation du type de strip
      0 = RGB ws2812b
      1 = RGBW SK6812
     */ 
    (!isTw)?_outputArray[i].set_type(0):_outputArray[i].set_type(1);

    if ( !Lines ) _outputArray[i].set_name(buffer_str);
    else _outputArray[i].set_name(al_tools::ch_toString(Lines[i]));

    _outputArray[i]._isArmoire = _outputArray[i]._armoire.isSegemnt();

    /** initialisation de du dossier ou sera sauvegarder l'etat du strip */
    _outputArray[i].setValueFromSpiff();


  }
  if (Lines){
		for(int i = 0; i < rSize; ++i) {delete Lines[i];}delete[] Lines;				
  }

  DynamicJsonDocument doc(2048);
  JsonObject root = doc.to<JsonObject>();
  JsonObject oOjbect = root.createNestedObject(FPSTR(ALMLPT_OUTPUTS));
  outputs_toJson(oOjbect, false, true);
  serializeJsonPretty(doc,Serial);Serial.println();

  // delay(2);
  
  for(uint8_t i=0; i< _output_count; i++) {
    LAMPPTRGET()->effectsTimer(i, T_ENABLE);
  }
  

    // get_outputArray(0)->_armoire.segemnt_leftRight((arm_div_left - mod_left)-1);
    // get_outputArray(0)->set_color(0, CRGB(0, 255, 0), 80, 255, 255);
// FastLED.show();
  ALT_TRACEC("main", "--\n");
}

al_taskScheduler * Device::get_taskSheduler() {return _TaskScheduler;}
TaskSimple * Device::get_task(uint8_t p) {return _TaskScheduler->get_task(p);}

void Device::get_outputCount(uint8_t & ret){ret = _output_count;}

/**
 * @brief      pass par reference le ptr de la class Output
 *
 * @param[in]  pos  position du strip rechercher
 *
 * @return     ptr par reference
 */
Output * Device::get_outputArray(uint8_t pos){
  Output * ptr = & _outputArray[pos];
  return ptr;
}

const char * Device::get_name() {return _name;}

/**
 * @brief      loop destiner a etre executer dans le main.cpp
 */
void Device::loop(){
  _TaskScheduler->loop();
  if(_task_reset) {if (_task_reset->isEnabled()) _task_reset->execute();}

}




/**
 * @brief     check le type de commande 
 * @details   slider H||S||V, colopricker RGB||RGB+HSV, slider TW, slider TW_V, slider BRI, bouton rgb, tw
 * @see      https://github.com/AdriLighting/
 *
 * @author  AdriLighting
 * @date    mar. 28 déc. 2021 16:20:51
 * 
 * @param[in]  cmd   The command
 *
 * @return     return true si c'est une comannd de type couleur
 */
boolean Device::isColorRequest(const String & cmd){
  if      (cmd == FPSTR(req_lampHue))       return true; 
  else if (cmd == FPSTR(req_lampSat))       return true; 
  else if (cmd == FPSTR(req_lampBri))       return true; 
  else if (cmd == FPSTR(req_lampWhite))     return true;
  else if (cmd == FPSTR(req_lampSetColor))  return true;
  else if (cmd == FPSTR(req_lampSetColor_f))  return true;
  else if (cmd == FPSTR(req_lampWhite_v))   return true;
  else if (cmd == FPSTR(req_lampWhite_f))   return true;
  else if (cmd == FPSTR(req_ledBri))        return true;
  return false; 
}

boolean Device::isEffectRequest(const String & cmd){
  if      (cmd == FPSTR(req_ledBri))        return true; 
  else if (cmd == FPSTR(req_ledBriIncr))    return true; 
  else if (cmd == FPSTR(req_eff_activ))     return true; 
  else if (cmd == FPSTR(req_eff_next))      return true;
  else if (cmd == FPSTR(req_eff_prev))      return true;
  else if (cmd == FPSTR(req_eff_load))      return true;
  else if (cmd == FPSTR(req_eff_f))         return true;
  else if (cmd == FPSTR(req_eff_speed))     return true;
  else if (cmd == FPSTR(req_eff_speedIncr)) return true;
  else if (cmd == FPSTR(req_eff_bri))       return true;
  else if (cmd == FPSTR(req_eff_briIncr))   return true;
  else if (cmd == FPSTR(req_eff_scale))     return true;
  else if (cmd == FPSTR(req_eff_id))        return true;
  else if (cmd == FPSTR(req_eff_autoplay))  return true;
  return false; 
}

boolean Device::isEffectLoadRequest(const String & cmd){
  if (cmd == FPSTR(req_eff_next))           return true;
  else if (cmd == FPSTR(req_eff_prev))      return true;
  else if (cmd == FPSTR(req_eff_load))      return true;
  else if (cmd == FPSTR(req_eff_f))         return true;
  return false; 
}

boolean Device::outputs_requestEffect(){
  String req = OutputSav[FPSTR(ALMLPT_REQUEST)].as<String>();
  return isEffectRequest(req);
}

/**
 * @brief      
 * @details   si command de type suivant/precedent
 * @details   si command de type String(<select>)
 *          
 * @param[in]  cmd    The command
 * @param[in]  value  The value
 *
 * @return     nom d'origine de l'effet via String
 */
void Output_set_eff (uint8_t sP, String &result, const String & cmd, const JsonObject & value){
  if (!ProgramPtrGet()) return;
  if ((cmd == FPSTR(req_eff_next)))       {DevicePtrGet()->get_outputArray(sP)->set_effActiv(true);ProgramPtrGet()->set_itemNext();ProgramPtrGet()->get_itemBase(result);}
  else if ((cmd == FPSTR(req_eff_prev)))  {DevicePtrGet()->get_outputArray(sP)->set_effActiv(true);ProgramPtrGet()->set_itemPrev();ProgramPtrGet()->get_itemBase(result);} 
  else if ((cmd == FPSTR(req_eff_load))) {
    result = value[F("v")].as<String>();
  }    
}
void Output_set_eff_f(uint8_t sP, String &result, const String & cmd, const JsonObject & value){
  if (!ProgramPtrGet()) return;
  if (cmd != FPSTR(req_eff_f)) return;

  String action = value[F("c")].as<String>();
  if ((action == FPSTR(req_eff_next)))       {DevicePtrGet()->get_outputArray(sP)->set_effActiv(true);ProgramPtrGet()->set_itemNext();ProgramPtrGet()->get_itemBase(result);}
  else if ((action == FPSTR(req_eff_prev)))  {DevicePtrGet()->get_outputArray(sP)->set_effActiv(true);ProgramPtrGet()->set_itemPrev();ProgramPtrGet()->get_itemBase(result);} 
  else if ((action == FPSTR(req_eff_load))) {
    DevicePtrGet()->get_outputArray(sP)->set_effActiv(true);
    uint8_t p = value[F("v")].as<uint8_t>();
    ProgramPtrGet()->set_itemById(p);
    ProgramPtrGet()->get_itemBase(result);
  } 
}

void Device::parseJson_output(uint8_t p , String & nextEffect, const String & req, JsonObject val) {

  get_outputArray(p)->set_select(req, val);
  if (!get_outputArray(p)->get_select()) return;

  get_outputArray(p)->set_onoff(req, val);
  get_outputArray(p)->set_toggle(req, val);

  if (!get_outputArray(p)->get_onoff()) return; 

  get_outputArray(p)->set_etage(req, val);
  get_outputArray(p)->set_bri(req, val);
  get_outputArray(p)->set_briIncr(req, val);
  get_outputArray(p)->set_hsv_hIncr(req, val);

  get_outputArray(p)->set_rgb_f(req, val);
  get_outputArray(p)->set_tw_f(req, val);

  get_outputArray(p)->set_eff_activ(req, val);
  // get_outputArray(p)->set_eff_f(req, val);
  get_outputArray(p)->set_eff_autoplay(req, val);   


  if (nextEffect=="") {Output_set_eff_f(p, nextEffect, req, val);} 

  if (get_outputArray(p)->get_eff_activ()) {

    LAMPPTRGET()->effectsLoop_Timer(req, val);
    get_outputArray(p)->set_eff_colorMod(req, val);
    get_outputArray(p)->set_eff_id(req, val);
    get_outputArray(p)->set_eff_rgb1(req, val);
    get_outputArray(p)->set_eff_bri(req, val);   
    get_outputArray(p)->set_eff_scale(req, val);   
    get_outputArray(p)->set_eff_speed(req, val);   
    get_outputArray(p)->set_eff_speedIncr(req, val);   

    // changemant unique de l'effet (ne doit sexcuter qu'une fois dansl le loop)
    if (nextEffect=="") {Output_set_eff(p, nextEffect, req, val);} 

    // chargement de l'effet pour chaque strip        
    if (nextEffect!="") {
      for (uint8_t j = 0; j < _effeNamIDList_cnt; ++j) {
        String find = FPSTR(_effeNamIDList[j]._nameId);
        if (find == nextEffect) {
          LAMPPTRGET()->effectChangeByPos(p, j, LAMPPTRGET()->isFaderOn(), true);
          break;
        }
      }
    }
    return;
  }

  if (!isColorRequest(req)) return;
  get_outputArray(p)->set_rgb(req, val);
  get_outputArray(p)->set_hsv_h(req, val);
  get_outputArray(p)->set_hsv_s(req, val);
  get_outputArray(p)->set_hsv_v(req, val);
  get_outputArray(p)->set_tw(req, val);      
  get_outputArray(p)->set_tw_v(req, val);    
}

/**
 * @brief     traintements de requettes via nom de device et position du strip/output
 *   
 * @details   op == 3 \n
 *              via nom de device et position du strip/output
 *       
 * @see       https://github.com/AdriLighting/
 *
 * @author    AdriLighting
 * @date      mar. 28 déc. 2021 16:20:51
 *      
 *          
 * @param      doc deserialize depuis le paquet udp reçu
 */ 
void Device::parseJson_output(DynamicJsonDocument & doc){
  boolean result = true;
  if (!doc.containsKey(FPSTR(ALMLPT_REQ)))     result = false;   // cmd
  if (!doc.containsKey(FPSTR(ALMLPT_VAL)))     result = false;   // cmd value
  if (!doc.containsKey(FPSTR(ALMLPT_OUTPUT)))  result = false;   // Position output 
  if (!result) {return;}

  String      req     = doc[FPSTR(ALMLPT_REQ)].as<String>();     // requette ou commande
  JsonObject  val     = doc[FPSTR(ALMLPT_VAL)];                  // valeur de la requette
  uint8_t     pos     = doc[FPSTR(ALMLPT_OUTPUT)].as<uint8_t>(); // poistion output/strip

  doc.clear();   // ? UTILE OU PAS OU TEST AVEC LE HEAP
  

  /**
   * Module de sauvegarde
   *  quand un control depuis l'ui et utiliser 
   *  ajout unique de la postion du strip utiliser
   *  une fois le control ui fini (indiquer via un poquet avec l'op: 4) traitement de la sauvgarde
   *  
   *    1 -> clear le doc json
   *    2 -> mod ??
   *    3 -> création de larray
   *    4 -> ajout de la position de l'output a sauvegardé
   */
  if (_outputsSav) {
    OutputSav.clear(); 
    OutputSav.garbageCollect();
    JsonObject root = OutputSav.to<JsonObject>(); 
    root[FPSTR(ALMLPT_MOD)] = 3;
    root[FPSTR(ALMLPT_REQUEST)] = req;
    OutputSavArray = root.createNestedArray(FPSTR(ALMLPT_OUTPUTS));   
    OutputSavArray.add(pos);

    _outputsSav = false;
  }


  /** IAM WRONG ?? utile pour les selection multiple
   * si une reuqette necissite de changer/charger un effet 
   *  initialisation du nouvel effet une seul foi : hor de la boucle, pour ne pas avoir d'effet different sur chaque strip
   */
  String nextEffect = "";
  Output_set_eff(pos, nextEffect, req, val);

  parseJson_output(pos, nextEffect, req, val)  ;

}

/**
 * @brief     traintements de requettes via nom de device pour tous les strip    
 *  
 * @details   op == 1 || op == 2 \n
 *              via nom de device pour tous les strip 
 *     
 * @see       https://github.com/AdriLighting/
 *
 * @author    AdriLighting
 * @date      mar. 28 déc. 2021 16:20:51
 *  
 *          
 * @param      doc deserialize depuis le paquet udp reçu
 */
void Device::parseJson_device(DynamicJsonDocument & doc){
  boolean result = true;
  if (!doc.containsKey(FPSTR(ALMLPT_REQ))) result = false; 
  if (!doc.containsKey(FPSTR(ALMLPT_VAL))) result = false; 
  if (!result) {return;}

  String      req     = doc[FPSTR(ALMLPT_REQ)].as<String>();
  JsonObject  val     = doc[FPSTR(ALMLPT_VAL)];  

  if (_outputsSav) {
    OutputSav.clear(); 
    OutputSav.garbageCollect();
    JsonObject root = OutputSav.to<JsonObject>(); 
    root[FPSTR(ALMLPT_MOD)] = 1;
    root[FPSTR(ALMLPT_REQUEST)] = req;
    OutputSavArray = root.createNestedArray(FPSTR(ALMLPT_OUTPUTS));    
  }

  String nextEffect="";

  for(int i=0; i<_output_count; i++){
    if (_outputsSav) OutputSavArray.add(i);
    parseJson_output(i, nextEffect, req, val)  ;
  }

  if (_outputsSav) {
    _outputsSav = false;
  }
}

/**
 * @brief     traintements de requettes  via nom de device et list des strip
 * 
 * @details   op == 0 \n
 *            via nom de device et list des strip   
  *   
 * @see       https://github.com/AdriLighting/
 *
 * @author    AdriLighting
 * @date      mar. 28 déc. 2021 16:20:51
 *          
 * @param      doc deserialize depuis le paquet udp reçu
 */   
void Device::parseJson_outpitListByDn(DynamicJsonDocument & doc){
  boolean result = true;
  if (!doc.containsKey(FPSTR(ALMLPT_REQ))) result = false; 
  if (!doc.containsKey(FPSTR(ALMLPT_VAL))) result = false; 
  if (!doc.containsKey(FPSTR(ALMLPT_A)))   result = false; 
  if (!result) {return;}

  String      req     = doc[FPSTR(ALMLPT_REQ)].as<String>();
  JsonObject  val     = doc[FPSTR(ALMLPT_VAL)];
  JsonArray   list_dn = doc[FPSTR(ALMLPT_A)];
  String      thisDn  = al_tools::ch_toString(_name);

  if (_outputsSav) {
    OutputSav.clear(); 
    OutputSav.garbageCollect();
    JsonObject root = OutputSav.to<JsonObject>(); 
    root[FPSTR(ALMLPT_MOD)] = 0;
    root[FPSTR(ALMLPT_REQUEST)] = req;
    OutputSavArray = root.createNestedArray(FPSTR(ALMLPT_OUTPUTS));
  }
  
  String nextEffect="";

  for (JsonObject item : list_dn) {

    if (!item.containsKey(FPSTR(ALMLPT_DN))) continue;

    const char * dn = item [FPSTR(ALMLPT_DN)];
    if (al_tools::ch_toString(dn) != thisDn) continue;

    if (!item.containsKey(FPSTR(ALMLPT_A))) continue;
    
    JsonArray list_output = item[FPSTR(ALMLPT_A)].as<JsonArray>();
    for (uint8_t item_2 : list_output) {

      if (_outputsSav) {
        OutputSavArray.add(item_2);
      }

      parseJson_output(item_2, nextEffect, req, val)  ;

    }
    _outputsSav = false;
  }
}

/**
 * @brief     generation du json par ouput
 * @see       https://github.com/AdriLighting/
 *
 * @author    AdriLighting
 * @date      mar. 28 déc. 2021 16:20:51
 * 
 * @param[in]  pos       The position of strip
 * @param      value     doc json
 * @param[in]  shortKey  affichage humain/binaire
 * @param[in]  fs        si le json va etre ecrif en memoir spiff ou envoyer via paquet udp
 */
void Device::output_toJson(uint8_t pos, JsonObject & value, boolean shortKey, boolean fs, boolean eff){

      value[F("0")]           = _outputArray[pos].get_hsv_h();
      value[F("1")]           = _outputArray[pos].get_hsv_s();
      value[F("2")]           = _outputArray[pos].get_hsv_v();
      value[F("3")]           = _outputArray[pos].get_rgb_r();
      value[F("4")]           = _outputArray[pos].get_rgb_g();
      value[F("5")]           = _outputArray[pos].get_rgb_b();
      value[F("6")]           = _outputArray[pos].get_tw();
      value[F("7")]           = _outputArray[pos].get_isTw();
      value[F("8")]           = _outputArray[pos].get_bri();
      value[F("9")]           = _outputArray[pos].get_select();
      value[F("10")]          = _outputArray[pos].get_onoff();
      value[F("11")]          = _outputArray[pos].get_type();
      value[F("12")]          = _outputArray[pos].get_pin();
      value[F("13")]          = _outputArray[pos].get_size();
      value[F("14")]          = _outputArray[pos].get_name();   
      value[F("15")]          = _outputArray[pos].get_tw_v();      
      value[F("16")]          = _outputArray[pos].get_eff_activ(); 
      if (_effects[pos]) {
      value[F("17")]          = _effects[pos]->getEn();                       
      }
      // value[F("19")]          = _outputArray[pos].get_isTw() ? _outputArray[pos].get_tw() : _outputArray[pos].get_hsv_v();
      if (_outputArray[pos].get_eff_activ()) value[F("19")] = _effects[pos]->getControls()[0]->getVal(); 
      else { value[F("19")] =  _outputArray[pos].get_isTw() ? _outputArray[pos].get_tw() : _outputArray[pos].get_hsv_v();}    
        
      value[F("20")]          = _outputArray[pos]._armoireEtage; 
      value[F("21")]          = _outputArray[pos]._isArmoire; 

      if (_effects[pos] && eff && _outputArray[pos].get_eff_activ() ) {
        JsonObject effectObj = value.createNestedObject("effects");
        _effects[pos]->geteffconfig(pos, effectObj, LAMPPTRGET()->get_globalBrightness(pos, 0));
      }
    // ALT_TRACEC("main", "[eff][pos: %d][name: %s]\n", _effects[pos]->getEn()  , _effects[pos]->getEffectName().c_str() );    
}

void Device::output_effect_toJson(uint8_t pos, JsonObject & value){
  JsonObject effectObj = value.createNestedObject("effects");
  _effects[pos]->geteffconfig(pos, effectObj, LAMPPTRGET()->get_globalBrightness(pos, 0));
}

void Device::outputs_effect_toJson(JsonObject & oject){
  for(uint8_t j = 0; j <_output_count; ++j) {
    String pos = String(j);
    JsonObject value = oject.createNestedObject(pos);
    output_effect_toJson(j, value);
  }
}

/**
 * @brief     genere le json de tous les strip au format String
 * 
 * @details  udp toutes les 1.5sec \n
 *            apres lutilisation de lui
 *  
 * @see       https://github.com/AdriLighting/
 *
 * @author    AdriLighting
 * @date      mar. 28 déc. 2021 16:20:51
 * 
 *               
 * @param      ou    le json au format String
 */
void Device::outputs_toNode(String & out){
    DynamicJsonDocument doc(2048);
    JsonObject root = doc.to<JsonObject>();
    JsonObject oOjbect = root.createNestedObject(FPSTR(ALMLPT_OUTPUTS));
    String ip = WiFi.localIP().toString();
    root[FPSTR(ALMLPT_IP)]= ip;
    root[FPSTR(ALMLPT_DN)]= _name;
    root[FPSTR(ALMLPT_OC)]= _output_count;
    root[FPSTR(ALMLPT_CLI)]= FPSTR(ALMLPT_RGB);
    outputs_toJson(oOjbect, true, false);
    serializeJson(doc, out);
}

void Device::outputs_toAppi(String & out, boolean eff, boolean prog, uint8_t effLoad){
    DynamicJsonDocument doc(3200);
    JsonObject root = doc.to<JsonObject>();
    String ip = WiFi.localIP().toString();
    root[FPSTR(ALMLPT_IP)]= ip;
    root[FPSTR(ALMLPT_DN)]= _name;
    root[FPSTR(ALMLPT_OC)]= _output_count;
    root[FPSTR(ALMLPT_CLI)]= FPSTR(ALMLPT_RGB);
    if (LAMPPTRGET()->isFaderOn()){
      if (effLoad == 0) {
        uint8_t p = isEffectLoadRequest(OutputSav[FPSTR(ALMLPT_REQUEST)].as<String>()) ? 1 : 0;
        root[F("effload")] = p;
        _effLoad = p;

      } else if (effLoad == 1) { 
        root[F("effload")] = 1;
        _effLoad = 1;

      } else if (effLoad == 2) { 
        root[F("effload")] = 0;
        _effLoad = 2;

      } else if (effLoad == 3) { 
        root[F("effload")] = _effLoad;
      }
    } else {
      root[F("effload")] = 0;
      _effLoad = 2;
    }

    JsonObject oOjbect = root.createNestedObject(F("lamp"));
    // oOjbect[F("0")] = LAMPPTRGET()->isColorPicker();
    oOjbect[F("1")] = LAMPPTRGET()->isRandDemoOn();
    // oOjbect[F("2")] = LAMPPTRGET()->isDebugOn();
    oOjbect[F("3")] = LAMPPTRGET()->isLampOn();
    oOjbect[F("4")] = LAMPPTRGET()->isGlobalBrightnessOn();
    oOjbect[F("5")] = LAMPPTRGET()->isFaderOn();

    // Serial.printf("effload: %d\n", root[F("effload")].as<uint8_t>());

    // root[F("effload")]    = effLoad ? false : isEffectLoadRequest(OutputSav[FPSTR(ALMLPT_REQUEST)].as<String>());
    oOjbect = root.createNestedObject(FPSTR(ALMLPT_OUTPUTS));
    outputs_toJson(oOjbect, true, false, eff);      

    if (prog) {
      DynamicJsonDocument progRequest(512);
      JsonArray           arr;
      JsonObject          obj;
      arr = progRequest.createNestedArray(FPSTR(ALMLPT_SET));  
      arr = progRequest.createNestedArray(FPSTR(ALMLPT_GET)); 
      arr.add(FPSTR(APPT_REP_005));
      arr.add(FPSTR(APPT_REP_003));
      _AP_Api.parsingRequest(progRequest, doc, "");      
    }


    serializeJson(doc, out);
}
void Device::outputs_effect_toAppi(String & out){
    DynamicJsonDocument doc(3200);
    JsonObject root = doc.to<JsonObject>();
    String ip = WiFi.localIP().toString();
    root[FPSTR(ALMLPT_IP)]= ip;
    root[FPSTR(ALMLPT_DN)]= _name;
    root[FPSTR(ALMLPT_OC)]= _output_count;
    root[FPSTR(ALMLPT_CLI)]= FPSTR(ALMLPT_RGB);
    JsonObject oOjbect = root.createNestedObject(FPSTR(ALMLPT_EFFECT));
    outputs_effect_toJson(oOjbect);      
    serializeJson(doc, out);
}
/**
 * @brief     genre un json pour chaque strip
 * @see       https://github.com/AdriLighting/
 *
 * @author    AdriLighting
 * @date      mar. 28 déc. 2021 16:20:51
 *
 * @param      oject     The oject
 * @param[in]  shortKey  The short key
 * @param[in]  fs        The file system
 */
void Device::outputs_toJson(JsonObject & oject, boolean shortKey, boolean fs, boolean eff){
  for(uint8_t j = 0; j <_output_count; ++j) {
    String pos = String(j);
    JsonObject value = oject.createNestedObject(pos);
    output_toJson(j, value, shortKey, fs, eff);
  }
}

void Device::armoiretofs(uint8_t pos){
  if (!_outputArray[pos]._isArmoire) return;

  File file = LittleFS.open("/outputs/armoire_"+String(pos)+".json", "w");
  if (!file) return;

  DynamicJsonDocument json(3500);

  JsonObject object;
  JsonObject root   = json.to<JsonObject>();  


  object  = root.createNestedObject("select");  
  object[F("0")] = _outputArray[pos]._armoireEtage;

  
    aramoire_segemnts   * ptr;
    aramoire_segemnt  * setSeg  = &_outputArray[pos]._armoire;
    uint8_t maxSeg = 0 ;
    object = root.createNestedObject("etage");
    setSeg->get_segmentCnt(maxSeg);          
    for (int i = 0; i < maxSeg; ++i) {
      ptr = setSeg->get_segment(i);
      object[String(i)] = String(ptr->_c1.r) + "." + String(ptr->_c1.g) + "." + String(ptr->_c1.b) ;
      object["hsv_"+String(i)] = String(ptr->_hsv_h) + "." + String(ptr->_hsv_s) + "." + String(ptr->_hsv_v) ;
      // object["coff_"+String(i)] = String(ptr->_coff.r) + "." + String(ptr->_coff.g) + "." + String(ptr->_coff.b) ;
      // String s1 = "isTw_" + String(i);
      // String s2 = "tw_" + String(i);
      // String s3 = "tw_v_" + String(i);
      object["tw_"+String(i)] = String(ptr->_isTw) + "." + String(ptr->_tw) + "." + String(ptr->_tw_v) ;

      // object[s1] = ptr->_isTw ;
      // object[s2] = ptr->_tw ;
      // object[s3] = ptr->_tw_v ;

    }  

  // fsprintf("\n");
  // serializeJson(json, Serial);
  // fsprintf("\n");
  // 
  serializeJson(json, file);

  file.close();   
}
void Device::output_toSpiff(uint8_t pos, const String & path){
  if (!FILESYSTEM.exists(path)) return;

  char      buf[10];
  String    line      = "";

  CRGB c1 = _outputArray[pos].get_c1();
  // 0, 3, 6
  sprintf(buf, "%03x%03x%03x", c1.r, c1.g, c1.b);                                                                             // c1 
  line += String(buf) + " "; 
  // 10, 13, 16
  c1 = _outputArray[pos].get_coff();
  sprintf(buf, "%03x%03x%03x", c1.r, c1.g, c1.b);                                                                             // coff
  line += String(buf) + " ";
  // 20, 23, 26
  sprintf(buf, "%03x%03x%03x", _outputArray[pos].get_hsv_h(), _outputArray[pos].get_hsv_s(), _outputArray[pos].get_hsv_v()); // hsv
  line += String(buf) + " ";
  // 30, 33, 36
  sprintf(buf, "%03x%03x%03x", _outputArray[pos].get_rgb_r(), _outputArray[pos].get_rgb_g(), _outputArray[pos].get_rgb_b()); // rgb
  line += String(buf) + " ";
  // 40, 43, 46
  sprintf(buf, "%03x%03x%03x", _outputArray[pos].get_tw(), _outputArray[pos].get_isTw(), _outputArray[pos].get_tw_v());     //tw,istw,tw_v
  line += String(buf) + " ";
  // 50, 53
  sprintf(buf, "%03x%03x", _outputArray[pos].get_select(), _outputArray[pos].get_onoff());                                  //select,onoff,
  line += String(buf) + " ";
  // // 57, 60, 63
  // sprintf(buf, "%03x%03x%03x", _outputArray[pos].get_type(), _outputArray[pos].get_pin(), _outputArray[pos].get_size());    //type,pin,size
  // line += String(buf) + " ";
  // 57, 60, 63
  sprintf(buf, "%03x%03x%03x", _effects[pos]->getEn(), _outputArray[pos].get_eff_colorMod(), _outputArray[pos].get_eff_activ());  //eff_nb,eff_colorMod
  line += String(buf) + " ";
  // 67, 70, 73
  RgbColor c2;
  _outputArray[pos].eff_get_c1(c2);
  sprintf(buf, "%03x%03x%03x", c2.R, c2.G, c2.B);                                                                          //eff_c1
  line += String(buf) + " ";  
  // 77, 80, 83
  _outputArray[pos].eff_get_c2(c2);
  sprintf(buf, "%03x%03x%03x", c2.R, c2.G, c2.B);                                                                         //eff_c2
  line += String(buf) + " ";

  sprintf(buf, "%03x", _outputArray[pos].get_briEff());                                 
  line += String(buf) + " ";

  uint8_t linePos = pos+1;
  File f;
  LH_file * _LH_file = new LH_file(f, path.c_str());
  _LH_file->replaceLine(line, linePos);
  delete _LH_file;
}

/**
 * @brief      modue de sauvegarde ver la memoire spiff
 * @see       https://github.com/AdriLighting/
 *
 * @author    AdriLighting
 * @date      mar. 28 déc. 2021 16:20:51
 *  
 */
void Device::outputs_sav(){
  if ((millis() - _timerOutPutsSav) < 1000) // chien de garde
    return;

  // LOG(-1,2,2,"SAV OUTPUT", "");    

  JsonArray outputs = OutputSav[FPSTR(ALMLPT_OUTPUTS)];
  uint8_t pos = outputs.size();
  for (uint8_t i = 0; i < pos; i++) {
    uint8_t s = outputs[i].as<uint8_t>();
    output_toSpiff(s, "/outputs/lamp.txt");
    armoiretofs(s);
    delay(2);    
  }

  OutputSav.clear();
  OutputSav.garbageCollect();

  _timerOutPutsSav = millis();
}


/**
 * @brief     souvegarde to spiff sans passer par le module de sauvegarde
 * @see       https://github.com/AdriLighting/
 *
 * @author    AdriLighting
 * @date      mar. 28 déc. 2021 16:20:51
 *  
 */
void Device::outputs_savForce(){
  if ((millis() - _timerOutPutsSav) < 1200)
    return;

  ALT_TRACEC("main", "-\n");    

  for (uint8_t i = 0; i < _output_count; i++) {
    output_toSpiff(i);
    armoiretofs(i);
    delay(2);
  }
  _timerOutPutsSav = millis();
}

/**
 * @brief     sauvegarde l'etat actuel de tous les strip ver la meme spiff avec un nom de preset
 * @see       https://github.com/AdriLighting/
 *
 * @author    AdriLighting
 * @date      mar. 28 déc. 2021 16:20:51
 *
 * @param     doc   The document
 */
void Device::preset_sav(DynamicJsonDocument & doc) {
  if(!FILESYSTEM.exists(F("/presets"))) {FILESYSTEM.mkdir(F("/presets"));}

  File file;
  String folder;

  String name = doc[F("name")].as<String>();
  String pos  = doc[F("pos")].as<String>();
  if (name!="") name.concat(F("_"));
  name.concat(pos);
  name.concat(F(".txt"));

  folder = "/presets";
  if(!FILESYSTEM.exists(folder)) {FILESYSTEM.mkdir(folder);}
  folder.concat(F("/"));

  String pathStr = folder+name;

  ALT_TRACEC("main", "[preset_sav][folder: %s][name: %s]\n", folder.c_str(), name.c_str());

  if (!FILESYSTEM.exists(folder+name)) {
    File f;
    LH_file * _LH_file = new LH_file(f, pathStr.c_str());
    for(int i=0; i<_output_count; i++) _LH_file->newLine("output_"+String(i));
    delete _LH_file;
  } 
  for (uint8_t i = 0; i < _output_count; i++) {
    output_toSpiff(i, pathStr.c_str());
    delay(1);
  }  

  doc.clear(); doc.garbageCollect();
  outputs_savForce();
}
void Device::preset_load(DynamicJsonDocument & doc) {
  if(!FILESYSTEM.exists(F("/presets"))) {FILESYSTEM.mkdir(F("/presets"));}

  String name = doc[F("name")].as<String>();
  String pos  = doc[F("pos")].as<String>();
  if (name!="") name.concat(F("_"));
  name.concat(pos);
  name.concat(F(".txt"));
  String pathStr = "/presets/";
  pathStr.concat(name);

  if(!FILESYSTEM.exists(pathStr)) return;

  ALT_TRACEC("main", "[preset_load][pathStr: %s]\n", pathStr.c_str());

  for (uint8_t i = 0; i < _output_count; i++) {
    _outputArray[i].setValueFromSpiff(pathStr);
  }  

  doc.clear(); doc.garbageCollect();
  outputs_savForce();
}


void Device::bp_onoff(){
  DynamicJsonDocument doc(255);
  JsonObject root = doc.to<JsonObject>();
  JsonObject oSel = root.createNestedObject(FPSTR(req_s_output));  
  oSel[F("v")] = 1;
  for(int i=0; i<_output_count; i++){
    get_outputArray(i)->set_toggle(FPSTR(req_toglle), oSel);  
  }
  doc.clear();
  doc.garbageCollect(); 
  outputs_savForce();
  // DeviceserverPtrGet()->udpMultiUpdate();
}
void Device::bp_onoff(uint8_t value){
  DynamicJsonDocument doc(255);
  JsonObject root = doc.to<JsonObject>();
  JsonObject oSel = root.createNestedObject(FPSTR(req_s_output));  
  oSel[F("v")] = value;
  for(int i=0; i<_output_count; i++){
    get_outputArray(i)->set_onoff(FPSTR(req_lampOnOff), oSel);  
  }
  doc.clear();
  doc.garbageCollect(); 
  outputs_savForce();
  // DeviceserverPtrGet()->udpMultiUpdate();
}
void Device::bp_briLoop(){
  for(int i=0; i<_output_count; i++){
    get_outputArray(i)->set_briLoop();  
  }
}

void Device::restart_requiered() {
  _task_reset->set_callbackOstart([](){/*Serial.println("-");*/ ESP.restart();});
  _task_reset->set_taskDelay(ETD::ETD_DELAY, true, 5, 2);
  _task_reset->set_taskDelayEnabled(ETD::ETD_DELAY, true);
  _task_reset->set_iteration_max(0);
  _task_reset->set_enabled(true);     
}
void Device::format(uint8_t mod){

  _TaskScheduler->get_task(_output_count+1)->set_enabled(false);  
  _TaskScheduler->get_task(_output_count+2)->set_enabled(false);  
  _TaskScheduler->get_task(_output_count+3)->set_enabled(false);  
  #ifdef ADS_PIN_IR
  _TaskScheduler->get_task(_output_count+4)->set_enabled(false);    
  #endif

  for (uint8_t i = 0; i < _output_count; i++) {
    _TaskScheduler->get_task(i)->set_enabled(false);   
  }

  Serial.printf_P(PSTR("FORMAT BEGIN\n"));
  
  switch (mod) {
    case 0: al_tools::SPIFFS_deleteRecursive(FILESYSTEM, "/outputs"); break;
    case 1: al_tools::SPIFFS_deleteRecursive(FILESYSTEM, "/eff"); break;
    case 2: FILESYSTEM.remove("/wcevo_config.json"); break;
    case 3: FILESYSTEM.format(); break;
    default:break;
  }



}
