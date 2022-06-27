#include "../../include/display/effectslist.h"
#include "../../include/display/remote.h"
#include "../../include/device/device.h"





Effectslist * _EffectslistPtr = nullptr;
Effectslist * EffectslistPtrGet() {return _EffectslistPtr;}



void _Program_cb(const String itemBaseName, const uint16_t& itemBasePos, uint8_t id, boolean updWebserver) {
  // Serial.printf_P(PSTR("[user_callback]\n\t[%-3d] %-35s %d\n"), itemBasePos, itemBaseName.c_str(), id);
  _EffectslistPtr->eff_byId(id);
  if( RemoteControlPtrGet()) {
    RemoteControlPtrGet()->send_appi(true, true, true, 1);
  }
}

Effectslist::Effectslist(){
  ALT_TRACEC("main", "-\n");

	_EffectslistPtr = this;


  uint8_t cnt = 0;

  uint8_t excluedStart = 0;
  String  excluedItem = "";

  for(int i = 0; i < _effeNamIDList_cnt; ++i) {
    String item = al_tools::ch_toString(_effeNamIDList[i]._nameId);

    excluedItem = "";
    excluedStart = 0;
    JsonArray excluedArr  = DeviceUserConfig[F("list_lb_exclued")].as<JsonArray>();
    uint8_t   excluedSize = excluedArr.size();
    for(uint8_t i = 0; i < excluedSize; ++i) {
      excluedItem = excluedArr[i].as<String>();
      if(excluedItem != "" && item.indexOf(excluedItem) > -1)  excluedStart++;
    }
    if (excluedStart>0)continue;
    // if(excluedItem != "" && item.indexOf(excluedItem) > -1)  continue;
    // if(item.indexOf("Syncro") > -1)  continue;
    // if(item.indexOf("Rainbow") < 0)  continue;

    cnt++;
  }
  _AP_DEFAULTLIST = new const char*[cnt+1]; 
  _listId  				= new uint8_t[cnt+1]; 
  uint8_t pos = 0;


  for(int i = 0; i < _effeNamIDList_cnt; ++i) {
    String item = al_tools::ch_toString(_effeNamIDList[i]._nameId);

    excluedItem = "";
    excluedStart = 0;
    JsonArray excluedArr  = DeviceUserConfig[F("list_lb_exclued")].as<JsonArray>();
    uint8_t   excluedSize = excluedArr.size();
    for(uint8_t i = 0; i < excluedSize; ++i) {
      excluedItem = excluedArr[i].as<String>();
      if(excluedItem != "" && item.indexOf(excluedItem) > -1)  excluedStart++;
    }
    if (excluedStart>0)continue;
    // if(excluedItem != "" && item.indexOf(excluedItem) > -1)  continue;
    // if(item.indexOf("Armoire") > -1)  continue;
    // if(item.indexOf("Syncro") > -1)  continue;
    // if(item.indexOf("Rainbow") < 0)  continue;

    char * buff = new char[item.length()+1];
    strcpy(buff, item.c_str());
    _AP_DEFAULTLIST[pos] = strdup(buff);
    _listId[pos] = i;

    // Serial.printf_P(PSTR("item add: [%-3d] %-35s\n"), _listId[pos], _AP_DEFAULTLIST[pos]);

    pos++;

    delete buff;
  }

  // for(int i = 0; i < cnt; ++i) {
  //   Serial.printf_P(PSTR("[%-3d] %-3d | %-35s\n"), i, _listId[i],_AP_DEFAULTLIST[i]);
  // }
  _Program = new Program(1, true);
  _Program->set_fs_pl(false);  
  _Program->set_callback(_Program_cb);
  _Program->initialize_lb(0, "full", cnt , _AP_DEFAULTLIST);
  _Program->initialize( cnt, _AP_DEFAULTLIST, _listId, "full", apListSortType_t::ST_AB);
  // _Program->print();

  uint8_t plC       = 1;
  uint8_t iC[]      = {1};  // nb items max
  const char * Ln[] = {"full"};
  _Program->initialize_playlist(plC, iC, Ln);

/*
  for(int i = 0; i < _effeNamIDList_cnt; ++i) {
    String name = FPSTR(_effeNamIDList[i]._nameId);
    String type = FPSTR(_effeNamIDList[i]._effType);
    String cat  = FPSTR(_effeNamIDList[i]._cat);
    String seg  = FPSTR(_effeNamIDList[i]._segment);
    Serial.printf_P(PSTR("[%-3d] %-40s %-10s %-3s %-3s\n"), i, name.c_str(), type.c_str(), cat.c_str(), seg.c_str());

  }
  for(int i = 0; i < _effeNamIDList_cnt; ++i) {
    String cfg  = FPSTR(_effeNamIDList[i]._defaultCfg);
    cfg.replace(F("@name@"), FPSTR(_effeNamIDList[i]._nameId));
    Serial.printf_P(PSTR("[%-3d] %-22s\n"), i, cfg.c_str());
  }  
*/
  _Program->remote_action(apSetter_t::APSET_PLAY_LB);
  _Program->remote_action(apSetter_t::APSET_PLAY_DELAY, "60");
  _Program->remote_action(apSetter_t::APSET_PLAY_STOP);
  _Program->remote_action(apSetter_t::APSET_ITEMID, "0");

  ALT_TRACEC("main", "--\n");
}

void Effectslist::eff_next(){
	_Program->remote_action(apSetter_t::APSET_ITEM_NEXT);	
}
void Effectslist::eff_prev(){
	_Program->remote_action(apSetter_t::APSET_ITEM_PREV);	
}
void Effectslist::toggle(){
  _Program->remote_action(apSetter_t::APSET_PLAY_TOGGLE); 
}
void Effectslist::eff_byId(uint8_t p){
  DynamicJsonDocument doc(255);
  doc[FPSTR(ALMLPT_REQ)]   = FPSTR(req_eff_f);
  JsonObject val = doc.createNestedObject(FPSTR(ALMLPT_VAL));
  val[FPSTR(ALMLPT_V)]     = p;
  val[FPSTR(ALMLPT_C)]     = FPSTR(req_eff_load);
  DevicePtrGet()->parseJson_device(doc);
}
void Effectslist::eff_byName(const char * name){
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

Program * Effectslist::get_program() {return _Program;}