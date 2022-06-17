#include "../../include/display/remote.h"
#include "../../include/device/device.h"


RemoteControl * RemoteControlPtr = nullptr;
RemoteControl * RemoteControlPtrGet(){return RemoteControlPtr;}

RemoteControl::RemoteControl(){
		RemoteControlPtr = this;
    #ifdef ADS_PIN_IR
    RemoteControl_ir::begin();  
    #endif
}

#ifdef ADS_PIN_IR
void RemoteControl::handle_ir() {
  if (RemoteControl_ir::handle()) {
    ALT_TRACEC(ALML_DEBUGREGION_REMOTE, "send_appi\n");
    RemoteControl_udp::send_appi(true, true, true);
  }
}  
#endif

void RemoteControl::handle_udp() 	{RemoteControl_udp::handle();}

void RemoteControl::begin()  			{RemoteControl_udp::begin();}

void RemoteControl_udp::begin(){
    UdpMulti::stop();
    UdpMulti::begin();
    Udp::stop();   	
    Udp::begin();     
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
  String out;

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

  DevicePtrGet()->outputs_sav();
  DevicePtrGet()->_outputsSav = true;


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
      String s;
      serializeJsonPretty(doc, s);
      ALT_TRACEC(ALML_DEBUGREGION_REMOTE, "&c:1&s:%s\n",s.c_str()); 
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
      String s;
      serializeJsonPretty(doc, s);
      ALT_TRACEC(ALML_DEBUGREGION_REMOTE, "&c:1&s:%s\n",s.c_str()); 
  #endif

  if (op == 4) { 
    doc.clear(); doc.garbageCollect();
    ALT_TRACEC(ALML_DEBUGREGION_REMOTE, "op: 4\n");
    handleJson(op);
    return;    
  }
  else if (op == 7) { 
    doc.clear(); doc.garbageCollect();
    ALT_TRACEC(ALML_DEBUGREGION_REMOTE, "op: 7\n");
    handleJson(op); 
    return;    
  }    
  else if (op == 8) {
    doc.clear(); doc.garbageCollect();
    ALT_TRACEC(ALML_DEBUGREGION_REMOTE, "op: 8\n");
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
