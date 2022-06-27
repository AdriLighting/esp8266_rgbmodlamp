#include "../../include/display/remote.h"
#include "../../include/device/device.h"
#include "../../include/pixel/effectcore.h"
#include "../../include/display/lamp.h"

extern EffectWorker  * _effects[];

RemoteControl * RemoteControlPtr = nullptr;
RemoteControl * RemoteControlPtrGet(){return RemoteControlPtr;}

RemoteControl::RemoteControl(){
		RemoteControlPtr = this;
    #ifdef ADS_PIN_IR
      _Ir_intern = new RemoteControl_ir(ADS_PIN_IR, RemoteControlIrMod_t::RIRMOD_INTERN);
      _Ir_intern->begin();
    #endif
    #ifdef ADS_PIN_IRTFT
      _Ir_TFT = new RemoteControl_ir(ADS_PIN_IRTFT, RemoteControlIrMod_t::RIRMOD_TFT);
      _Ir_TFT->begin();
    #endif        
}

// #ifdef ADS_PIN_IR
  void RemoteControl::handle_ir() {
    if (_Ir_intern && _Ir_intern->handle()) {
      ALT_TRACEC(ALML_DEBUGREGION_REMOTE, "send_appi\n");
      RemoteControl_udp::send_appi(true, true, true);
    }
  }  
// #endif

void RemoteControl::handle_udp() 	{RemoteControl_udp::handle();}

void RemoteControl::begin()  			{RemoteControl_udp::begin();}

void RemoteControl_udp::begin(){
    UdpMulti::stop();
    Udp::stop();  
    UdpMulti::begin();
    Udp::begin();     
}
void RemoteControl_udp::send_toIp(const String & transmit_buffer, IPAddress ip, uint16_t port){ 
  Udp::send_toIp(transmit_buffer, ip, port); 
}
void RemoteControl_udp::send_appi(boolean eff, boolean prog, boolean reset, uint8_t effLoad){ 
    String out = "";
    DevicePtrGet()->outputs_toAppi(out, eff, prog, effLoad);
    Udp::send_toIp(out);  
    if (reset) timerMultiSend_jsonDevice = millis();
}
void RemoteControl_udp::handle(){
  // if (!WCEVO_CONNECTED) return;

	if (Udp::receive()){
	  DynamicJsonDocument doc(2048);  
    String sData = "";
    Udp::get_packetBuffer(sData); 
    // Serial.printf_P(PSTR("UDP\\n")); 
    // Serial.println(sData);
	  DeserializationError error = deserializeJson(doc, sData);
	  if (error) {
      
	  }  else {
	    handleJson(doc, false);
	    yield();
	  }  
	}

  else if (UdpMulti::receive()){
    DynamicJsonDocument doc(2048);  
    String sData = "";
    UdpMulti::get_packetBuffer(sData);  
    // Serial.printf_P(PSTR("%s\n"), sData.c_str());
    DeserializationError error = deserializeJson(doc, sData);
    if (error) {
      
    }  else {
      handleJson(doc);
      yield();
    }       
  }

  else if (multiSend_jsonDevice && (millis() - timerMultiSend_jsonDevice > 1500)) {
    String out;
    DevicePtrGet()->outputs_toNode( out);
    UdpMulti::send(out) ;

    send_appi(false, true, false, 3);

    timerMultiSend_jsonDevice = millis(); 
    yield();
  }
}
void RemoteControl_udp::handleJson(uint8_t op){

  if (op == 4 || op == 7 || op == 8) DevicePtrGet()->outputs_sav();

  String out;

  ALT_TRACEC(ALML_DEBUGREGION_REMOTE, "op: %d - lastrequest: %s\n", op, DevicePtrGet()->get_lastRequest().c_str());

  if (op == 4) { 
    DevicePtrGet()->outputs_toNode( out);
    UdpMulti::send(out) ;
    out = "";
    if (DevicePtrGet()->outputs_requestEffect())  DevicePtrGet()->outputs_toAppi(out, true,   true, 0);
    else                                          DevicePtrGet()->outputs_toAppi(out, false,  true, 0);
    Udp::send_toIp(out);  
  }
  else if (op == 7) { 
    DevicePtrGet()->outputs_effect_toAppi(out);
    Udp::send_toIp(out);  
  }    
  else if (op == 8) {
    DevicePtrGet()->outputs_toNode( out);
    UdpMulti::send(out) ;
    out = "";
    DevicePtrGet()->outputs_toAppi(out, true, true, 2); 
    Udp::send_toIp(out);  
    // send_appi(false, true, false, 0);
  }  

  multiSend_jsonDevice = true;  
  timerMultiSend_jsonDevice = millis(); 

  
  // DevicePtrGet()->set_outputSav(true);


}
void RemoteControl_udp::handleJson(DynamicJsonDocument & doc, boolean udpMulti){

/*
  JSON 
    obj ->  op  

    op 0 || 1 || 2 || 3 = handle from controlid = besion d'interompre la mise jour aux serveur
    op == 0 : group d'outputs 
      obj ->  req     | string  | "commande a effectuer pour tous le grp"  
      obj ->  val     | obj     | "valeure de la commande" (for colorpicker(rgb hsv) : .r .g .b .h .s .v)(other : .v)
      obj ->  from    | byte    |
      obj ->  array   | byte    |
      array == 0 : liste des output par position trier par device          
        array ->  A     |         | array of devices 
          obj   -> dn   | string  |"device name" 
          array -> A    | Array   | array of output position  
    op == 1 : device   
      obj ->  dn      | string  |
      obj ->  req     | string  |
      obj ->  val     | obj     |
      obj ->  from    | byte    |
    op == 2 : all device   
      obj ->  req     | string  |
      obj ->  val     | obj     |
      obj ->  from    | byte    |
    op == 3 : output   
      obj ->  dn      | string  |
      obj ->  output  | byte    |
      obj ->  req     | string  |
      obj ->  val     | obj     |
      obj ->  from    | byte    |

    op == 4 : handle control id realesed 
      sauvegarde FS
      envoi mis a hour serveur
      réactivation timerAuto mise a a jour serveur
    
    op == 5
      preset
        load from fs
        sav to fs
        send update server

    op == 6
      device management

*/
  


  if (doc.containsKey(FPSTR(ALMLPT_GET)) && doc.containsKey(FPSTR(ALMLPT_SET))) {
    String reply = "";
    _AP_Api.parsingRequest(doc, reply, "");
    if (ProgramPtrGet()->isPlaying()) {
      ProgramPtrGet()->remote_action(apSetter_t::APSET_ITEM);
    } else {
      handleJson(8); 
    }
    multiSend_jsonDevice = false;
    
    #ifdef DEBUG_ALMLREMOTE
      if (ALT_debugPrint(ALML_DEBUGREGION_REMOTE)) {String s;serializeJsonPretty(doc, Serial);Serial.println();}
    #endif
    return;
  }

  if (doc.containsKey(FPSTR(ALMLPT_CLI))) {
    if ( doc[FPSTR(ALMLPT_CLI)].as<String>() == FPSTR(ALMLPT_RGB))        {return;}
    if ( doc[FPSTR(ALMLPT_CLI)].as<String>() == FPSTR(ALMLPT_RELAY_AC))   {return;}
    if ( doc[FPSTR(ALMLPT_CLI)].as<String>() == FPSTR(ALMLPT_PLUG))       {return;}
  }
  if (doc.containsKey(FPSTR(ALMLPT_FROM))) {
    if ( doc[FPSTR(ALMLPT_FROM)].as<uint8_t>() != 0 && doc[FPSTR(ALMLPT_FROM)].as<uint8_t>() != 1) {return;}
  }

  uint8_t op = 0;
  if (doc.containsKey(FPSTR(ALMLPT_OP))) {
    op = doc[FPSTR(ALMLPT_OP)];
    if (doc[FPSTR(ALMLPT_FROM)].as<uint8_t>()==1) Udp::client_add(); 
  } else {
    return;
  }

  #ifdef DEBUG_ALMLREMOTE
    if (ALT_debugPrint(ALML_DEBUGREGION_REMOTE)) {String s;serializeJsonPretty(doc, Serial);Serial.println();}
  #endif

  if (op == 4) { 
    doc.clear(); doc.garbageCollect();
    // ALT_TRACEC(ALML_DEBUGREGION_REMOTE, "op: 4\n");
    handleJson(op);
    return;    
  }
  else if (op == 7) { 
    doc.clear(); doc.garbageCollect();
    // ALT_TRACEC(ALML_DEBUGREGION_REMOTE, "op: 7\n");
    handleJson(op); 
    return;    
  }    
  else if (op == 8) {
    doc.clear(); doc.garbageCollect();
    // ALT_TRACEC(ALML_DEBUGREGION_REMOTE, "op: 8\n");
    handleJson(op);  
    return;
  } else {

    multiSend_jsonDevice = false;

    if ((op == 0) && doc.containsKey(FPSTR(ALMLPT_ARRAY))) {
      uint8_t array = doc[FPSTR(ALMLPT_ARRAY)];
      if (array == 0) {
        DevicePtrGet()->parseJson_outpitListByDn(doc);
      } 
    } else if ((op == 1) && doc.containsKey(FPSTR(ALMLPT_DN)) ) {
      String dn       = doc[FPSTR(ALMLPT_DN)].as<String>();
      String thisDn   = al_tools::ch_toString(DevicePtrGet()->get_name());
      if (dn ==  thisDn) {
        DevicePtrGet()->parseJson_device(doc);
      }

    } else if (op == 2)  {
      DevicePtrGet()->parseJson_device(doc);

    } else if ((op == 3) && doc.containsKey(FPSTR(ALMLPT_DN))) {
      String dn = doc[FPSTR(ALMLPT_DN)].as<String>();
      String thisDn = al_tools::ch_toString(DevicePtrGet()->get_name());
      if (dn ==  thisDn) {
        DevicePtrGet()->parseJson_output(doc);
      }
      
    } else if (op == 5)  {
      if (doc.containsKey(FPSTR(ALMLPT_PRESET))) {
        if (doc[FPSTR(ALMLPT_PRESET)].as<String>() == FPSTR(ALMLPT_SAV))   DevicePtrGet()->preset_sav(doc);
        if (doc[FPSTR(ALMLPT_PRESET)].as<String>() == FPSTR(ALMLPT_LOAD))  DevicePtrGet()->preset_load(doc);
      }
    }     
  }
}
void api_getter(DynamicJsonDocument & doc, const char * in) {
  char    * key = nullptr;
  boolean cpy   = true;
  uint8_t count = ARRAY_SIZE(ALMLPT_KKEY_ALL);
  if(isDigit(al_tools::ch_toString(in).charAt(0))) {
    for(int i = 0; i < count; ++i) {
      if (al_tools::ch_toString(in).toInt() == i) {
        key = new char[255];
        strcpy(key, ALMLPT_KKEY_ALL[i]);
        cpy=false;
        break;
      }  
    }    
  }
  if (cpy) {
    key = new char[strlen(in)+1];
    strcpy(key, in);
  }
  if ( al_tools::ch_toString(key) == FPSTR(ALMLPT_KKEY_000) ) 
    EffectslistPtrGet()->get_program()->print();

  if ( al_tools::ch_toString(key) == FPSTR(ALMLPT_KKEY_001) ) {
    uint8_t oP;
    DevicePtrGet()->get_outputCount(oP);
    for(int i = 0; i < oP; ++i) {
      JsonObject effectObj = doc.createNestedObject("opeff_"+String(i));
      _effects[i]->geteffconfig(i, effectObj, LAMPPTRGET()->get_globalBrightness(i, 0));       
    }
  }

  if ( al_tools::ch_toString(key) == FPSTR(ALMLPT_KKEY_002) ) {
    JsonObject oOjbect = doc.createNestedObject(FPSTR(ALMLPT_OUTPUTS));
    DevicePtrGet()->outputs_toJson(oOjbect, false, true, false);
  }
  if ( al_tools::ch_toString(key) == FPSTR(ALMLPT_KKEY_003) ) {
    JsonObject oOjbect = doc.createNestedObject(FPSTR(ALMLPT_OUTPUTS));
    DevicePtrGet()->outputs_toJson(oOjbect, false, true, true);
  }

  if ( al_tools::ch_toString(key) == FPSTR(ALMLPT_KKEY_004) ) 
    reqNamIDList_json(0, doc);  

  if ( al_tools::ch_toString(key) == FPSTR(ALMLPT_KKEY_005) ) 
    serializeJsonPretty(DeviceUserConfig, Serial);Serial.println();



}
void keyboard_getter(const String & v1) {
  int rSize = 0;
  DynamicJsonDocument doc(3500);

  LList<SplitItem *> _SplitItem;
  splitText(v1, "&",  ':', &_SplitItem);

  for(int j = 0; j < _SplitItem.size(); ++j) {
    const char** split = al_tools::explode(_SplitItem[j]->_value, ',', rSize);
    if (split) {
      for(int i = 0; i < rSize; ++i) {
        Serial.printf_P(PSTR("[%d] %s\n"), i , split[i]);
        if (strcmp_P(_SplitItem[j]->_cmd, "JSON") == 0)     api_getter(doc, split[i]);                           
        // #ifdef ALSI_ENABLED
        // if (strcmp_P(_SplitItem[j]->_cmd, "ALSI") == 0)   ALSYSINFO_getterByCat(doc, split[i]);                           
        // if (strcmp_P(_SplitItem[j]->_cmd, "ALSII") == 0)  ALSYSINFO_getterByKey(doc, split[i]);   
        // #endif                          
      }
      for(int i = 0; i < rSize; ++i) {
        delete split[i];
      }
      delete[] split; 
    } else {
        if (strcmp_P(_SplitItem[j]->_cmd, "JSON") == 0)     api_getter(doc, _SplitItem[j]->_value);                           
        // #ifdef ALSI_ENABLED
        // if (strcmp_P(_SplitItem[j]->_cmd, "ALSI") == 0)   ALSYSINFO_getterByCat(doc, _SplitItem[j]->_value);                           
        // if (strcmp_P(_SplitItem[j]->_cmd, "ALSII") == 0)  ALSYSINFO_getterByKey(doc, _SplitItem[j]->_value);   
        // #endif        
    }
  }
  while (_SplitItem.size()) {
    SplitItem *eff = _SplitItem.shift();
    delete eff;
  }
  _SplitItem.clear();

  serializeJsonPretty(doc,Serial);Serial.println(); 
   
}
void keyboard_print() {
  Serial.printf_P(PSTR("@&JSON:0,list_lbid=\n"));
  Serial.printf_P(PSTR("JSON\n"));
  uint8_t count = ARRAY_SIZE(ALMLPT_KKEY_ALL);
  for(int i = 0; i < count; ++i) {
    Serial.printf_P(PSTR("[%-3d] %s\n"), i, ALMLPT_KKEY_ALL[i]);
  }    
}

