#include "../../include/pixeleffect/pixeleffect.h"
#include "../../include/display/lamp.h"
#include "../../include/device/device.h"


EffectWorker::EffectWorker(LAMPSTATE * lampstate){
  _lampstate = lampstate;
}
EffectWorker::~EffectWorker(){
    ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "\n[DELETE EffectWorker]\n");
    clearControlsList();
}
void EffectWorker::clearControlsList()
{
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "-\n");
  while (controls.size()) {
      UIControl *ctrl = controls.shift();
      delete ctrl;
  }
  controls.clear();
}

void EffectWorker::savedefaulteffconfig(const uint16_t nb, const char *folder) {
  String filename = geteffectpathname(nb,folder);
  savedefaulteffconfig(nb, filename); 
}
boolean EffectWorker::loadeffconfig(const uint16_t nb, const char *folder) {
  if(worker==nullptr) return false;  

  boolean setDefault = false;

  String filename = geteffectpathname(nb,folder);

  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "\n\tfilename: %s\n\tnb: %d\n\tname: %s\n", filename.c_str(), nb, effectName.c_str());

  DynamicJsonDocument doc(2048);

  if (!deserializeFile(doc, filename.c_str() )){
    ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tERROR deserializeFile from filesystem file\n");
    doc.clear();
    FILESYSTEM.remove(filename);
    savedefaulteffconfig(nb, filename);   
    if (!deserializeFile(doc, filename.c_str() )) {
      ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tERROR deserializeFile from default file\n");
      FILESYSTEM.remove(filename); 
      return false;
    } 
    setDefault = true; 
  }

  // serializeJsonPretty(doc, Serial);Serial.println();

  curEff = doc[F("nb")].as<uint16_t>();
  const char* name = doc[F("name")];

  uint8_t resetJson = 0;
  if (al_tools::ch_toString(name) != FPSTR(_effeNamIDList[(uint8_t)nb]._nameId))  resetJson = 1;
  if (curEff != nb)                                                               resetJson = (resetJson==1) ? 3 : 2;
  if (resetJson>0) {
    if (resetJson == 1) ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tERROR from JSON : name from struct != JSON name\n");
    if (resetJson == 2) ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tERROR from JSON : id eff != JSON id eff \n");
    if (resetJson == 3) ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tERROR from JSON : name from struct && id eff != JSON name && id eff \n");
    doc.clear();
    FILESYSTEM.remove(filename);
    savedefaulteffconfig(nb, filename);   
    if (!deserializeFile(doc, filename.c_str() )) {
      ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tERROR deserializeFile from default file\n");
      FILESYSTEM.remove(filename); 
      return false;
    } 
    setDefault = true;     
    // serializeJsonPretty(doc, Serial);Serial.println();
  }

  // effectName = name ? name : (String)(FPSTR(_effeNamIDList[(uint8_t)nb]._nameId));
  effectName = (String)(FPSTR(_effeNamIDList[(uint8_t)nb]._nameId));
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\teffname from STRUCT: %s\n", FPSTR(_effeNamIDList[(uint8_t)nb]._nameId));
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\teffname from JSON: %s\n", name);


  JsonArray arr = doc[F("ctrls")].as<JsonArray>();
  clearControlsList();
  uint8_t id_tst = 0x0; 
  for (size_t i = 0; i < arr.size(); i++) {
      JsonObject item = arr[i];
      uint8_t id = item[F("id")].as<uint8_t>();
      if(!(id_tst&(1<<id))){ 
          id_tst |= 1<<item[F("id")].as<uint8_t>();
          String name = item.containsKey(F("name")) ?
              item[F("name")].as<String>()
              : id == 0 ? String(FPSTR(ALMLPT_LEDS_005))
              : id == 1 ? String(FPSTR(ALMLPT_LEDS_006))
              : id == 2 ? String(FPSTR(ALMLPT_LEDS_007))
              : String(F("Доп."))+String(id);
          String val = item.containsKey(F("val")) ? item[F("val")].as<String>() : String(1);
          String min = item.containsKey(F("min")) && id>1 ? item[F("min")].as<String>() : String(1);
          String max = item.containsKey(F("max")) && id>1 ? item[F("max")].as<String>() : String(255);
          String step = item.containsKey(F("step")) && id>2 ?  item[F("step")].as<String>() : String(1);
          CONTROL_TYPE type = item.containsKey(F("type")) && (id>2 || (id<=2 && (item[F("type")].as<CONTROL_TYPE>())==CONTROL_TYPE::RANGE)) ? item[F("type")].as<CONTROL_TYPE>() : CONTROL_TYPE::RANGE;
          CONTROL_CASE typeState = item.containsKey(F("state")) ? item[F("state")].as<CONTROL_CASE>() : CONTROL_CASE::ALWAYS;
          controls.add(new UIControl(
              id,             // id
              type,           // type
              typeState,      // type
              name,           // name
              val,            // value
              min,            // min
              max,            // max
              step            // step
          ));
      }
  }
  doc.clear();
  
  for(int8_t id=0;id<3;id++){
      if(!((id_tst>>id)&1)){ 
          controls.add(new UIControl(
              id,                                     // id
              CONTROL_TYPE::RANGE,                    // type
              CONTROL_CASE::ALWAYS,                   // typeState
              id==0 ? FPSTR(ALMLPT_LEDS_005) : id==1 ? FPSTR(ALMLPT_LEDS_006) : FPSTR(ALMLPT_LEDS_007),           // name
              String(127),                            // value
              String(1),                              // min
              String(255),                            // max
              String(1)                               // step
          ));
      }
  }
  controls.sort([](UIControl *&a, UIControl *&b){ return a->getId() - b->getId();}); 

  if (setDefault) {
    
    int defSpd = _effeNamIDList[(uint8_t)nb]._spd;
    if (defSpd > 0) {
      ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tspd from struct: %d\n", _effeNamIDList[(uint8_t)nb]._spd);
      controls[1]->setVal(String(defSpd));
      worker->setspd(defSpd);
    }
  }
  

  return true; 
}

String EffectWorker::geteffconfig(uint16_t nb, uint8_t replaceBright)
{
    DynamicJsonDocument doc(2048);
    doc[F("nb")]    = nb;
    doc[F("name")]  = effectName;
    JsonArray arr = doc.createNestedArray(F("ctrls"));
    for (int i = 0; i < controls.size(); i++)
    {
        JsonObject var = arr.createNestedObject();
        var[F("id")]      = controls[i]->getId();
        var[F("type")]    = controls[i]->getType();
        var[F("state")]   = controls[i]->getTypeState();
        var[F("name")]    = controls[i]->getName();
        var[F("val")]     = (controls[i]->getId()==0 && replaceBright) ? String(replaceBright) : controls[i]->getVal();
        var[F("min")]     = controls[i]->getMin();
        var[F("max")]     = controls[i]->getMax();
        var[F("step")]    = controls[i]->getStep();
    }
    String cfg_str;
    serializeJson(doc, cfg_str);
    doc.clear();
    return cfg_str;
}

void EffectWorker::geteffconfig(uint16_t nb, JsonObject & doc, uint8_t replaceBright) {
    doc[F("name")]  = effectName;
    worker->geteffJsonconfig(doc, replaceBright);
}

void EffectWorker::geteffconfig(uint16_t nb, JsonObject & doc, uint8_t replaceBright, int id) {
    worker->geteffJsonconfig(doc, replaceBright, id);
}

boolean EffectWorker::saveeffconfig(uint16_t nb, char *folder){
  if(millis()<10000) {return false;} 

  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "-\n");

  uint32_t mS = millis();

  File configFile;
  String filename = geteffectpathname(nb, folder);

  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\teffectName : %s\n",         effectName.c_str());
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tgeteffectpathname : %s\n",  filename.c_str());

  configFile = FILESYSTEM.open(filename, "w"); 
  configFile.print(geteffconfig(nb));
  configFile.close();

  uint32_t mSdiff = millis()-mS;
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\ttook: %u ms\n",mSdiff);
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "--\n");
  return true;
}

void EffectWorker::copyeffconfig(uint16_t nb, const char *srcFolder, const char *destFolder, String & path){

    String src      = geteffectpathname(nb, srcFolder);
    String filename = geteffectpathname(nb, destFolder);

    File desFile;  
    File srcFile;  

    if (FILESYSTEM.exists(filename) == true) FILESYSTEM.remove(filename);

    srcFile = FILESYSTEM.open(src, "r"); 
    if (!srcFile) return;
    desFile = FILESYSTEM.open(filename, "w"); 
    if (!desFile) return;

    #ifdef ESP8266
      char ibuffer[64];  
    #endif
    while (srcFile.available() > 0)
    {
      #if defined(ESP8266)
        byte i = srcFile.readBytes(ibuffer, 64); // i = number of bytes placed in buffer from file f1
        desFile.write(ibuffer, i);               // write i bytes from buffer to file f2  
      #elif defined(ESP32)
        // uint8_t i = srcFile.readBytes(ibuffer, 64); // i = number of bytes placed in buffer from file f1
        // desFile.write(ibuffer, srcFile.read());               // write i bytes from buffer to file f2    
        desFile.print(srcFile.read());     
      #endif

    }

    srcFile.close();
    desFile.close();
    path = filename;
}

void EffectWorker::savedefaulteffconfig(uint16_t nb, String filename){
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "-\n");

  const String efname(FPSTR(_effeNamIDList[nb]._nameId)); 
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\twrite default config file\n\t\t%d - %s\n\t\t%s\n", nb, efname.c_str(), filename.c_str());

  String cfg(FPSTR(FPSTR(_effeNamIDList[nb]._defaultCfg)));   
  cfg.replace(F("@name@"), efname);
  cfg.replace(F("@nb@"), String(nb));

  // #ifdef ALT_DEBUG_TARCE
  //   DynamicJsonDocument doc(1024);
  //   deserializeJson(doc, cfg);
  //   serializeJsonPretty(doc, Serial);Serial.println();  
  // #endif

  File configFile = FILESYSTEM.open(filename, "w"); 
  if (configFile){
      configFile.print(cfg.c_str());
      configFile.close();
      ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tSUCCESS\n");
  } else ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tFAILE writing file\n");
   ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "--\n");
}

const String EffectWorker::geteffectpathname(const uint16_t nb, const char *folder){
    uint16_t swapnb = nb>>8|nb<<8; 
    String filename;
    char buffer[5];
    if (folder) {
        filename.concat(F("/"));
        filename.concat(folder);
    }
    filename.concat(F("/eff/"));
    sprintf_P(buffer,PSTR("%04x"), swapnb);
    filename.concat(buffer);
    filename.concat(F(".json"));
    return filename;
}

bool EffectWorker::deserializeFile(DynamicJsonDocument& doc, const char* filepath){
    if (!filepath || !*filepath)
        return false;

    File jfile = FILESYSTEM.open(filepath, "r");
    DeserializationError error;
    if (jfile){
        error = deserializeJson(doc, jfile);
        jfile.close();
    } else {
        return false;
    }

    if (error) {
        return false;
    }
    return true;
}

void EffectWorker::workerset(uint8_t stripPos, uint16_t effect, const bool isCfgProceed, const bool effStart, const char *folder, const bool effSav){
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "-\n");

  String folderStr = al_tools::ch_toString(folder);
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tLOAD EFFECT\n\t\teffect nb: %d\n\t\tisCfgProceed: %d\n\t\teffStart: %d\n\t\tfolder: %s\n\t\teffSav: %d\n", 
     effect, isCfgProceed, effStart, folderStr.c_str(), effSav);

  if (worker) {
    ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\t\tOLD EFFECT\n\t\t\teffect nb: %d\n\t\t\teffect name: %s\n", curEff, effectName.c_str());
  }
  if(worker && isCfgProceed && effSav){ 
    saveeffconfig(curEff, (char *)folder); 
  }
  if(worker)
     worker.reset(); 

  uint8_t cat = atoi(_effeNamIDList[(uint8_t)effect]._cat);
  switch (cat) {
    case 1: worker = std::unique_ptr<EffectFx_mode> (new EffectFx_mode(effect)); break;
    case 2:
      switch (effect) {
  case 19   : worker = std::unique_ptr<EffectFx_comets>                         (new EffectFx_comets                      (false)           ); break;
  case 20   : worker = std::unique_ptr<EffectFx_Comet>                          (new EffectFx_Comet                       (false)           ); break;
  case 21   : worker = std::unique_ptr<EffectFx_ruggle>                         (new EffectFx_ruggle                      (false)           ); break;
  case 22   : worker = std::unique_ptr<EffectFx_rainbowWave>                    (new EffectFx_rainbowWave                 (false)           ); break;
  case 23   : worker = std::unique_ptr<EffectFx_ArmoireOutsideToInside>         (new EffectFx_ArmoireOutsideToInside      (false)           ); break;
        default   : worker = std::unique_ptr<EffectFx_comets>                         (new EffectFx_comets                      (false)           ); break;
      }      
      break;
    default: break;
  }
  if(worker){
    worker->pre_init((uint8_t)effect, this, &(getControls()), _lampstate);
    originalName = effectName = FPSTR(_effeNamIDList[(uint8_t)effect]._nameId);
    if(isCfgProceed){ 
      ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\t\tNEW EFFECT\n\t\t\teffect name: %s - strip: %d\n", originalName.c_str(), stripPos);
      loadeffconfig(effect, folder);
      if (effStart) {
        worker->init(stripPos, effect, &(getControls()), _lampstate); 
      }
    }
    
    LAMPPTRGET()->set_hueLoop     (LAMPPTRGET()->effectsLoop_hue());
    LAMPPTRGET()->set_palLoop     (LAMPPTRGET()->effectsLoop_pal());
    LAMPPTRGET()->set_palgradLoop (LAMPPTRGET()->effectsLoop_palGrad());

    #ifdef ALML_TFT
      _eff_bri._value   = -1;
      _eff_speed._value = -1;
      _eff_scale._value = -1;  
    #endif

    #ifdef ALT_DEBUG_TARCE    
      ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tPRINT CONTROLS\n");
      LList<UIControl*>&controlsPrint = LAMPPTRGET()->get_effect(0)->getControls();
      for(int i = 0; i < controlsPrint.size(); ++i) {
        controlsPrint[i]->print();
      }  
    #endif
    // ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tPRINT OUTPUT\n");
    // DynamicJsonDocument doc(2048);
    // JsonObject root = doc.to<JsonObject>();
    // JsonObject oOjbect = root.createNestedObject("outputs");
    // DevicePtrGet()->output_toJson(stripPos, oOjbect, false, true);
    // serializeJsonPretty(doc, Serial);Serial.println();
/*
      value[F("eff_activ")]   = _outputArray[pos].get_eff_activ();
      if (fs) {
      if (_effects[pos]) {
      value[F("eff_nb")]      = _effects[pos]->getEn();      
      }
      value[F("eff_colorMod")] = _outputArray[pos].get_eff_colorMod();
      RgbColor c1;
      _outputArray[pos].eff_get_c1(c1);
      col = value.createNestedObject("eff_c1");
      col[F("r")] = c1.R;
      col[F("g")] = c1.G;
      col[F("b")] = c1.B;
      _outputArray[pos].eff_get_c2(c1);
      col = value.createNestedObject("eff_c2");
      col[F("r")] = c1.R;
      col[F("g")] = c1.G;
      col[F("b")] = c1.B;      
      }
*/
  }

  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "--\n");
}







