#include "../../include/device/device.h"



#include "../../include/fastled/fastled.h"
#include "../../include/pixel/effectcore.h"
#include "../../include/display/lamp.h"
#include "../../include/alml_webserver.h"
#include "../../include/tft/tft.h"
#include "../../include/tft/tfttxt.h"

#include <wificonnectevo.h>

extern AsyncWebServer web_server;

alml_taskSheduler _alml_taskSheduler;


// String& DeviceSave_item::operator=(const String& source)
// {
//     if (_request) delete _request;
//     int len = source.length();
//     _request = new char[len+1];
//     strcpy(_request, source.c_str());

//     String s = String(_request);
//     return s;
// }


  DeviceSave_item::DeviceSave_item(boolean s, uint8_t p, const char * r){
    _succes   = s;
    _pin      = p;
    * this    = r;

  };
  DeviceSave_item::~DeviceSave_item(){
    if (_request) delete _request;
  };

  DeviceSave_item &DeviceSave_item::operator = (const char * const & other) {
    if (_request) delete _request;
    _request = new char[strlen(other)+1];
    strcpy(_request, other);
    return *this;
  } 
  DeviceSave_item &DeviceSave_item::operator = (uint8_t const & other) {
    _pin = other;
    return *this;
  } 
  void DeviceSave_item::get_request(const char * & ret) {
    ret = _request;
  } 
  uint8_t DeviceSave_item::get_pin(){
    return _pin;
  } 
  boolean DeviceSave_item::get_succes(){
    return _succes;
  } 

  DeviceSave::DeviceSave(){};
  DeviceSave::~DeviceSave(){
    while (_currentList.size()) {
      DeviceSave_item *ptr = _currentList.shift();
      delete ptr;
    }
    _currentList.clear(); 
    while (_lastList.size()) {
      DeviceSave_item *ptr = _lastList.shift();
      delete ptr;
    }
    _lastList.clear();      
  };
  DeviceSave_item * DeviceSave::get_item(boolean dir, uint8_t p) {return get_list(dir)[p];}
  LList<DeviceSave_item*>& DeviceSave::get_list(boolean dir) {
    if (dir)
      return _currentList;
    else
      return _lastList;
  }

  void DeviceSave::new_req(boolean s, uint8_t p, const char * r){

    ALT_TRACEC("main", "statu: %d - pin: %d - request: %s -> ", s, p, r);   

    size_t size = _currentList.size();

    boolean add = true;

    // if (size == 0) add = false;

    for(size_t i = 0; i < size; ++i) {
      DeviceSave_item * item  =  _currentList.get(i);
      boolean         succes  = item->get_succes();
      uint8_t         pin     = item->get_pin();
      const char      * name  = "";  
      item->get_request(name);

      if (pin == p && succes == s && (strcmp(name, r) == 0)) add = false;
    }

    if (add) _currentList.add(new DeviceSave_item(s, p, r));

    #ifdef ALT_DEBUG_TARCE
      if (ALT_debugPrint("main")) Serial.printf_P(PSTR("%d\n"), add);  
    #endif

  }

  const String DeviceSave::get_lastRequest() {

    size_t size = _lastList.size();

    if (size == 0) return String("");

    DeviceSave_item * ptr = _lastList.get(size-1);

    const char * name = "";

    ptr->get_request(name);

    return String(name);
  }


  void DeviceSave::sav(){

    while (_lastList.size()) {
      DeviceSave_item *ptr = _lastList.shift();
      delete ptr;
    }
    _lastList.clear();


    size_t size = _currentList.size();
    for(size_t i = 0; i < size; ++i) {
      DeviceSave_item * item  =  _currentList.get(i);
      boolean         succes  = item->get_succes();
      uint8_t         pin     = item->get_pin();
      const char      * name  = "";
      item->get_request(name);

      _lastList.add(new DeviceSave_item(succes, pin, name));
     
      ALT_TRACEC("main", "&c:1&s:\tstatu: %d - pin: %d - request: %s -> ", succes, pin, name);   

      if (!succes) {
        #ifdef ALT_DEBUG_TARCE
          if (ALT_debugPrint("main")) Serial.println(F("Not SAV"));  
        #endif        
        continue;
      }

      #ifdef ALT_DEBUG_TARCE
        if (ALT_debugPrint("main")) Serial.println(F("SAV"));  
      #endif  

      DevicePtrGet()->output_toSpiff(pin, "/outputs/lamp.txt");
      DevicePtrGet()->armoiretofs(pin);

      delay(0);       
    }

    while (_currentList.size()) {
      DeviceSave_item *ptr = _currentList.shift();
      delete ptr;
    }
    _currentList.clear();    
  }

DeviceSave _DeviceSave;




#ifdef ALML_TFT
  Tft_txt * tft_output_1 = nullptr;
  Tft_txt * tft_output_2 = nullptr;
  Tft_txt * tft_output_3 = nullptr;
  Tft_txt * tft_output_4 = nullptr;
  EffectValue _eff_bri ;
  EffectValue _eff_speed ;
  EffectValue _eff_scale ;
#endif

namespace {
  String  _string_to_split(String name, String value, String sep){
      return name + sep + value;
  }
  String  _string_to_split(String name, String value){
      return name + ";" + value + "\n";
  }   


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
  #ifdef DEBUG_KEYBOARD  
    void taskCb_serial(){
      _Sr_menu.serialRead();  
    }
  #endif
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

  void taskSetup(uint8_t tp, uint32_t d, TaskSetFunction_t f){
      _alml_taskSheduler.get_task(tp)->set_callback(f); 
      _alml_taskSheduler.get_task(tp)->set_taskDelay(ETD::ETD_TIMER, true, d);
      _alml_taskSheduler.get_task(tp)->set_iteration_max(-1);
      _alml_taskSheduler.get_task(tp)->setup(true);
      _alml_taskSheduler.get_task(tp)->set_enabled(true);   
  }


}



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


StaticJsonDocument<1024> DeviceUserConfig;

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
// StaticJsonDocument<200> OutputSav;

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


  if (DeviceUserConfig.containsKey(F("segment"))) {

    uint8_t segment_op = DeviceUserConfig[F("segment")][F("op")].as<uint8_t>();

    Serial.printf_P(PSTR("segment initialize on pin: %d\n"), segment_op);

    JsonArray segmentArray = DeviceUserConfig[F("segment")][F("items")].as<JsonArray>();
    for (JsonObject item : segmentArray) {
      uint8_t s = item[F("s")].as<uint8_t>();
      uint8_t e = item[F("e")].as<uint8_t>();
      bool    d = item[F("d")].as<bool>(); 
      Serial.printf_P(PSTR("\ts: %d e: %d d: %d\n"), s, e, d);
      get_outputArray(segment_op)->_leds_segment.segment_new(s, e, d);
    }
  }



  // get_outputArray(0)->_leds_segment.segment_new(0, 63,  false);
  // get_outputArray(0)->_leds_segment.segment_new(64, 127, true);

  if (DeviceUserConfig.containsKey(F("segment_armoire"))) {

    uint8_t segment_op = DeviceUserConfig[F("segment_armoire")][F("op")].as<uint8_t>();

    Serial.printf_P(PSTR("segment_armoire initialize on pin: %d\n"), segment_op);

    int     arm_left      = (128/2);
    int     arm_right     = (128/2);
    int     mod_left      = arm_left % 4;
    int     mod_right     = arm_right % 4;
    uint8_t arm_div_left  = arm_left / 4;
    uint8_t arm_div_right = arm_right / 4;
    get_outputArray(segment_op)->_armoire.segemnt_new(0,                             arm_div_left-1,                               false,  aramoire_side_left  );
    get_outputArray(segment_op)->_armoire.segemnt_new(arm_left  + (arm_div_right*3), arm_left + (((arm_div_right*4)-1)+mod_right), true,   aramoire_side_right );
    get_outputArray(segment_op)->_armoire.segemnt_new(arm_div_left,                  (arm_div_left*2)-1,                           false,  aramoire_side_left  );
    get_outputArray(segment_op)->_armoire.segemnt_new(arm_left + (arm_div_right*2),   arm_left + ((arm_div_right*3)-1),            true,   aramoire_side_right );
    get_outputArray(segment_op)->_armoire.segemnt_new(arm_div_left*2,                (arm_div_left*3)-1,                           false,  aramoire_side_left  );
    get_outputArray(segment_op)->_armoire.segemnt_new(arm_left + arm_div_right,      arm_left + ((arm_div_right*2)-1),             true,   aramoire_side_right );
    get_outputArray(segment_op)->_armoire.segemnt_new(arm_div_left*3,                ((arm_div_left*4)-1)+mod_left,                false,  aramoire_side_left  );
    get_outputArray(segment_op)->_armoire.segemnt_new(arm_left,                      arm_left + (arm_div_right-1),                 true,   aramoire_side_right );  
    Serial.printf_P(PSTR("[mod_left][%d]"), mod_left);
    Serial.printf_P(PSTR("[mod_right][%d]"), mod_right);
    Serial.printf_P(PSTR("[arm_div_left][%d]"), arm_div_left);
    Serial.printf_P(PSTR("[arm_div_right][%d]\n"), arm_div_right);
  }


  ALT_TRACEC("main", "LAMP new instance\n");
  new LAMP(0);
  delay(2);

  ALT_TRACEC("main", "RemoteControl new instance\n");  
  new RemoteControl();

  ALT_TRACEC("main", "al_taskScheduler new instance\n");
  for (int i = 0; i < _output_count; ++i) {
  _alml_taskSheduler.get_listItem().add(new alml_taskItem(nullptr,          0,       TASKNAME_OUTPUT,    TASKLEVEL_ALLWAYS));               // 0
  }
  #ifdef ADS_PIN_IR
  _alml_taskSheduler.get_listItem().add(new alml_taskItem(taskCb_ir,        50000,   TASKNAME_IR,        TASKLEVEL_ALLWAYS, taskSetup));    // 1
  #endif
  _alml_taskSheduler.get_listItem().add(new alml_taskItem(taskCb_udp,       100000,  TASKNAME_UDP,       TASKLEVEL_ALLWAYS, taskSetup));    // 2
  _alml_taskSheduler.get_listItem().add(new alml_taskItem(taskCb_lamp,      50000,   TASKNAME_LAMP,      TASKLEVEL_ALLWAYS, taskSetup));    // 3
  _alml_taskSheduler.get_listItem().add(new alml_taskItem(taskCb_program,   100000,  TASKNAME_PROGRAM,   TASKLEVEL_ALLWAYS, taskSetup));    // 4
  _alml_taskSheduler.get_listItem().add(new alml_taskItem(taskCb_wifi,      50000,  TASKNAME_WIFI,      TASKLEVEL_ALLWAYS, taskSetup));    // 5
  _alml_taskSheduler.get_listItem().add(new alml_taskItem(taskCb_webserver, 100000,  TASKNAME_WEBSERVER, TASKLEVEL_ALLWAYS, taskSetup));    // 6
  #ifdef DEBUG_KEYBOARD
  _alml_taskSheduler.get_listItem().add(new alml_taskItem(taskCb_serial,    50000,    TASKNAME_SERIAL,    TASKLEVEL_ALLWAYS, taskSetup));    // 7  
  #endif
  for(int i = 0; i < _alml_taskSheduler.get_listItem().size(); ++i) {
    _alml_taskSheduler.get_list().add(new TaskSimple());
    alml_taskItem * item = _alml_taskSheduler.get_listItem()[i];
    item->setup(i);
  }
  _alml_taskSheduler.set_debug_byTaskName(TASKNAME_OUTPUT, false);  
  _alml_taskSheduler.set_name_byTaskName(TASKNAME_OUTPUT,     "OUTPUT");  
  #ifdef ADS_PIN_IR
  _alml_taskSheduler.set_name_byTaskName(TASKNAME_IR,         "IR");    
  #endif
  _alml_taskSheduler.set_name_byTaskName(TASKNAME_UDP,        "UDP");  
  _alml_taskSheduler.set_name_byTaskName(TASKNAME_LAMP,       "LAMP");  
  _alml_taskSheduler.set_name_byTaskName(TASKNAME_PROGRAM,    "PROGRAM");  
  _alml_taskSheduler.set_name_byTaskName(TASKNAME_WIFI,       "WIFI");  
  _alml_taskSheduler.set_name_byTaskName(TASKNAME_WEBSERVER,  "WEBSERVER");  
  #ifdef DEBUG_KEYBOARD
   _alml_taskSheduler.set_name_byTaskName(TASKNAME_SERIAL,     "SERIAL");   
   #endif 

  WCEVO_managerPtrGet()->set_cb_webserveAprEvent([](){
    // Serial.printf_P(PSTR("set_cb_webserveAprEvent\n"));
    if (RemoteControlPtrGet()) RemoteControlPtrGet()->begin();
  });  
  WCEVO_managerPtrGet()->set_cb_serverEvent([](){
    // Serial.printf_P(PSTR("set_cb_serverEvent\n"));
    if (RemoteControlPtrGet()) RemoteControlPtrGet()->begin();
  });
  WCEVO_managerPtrGet()->set_cb_webserverOn([](){
    // Serial.printf_P(PSTR("set_cb_webserverOn\n"));
    _Webserver.setup(); 
  });
  WCEVO_managerPtrGet()->set_cb_webserveAprOn([](){
    // Serial.printf_P(PSTR("set_cb_webserveAprOn\n"));
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



  // delay(2);
  
  for(uint8_t i=0; i< _output_count; i++) {
    LAMPPTRGET()->effectsTimer(i, T_ENABLE);
  }
  
#ifdef ALML_TFT

  tft_output_1 = new Tft_txt(&_almlTft, _almlU8g2);
  tft_output_1->textColor(C_YELLOW, C_BLACK);
  tft_output_1->u8g2_mod(1);
  tft_output_1->u8g2_font(u8g2_font_helvR14_tf);
  tft_output_1->textPos(105, 115);
  tft_output_1->text("tft_output_1");
  tft_output_1->u8g2_setup();

  tft_output_2 = new Tft_txt(&_almlTft, _almlU8g2);
  tft_output_2->textColor(C_YELLOW, C_BLACK);
  tft_output_2->u8g2_mod(1);
  tft_output_2->u8g2_font(u8g2_font_helvR14_tf);
  tft_output_2->textPos(155, 140);
  tft_output_2->text("tft_output_2");
  tft_output_2->u8g2_setup();

  tft_output_3 = new Tft_txt(&_almlTft, _almlU8g2);
  tft_output_3->textColor(C_YELLOW, C_BLACK);
  tft_output_3->u8g2_mod(1);
  tft_output_3->u8g2_font(u8g2_font_helvR14_tf);
  tft_output_3->textPos(155, 160);
  tft_output_3->text("tft_output_3");
  tft_output_3->u8g2_setup();

  tft_output_4 = new Tft_txt(&_almlTft, _almlU8g2);
  tft_output_4->textColor(C_YELLOW, C_BLACK);
  tft_output_4->u8g2_mod(1);
  tft_output_4->u8g2_font(u8g2_font_helvR14_tf);
  tft_output_4->textPos(155, 180);
  tft_output_4->text("tft_output_4");
  tft_output_4->u8g2_setup();
#endif

    // get_outputArray(0)->_armoire.segemnt_leftRight((arm_div_left - mod_left)-1);
    // get_outputArray(0)->set_color(0, CRGB(0, 255, 0), 80, 255, 255);
// FastLED.show();
    // 
  testTft(0);

  ALT_TRACEC("main", "--\n");
}

alml_taskSheduler * Device::get_taskSheduler() {return &_alml_taskSheduler;}
TaskSimple * Device::get_task(uint8_t p) {return _alml_taskSheduler.get_task(p);}


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
  // _TaskScheduler->loop();
  _alml_taskSheduler.loop();
  if(_task_reset) {if (_task_reset->isEnabled()) _task_reset->execute();}
  if ( WCEVO_managerPtrGet()->updateModReady() ){
    #ifdef ALML_TFT
    wcevo_updateMod_t mod = WCEVO_managerPtrGet()->get_updateMod();  
    #endif
    WCEVO_managerPtrGet()->set_updateModEnd() ;
    #ifdef ALML_TFT
      _almlTft.fillRect(
          0, 
          0, 
          _almlTft.width(), 
          70, 
          ILI9341_BLACK
        ); 
      Tft_txt temp_2(&_almlTft, _almlU8g2);
      const char * value;
      switch (mod) {
          case wcevo_updateMod_t::WCEVO_UM_STA  : 
            ALSYSINFO_getValByKey(ALSI_STAIP, value);
            temp_2.drawString(5, 30, "La lampe est connecter au routeur", _almlTft.color565(0, 255, 0),    &SansSerif_plain_14);
            temp_2.drawString(5, 50, "L'adresse de la lampe est: " + al_tools::ch_toString(value), _almlTft.color565(255, 255, 255),  &SansSerif_plain_13);
          break;
          case wcevo_updateMod_t::WCEVO_UM_STAAP: 
            ALSYSINFO_getValByKey(ALSI_STAIP, value);
            temp_2.drawString(5, 30, "La lampe est connecter au routeur+ap", _almlTft.color565(0, 255, 0),    &SansSerif_plain_14);
            temp_2.drawString(5, 50, "L'adresse de la lampe est: " + al_tools::ch_toString(value), _almlTft.color565(255, 255, 255),  &SansSerif_plain_13);
          break;
          case wcevo_updateMod_t::WCEVO_UM_AP   : 
            temp_2.drawString(5, 30, "La lampe est en point d'acces", _almlTft.color565(0, 255, 0), &SansSerif_plain_14);
            temp_2.drawString(5, 50, "L'adresse de la lampe est: 8.8.8.8", _almlTft.color565(255, 255, 255),  &SansSerif_plain_13);
          break;
          default:break;
      }
    #endif      
    
  }
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
  if (cmd == FPSTR(req_eff_activ))     return true; 
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
  String req = _DeviceSave.get_lastRequest();
  return isEffectRequest(req);
  // return true;
}
String Device::get_lastRequest() const {
  // return OutputSav[FPSTR(ALMLPT_REQUEST)].as<String>();
  return _DeviceSave.get_lastRequest();
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
boolean Output_set_eff (uint8_t sP, String &result, const String & cmd, const JsonObject & value){
  if (!ProgramPtrGet()) return false;
  if ((cmd == FPSTR(req_eff_next)))       {DevicePtrGet()->get_outputArray(sP)->set_effActiv(true);ProgramPtrGet()->set_itemNext();ProgramPtrGet()->get_itemBase(result);return true;}
  else if ((cmd == FPSTR(req_eff_prev)))  {DevicePtrGet()->get_outputArray(sP)->set_effActiv(true);ProgramPtrGet()->set_itemPrev();ProgramPtrGet()->get_itemBase(result);return true;} 
  else if ((cmd == FPSTR(req_eff_load))) {
    result = value[F("v")].as<String>();
    return true;
  } 
  return false;   
}
boolean Output_set_eff_f(uint8_t sP, String &result, const String & cmd, const JsonObject & value){
  if (!ProgramPtrGet()) return false;
  if (cmd != FPSTR(req_eff_f)) return false;

  String action = value[F("c")].as<String>();
  if ((action == FPSTR(req_eff_next)))       {DevicePtrGet()->get_outputArray(sP)->set_effActiv(true);ProgramPtrGet()->set_itemNext();ProgramPtrGet()->get_itemBase(result);return true;}
  else if ((action == FPSTR(req_eff_prev)))  {DevicePtrGet()->get_outputArray(sP)->set_effActiv(true);ProgramPtrGet()->set_itemPrev();ProgramPtrGet()->get_itemBase(result);return true;} 
  else if ((action == FPSTR(req_eff_load))) {
    DevicePtrGet()->get_outputArray(sP)->set_effActiv(true);
    uint8_t p = value[F("v")].as<uint8_t>();
    ProgramPtrGet()->set_itemById(p);
    ProgramPtrGet()->get_itemBase(result);
    return true;
  } 
  return false;
}

boolean Device::parseJson_output(uint8_t p , String & nextEffect, const String & req, JsonObject val) {

  if (get_outputArray(p)->set_select(req, val))           return true;
  
  if (!get_outputArray(p)->get_select()) return false;

  if (get_outputArray(p)->set_onoff(req, val))            return true;
  if (get_outputArray(p)->set_toggle(req, val))           return true;

  if (!get_outputArray(p)->get_onoff()) return false; 

  if (get_outputArray(p)->set_etage(req, val))            return true;
  if (get_outputArray(p)->set_bri(req, val))              return true;
  if (get_outputArray(p)->set_briIncr(req, val))          return true;
  if (get_outputArray(p)->set_hsv_hIncr(req, val))        return true;

  if (get_outputArray(p)->set_rgb_f(req, val))            return true;
  if (get_outputArray(p)->set_tw_f(req, val))             return true;

  if (get_outputArray(p)->set_eff_activ(req, val))        return true;
  // get_outputArray(p)->set_eff_f(req, val);
  if (get_outputArray(p)->set_eff_autoplay(req, val))     return true;


  if (nextEffect=="") {
    // if (Output_set_eff_f(p, nextEffect, req, val))        return true;
    Output_set_eff_f(p, nextEffect, req, val);
  } 

  if (get_outputArray(p)->get_eff_activ()) {

    if (LAMPPTRGET()->effectsLoop_Timer(req, val))        return true;
    if (get_outputArray(p)->set_eff_colorMod(req, val))   return true;
    if (get_outputArray(p)->set_eff_id(req, val))         return true;
    if (get_outputArray(p)->set_eff_rgb1(req, val))       return true;
    if (get_outputArray(p)->set_eff_bri(req, val))        return true;   
    if (get_outputArray(p)->set_eff_scale(req, val))      return true;   
    if (get_outputArray(p)->set_eff_speed(req, val))      return true;   
    if (get_outputArray(p)->set_eff_speedIncr(req, val))  return true;   

    // changemant unique de l'effet (ne doit sexcuter qu'une fois dansl le loop)
    if (nextEffect=="") {
      // if (Output_set_eff(p, nextEffect, req, val))        return true;
      Output_set_eff(p, nextEffect, req, val);
    } 

    // chargement de l'effet pour chaque strip        
    if (nextEffect!="") {
      for (uint8_t j = 0; j < _effeNamIDList_cnt; ++j) {
        String find = FPSTR(_effeNamIDList[j]._nameId);
        if (find == nextEffect) {
          LAMPPTRGET()->effectChangeByPos(p, j, LAMPPTRGET()->isFaderOn(), true);
          return true;
        }
      }
    }

    return false;
  }

  if (!isColorRequest(req)) return false;
  if (get_outputArray(p)->set_rgb(req, val))            return true;
  if (get_outputArray(p)->set_hsv_h(req, val))          return true;
  if (get_outputArray(p)->set_hsv_s(req, val))          return true;
  if (get_outputArray(p)->set_hsv_v(req, val))          return true;
  if (get_outputArray(p)->set_tw(req, val))             return true;      
  if (get_outputArray(p)->set_tw_v(req, val))           return true; 

  return false;   
}




// tft_output_2 = new Tft_txt(&_almlTft, _almlU8g2);
// tft_output_2->canvas_create(10, 20, 40, 20);
// tft_output_2->text_font(&SansSerif_plain_14);
// tft_output_2->textColor(C_WHITE, 0);
// 


void Device::testTft(int8_t sp){
#ifdef ALML_TFT
  static  boolean       lastState_1 = true;
  static  boolean       lastState_2 = true;
  static  String        effName     = "";
          Output        * ptr       = get_outputArray(sp);
          EffectWorker  * eff       = _effects[sp];
          Tft_txt       temp_2(&_almlTft, _almlU8g2);  

  if (lastState_1 != ptr->get_eff_activ()) {
    _almlTft.fillRect(
        0, 
        80, 
        _almlTft.width(), 
        _almlTft.height()-80, 
        ILI9341_BLACK
      );
    lastState_1 = ptr->get_eff_activ();
    effName = "";
  }
  if (lastState_2 != ptr->get_onoff()) {
    _almlTft.fillRect(
        0, 
        80, 
        _almlTft.width(), 
        _almlTft.height()-80, 
        ILI9341_BLACK
      );
    lastState_2 = ptr->get_onoff();
    effName = "";
  }

  _almlTft.drawBitmap(5, 80, lightBitmap, 40, 40, ptr->get_onoff() ? _almlTft.color565(0,255,0) : _almlTft.color565(255,0,0));

  if (ptr->get_onoff()) {
    if (ptr->get_eff_activ()) {

      boolean updBri    = false;
      boolean updSpeed  = false;
      boolean updScale  = false;

      if (eff->worker) {
        if (_eff_bri._value != eff->getControls()[0]->getVal().toInt()) updBri = true;
        _eff_bri._value     = eff->getControls()[0]->getVal().toInt();
        _eff_bri._state     = eff->getControls()[0]->getTypeState();

        if (_eff_speed._value != eff->getControls()[1]->getVal().toInt()) updSpeed = true;
        _eff_speed._value   = eff->getControls()[1]->getVal().toInt();
        _eff_speed._state   = eff->getControls()[1]->getTypeState();

        if (_eff_scale._value != eff->getControls()[2]->getVal().toInt()) updScale = true;
        _eff_scale._value   = eff->getControls()[2]->getVal().toInt();
        _eff_scale._state   = eff->getControls()[2]->getTypeState();
      }

      _almlTft.drawBitmap(55, 80, epd_bitmap, 40, 40, _almlTft.color565(255,255,255));
      temp_2.drawString(105, 95, "Effet en cour:", _almlTft.color565(255, 255, 255), &SansSerif_plain_12);
      if (tft_output_1 && eff->worker && effName != eff->getEffectName()) {
        effName = eff->getEffectName();
        tft_output_1->textColor(C_WHITE, C_BLACK);
        tft_output_1->u8g2_setup();
        tft_output_1->u8g2_drawRect( eff->getEffectName() );
      }

      if (tft_output_2 && tft_output_3 && tft_output_4 && eff->worker) {
        if (_eff_bri._state == 0) {
          if (updBri) temp_2.u8g2_draw(5, 140, FPSTR(ALMLPT_LEDS_005), _almlTft.color565(255,255,255), u8g2_font_helvR14_tf);
          if (updBri) tft_output_2->u8g2_drawRect( String(_eff_bri._value) );       
        } else {
          _almlTft.fillRect(0, 123, _almlTft.width(), 18, ILI9341_BLACK);
        }
        if (_eff_speed._state == 0) {
          if (updSpeed) temp_2.u8g2_draw(5, 160, FPSTR(ALMLPT_LEDS_006), _almlTft.color565(255,255,255), u8g2_font_helvR14_tf);
          if (updSpeed) tft_output_3->u8g2_drawRect( String(_eff_speed._value) );      
        } else {
          _almlTft.fillRect(0, 143, _almlTft.width(), 18, ILI9341_BLACK);
        }
        if (_eff_scale._state == 0) {
          if (updScale) temp_2.u8g2_draw(5, 180, FPSTR(ALMLPT_LEDS_007), _almlTft.color565(255,255,255), u8g2_font_helvR14_tf);
          if (updScale) tft_output_4->u8g2_drawRect( String(_eff_scale._value) );        
        } else {
          _almlTft.fillRect(0, 163, _almlTft.width(), 18, ILI9341_BLACK);
        }
      }
    }else{
      CRGB newColor = CHSV(ptr->get_hsv_h(), ptr->get_hsv_s(), 255);
      _almlTft.drawBitmap(55, 80, rgbsBitmap, 40, 40, (!ptr->get_isTw()) ? _almlTft.color565(newColor.r, newColor.g, newColor.b) : _almlTft.color565(255, 255, 255));
      temp_2.u8g2_draw(105, 115, FPSTR(ALMLPT_LEDS_009), (!ptr->get_isTw()) ? _almlTft.color565(newColor.r, newColor.g, newColor.b) : _almlTft.color565(255, 255, 255), u8g2_font_helvR14_tf);

      temp_2.u8g2_draw(5, 140, FPSTR(ALMLPT_LEDS_005), _almlTft.color565(255,255,255), u8g2_font_helvR14_tf);
      tft_output_2->u8g2_drawRect( String(!ptr->get_isTw()?ptr->get_hsv_v():ptr->get_tw()));

      (!ptr->get_isTw()) ? temp_2.u8g2_draw(5, 160, FPSTR(ALMLPT_LEDS_008), _almlTft.color565(255,255,255), u8g2_font_helvR14_tf) : _almlTft.fillRect(0, 143, _almlTft.width(), 18, ILI9341_BLACK);
      if (!ptr->get_isTw()) tft_output_3->u8g2_drawRect( String(ptr->get_hsv_h()));
    }
  } else {
      effName = "";
      _eff_bri._value   = -1;
      _eff_speed._value = -1;
      _eff_scale._value = -1;
      _almlTft.fillRect(
          5, 
          140, 
          _almlTft.width(), 
          _almlTft.height()-140, 
          ILI9341_BLACK
        );      
  }
#endif   
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

  String nextEffect = "";
  Output_set_eff(pos, nextEffect, req, val);

  boolean savRequest = parseJson_output(pos, nextEffect, req, val);
  _DeviceSave.new_req(savRequest, pos, req.c_str());
  if (savRequest) testTft(pos);
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
  boolean result      = true;
  String  nextEffect  = "";
  boolean savRequest  = false;
  int8_t  update      = -1;


  JsonArray reqArr = doc[F("A")].as<JsonArray>();
  if (!reqArr.isNull()) {
    Serial.printf_P(PSTR("JSON START ARRAY\n"));

    for (JsonObject obj : reqArr) {
      String      req     = obj[FPSTR(ALMLPT_REQ)].as<String>();
      JsonObject  val     = obj[FPSTR(ALMLPT_VAL)]; 
      for(int i=0; i<_output_count; i++){
        savRequest = parseJson_output(i, nextEffect, req, val);
        _DeviceSave.new_req(savRequest, i, req.c_str());
        if (savRequest) update = i;
      }
    } 

    if (update>=0) testTft(update);

    return;
  }


  if (!doc.containsKey(FPSTR(ALMLPT_REQ))) result = false; 
  if (!doc.containsKey(FPSTR(ALMLPT_VAL))) result = false; 
  if (!result) {return;}

  String      req = doc[FPSTR(ALMLPT_REQ)].as<String>();
  JsonObject  val = doc[FPSTR(ALMLPT_VAL)];  

  for(int i=0; i<_output_count; i++){
    savRequest = parseJson_output(i, nextEffect, req, val);
    _DeviceSave.new_req(savRequest, i, req.c_str());
    if (savRequest) update = i;
  }
  if (update>=0) testTft(update);

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

  String      req         = doc[FPSTR(ALMLPT_REQ)].as<String>();
  JsonObject  val         = doc[FPSTR(ALMLPT_VAL)];
  JsonArray   list_dn     = doc[FPSTR(ALMLPT_A)];
  String      thisDn      = al_tools::ch_toString(_name);
  String      nextEffect  = "";
  int8_t      update      = -1;
  boolean     savRequest  = false;

  for (JsonObject item : list_dn) {

    if (!item.containsKey(FPSTR(ALMLPT_DN))) continue;

    const char * dn = item [FPSTR(ALMLPT_DN)];
    if (al_tools::ch_toString(dn) != thisDn) continue;

    if (doc.containsKey(FPSTR(ALMLPT_API_OP))) {
      String almlapiReponse="";
      _Webserver.device_api(doc, almlapiReponse);  
    }

    if (!item.containsKey(FPSTR(ALMLPT_A))) continue;
    
    JsonArray list_output = item[FPSTR(ALMLPT_A)].as<JsonArray>();

    for (uint8_t item_2 : list_output) {
      savRequest = parseJson_output(item_2, nextEffect, req, val);
      _DeviceSave.new_req(savRequest, item_2, req.c_str());
      if (savRequest) update = item_2;      
    }

  }

  if (update>=0) testTft(update);

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
  if (!shortKey) {
    String s;

    value[FPSTR(req_lampHue)]     = _outputArray[pos].get_hsv_h();
    value[FPSTR(req_lampSat)]     = _outputArray[pos].get_hsv_s();
    value[FPSTR(req_lampBri)]     = _outputArray[pos].get_hsv_v();
    s=FPSTR(req_lampSetColor);
    s.concat(" R");
    value[s]                      = _outputArray[pos].get_rgb_r();
    s=FPSTR(req_lampSetColor);
    s.concat(" G");    
    value[s]                      = _outputArray[pos].get_rgb_g();
    s=FPSTR(req_lampSetColor);
    s.concat(" B");      
    value[s]                      = _outputArray[pos].get_rgb_b();
    value[FPSTR(req_lampWhite)]   = _outputArray[pos].get_tw();
    value[FPSTR(req_lampWhite)]   = _outputArray[pos].get_isTw();
    value[F("bri")]               = _outputArray[pos].get_bri();
    value[FPSTR(req_s_output)]    = _outputArray[pos].get_select();
    value[FPSTR(req_lampOnOff)]   = _outputArray[pos].get_onoff();
    value[F("type")]              = _outputArray[pos].get_type();
    value[F("pin")]               = _outputArray[pos].get_pin();
    value[F("size")]              = _outputArray[pos].get_size();
    value[F("name")]              = _outputArray[pos].get_name();   
    value[FPSTR(req_lampWhite_v)] = _outputArray[pos].get_tw_v();      
    value[FPSTR(req_eff_activ)]   = _outputArray[pos].get_eff_activ(); 
    if (_effects[pos]) {
    value[F("getEn")]             = _effects[pos]->getEn();                       
    }
    // value[F("19")]          = _outputArray[pos].get_isTw() ? _outputArray[pos].get_tw() : _outputArray[pos].get_hsv_v();
    if (_outputArray[pos].get_eff_activ()) value[FPSTR(req_lampBri)] = _effects[pos]->getControls()[0]->getVal(); 
    else { value[FPSTR(req_lampBri)] =  _outputArray[pos].get_isTw() ? _outputArray[pos].get_tw() : _outputArray[pos].get_hsv_v();}    
    
    value[FPSTR(req_etage)]       = _outputArray[pos]._armoireEtage; 
    value[F("isArmoire")]         = _outputArray[pos]._isArmoire;         
  }else{
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
  }
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
        // uint8_t p = isEffectLoadRequest(OutputSav[FPSTR(ALMLPT_REQUEST)].as<String>()) ? 1 : 0;
        root[F("effload")] = 0;
        _effLoad = 0;

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
    object["tw_"+String(i)] = String(ptr->_isTw) + "." + String(ptr->_tw) + "." + String(ptr->_tw_v) ;
  }  

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

  ALT_TRACEC("main", "-\n"); 

  _DeviceSave.sav();

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
    delay(0);
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

  if(!FILESYSTEM.exists(pathStr)) {ALT_TRACEC("main", "[preset_load file not load][pathStr: %s]\n", pathStr.c_str()); return;}

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

  #ifdef ADS_PIN_IR
  _alml_taskSheduler.get_taskByName(TASKNAME_IR)->set_enabled(false);    
  #endif
  _alml_taskSheduler.get_taskByName(TASKNAME_UDP)->set_enabled(false);  
  _alml_taskSheduler.get_taskByName(TASKNAME_PROGRAM)->set_enabled(false);  
  _alml_taskSheduler.get_taskByName(TASKNAME_WIFI)->set_enabled(false);  
  _alml_taskSheduler.get_taskByName(TASKNAME_SERIAL)->set_enabled(false);  
  _alml_taskSheduler.set_enabled_byTaskName(TASKNAME_OUTPUT, false);  


  Serial.printf_P(PSTR("FORMAT BEGIN\n"));
  
  switch (mod) {
    case 0: al_tools::SPIFFS_deleteRecursive(FILESYSTEM, "/outputs"); break;
    case 1: al_tools::SPIFFS_deleteRecursive(FILESYSTEM, "/eff"); break;
    case 2: FILESYSTEM.remove("/wcevo_config.json"); break;
    case 3: FILESYSTEM.format(); break;
    default:break;
  }



}

