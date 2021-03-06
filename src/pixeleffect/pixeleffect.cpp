#include "../../include/pixeleffect/pixeleffect.h"
#include "../../include/pixel/color_palette.h"
#include "../../include/display/lamp.h"

extern CRGBPalette16 cTargetPalette;;

void nscale8( CRGBW* leds, uint16_t num_leds, uint8_t scale)
{
  for( uint16_t i = 0; i < num_leds; i++) {
      leds[i].nscale8( scale);
  }
}

void fadeToBlackBy( CRGBW* leds, uint16_t num_leds, uint8_t fadeBy)
{
    nscale8( leds, num_leds, 255 - fadeBy);
}


void EffectFx_neo::setup(uint8_t strip_pos, OUTPUT_EFFECTTYPE mod){ 
  _strip_pos = strip_pos;
  boolean isTw;
  leds_managePtrGet()->get_ledsIsTw(strip_pos, isTw);
  if (isTw) ledarray_w  = leds_managePtrGet()->get_crgwbArray(strip_pos);
  else      ledarray    = leds_managePtrGet()->get_crgbArray(strip_pos);
  leds_managePtrGet()->get_numLeds(strip_pos, numLeds);

  _effectType = mod;
  DevicePtrGet()->get_outputArray(strip_pos)->set_effectType(_effectType);

String s;
 effectType_toString(mod, s);
ALT_TRACEC("main", "test: %s\n", s.c_str());

  neoSetting.pos            = 0;
  neoSetting.numLeds        = numLeds;
  neoSetting.segment        = true;
  neoSetting.reverseStart   = false;
  neoSetting.c1             = {0,80,0};
  neoSetting.c2             = {0,0,80};    
  neoSetting.isTw           = isTw;                
  neoSetting.fade           = getCtrlVal(3).toInt();                
  neoSetting.scale          = scale;                
  neoSetting.size           = 3;                
  neoSetting.intensity      = 60;                
  neoSetting.palette        = 0;                
  oldBpm                    = 0;
  oldBri                    = 0;
} 
void EffectFx_neo::instance(){
  boolean isTw;
  leds_managePtrGet()->get_ledsIsTw(_strip_pos, isTw);    
  if (isTw) _neoFxxPtr = new WS2812FX(_strip_pos, ledarray_w, &neoSetting);
  else      _neoFxxPtr = new WS2812FX(_strip_pos, ledarray, &neoSetting);  
  LAMPPTRGET()->effectsLoop_palGrad_reset(_pPos);
  _gTargetPalette       =  gGradientPalettes[_pPos] ;
  neoSetting.pPalette   =  gGradientPalettes[_pPos] ;   
}
const TProgmemRGBPalette16 *palette_arr[] = {
  &AuroraColors_p,
  &ForestColors_p,
  &NormalFire_p,
  &LavaColors_p,
  &OceanColors_p,
  &PartyColors_p,
  &RainbowColors_p,
  &HeatColors_p,
  &CloudColors_p,
  &EveningColors_p,
  &LithiumFireColors_p,
  &WoodFireColors_p,
  &SodiumFireColors_p,
  &CopperFireColors_p,
  &AlcoholFireColors_p,
  &RubidiumFireColors_p,
  &PotassiumFireColors_p,
  &WaterfallColors_p,
  &AutumnColors_p,
  &AcidColors_p,
  &StepkosColors_p,
  &HolyLightsColors_p
};
void EffectFx_neo::update(){
    if (oldBpm!= speed){
      oldBpm = speed;
      _neoFxxPtr->setSpeed(convertSpeed(speed));
    }   
    neoSetting.intensity  = LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness) ;
    if (oldBri!= LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness)){
      oldBri = LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness);
      ALT_TRACEC("main", ">>>>>>>>>%d\n", LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness));
      if (_hue_1 != 0) neoSetting.c1 = hsv2rgb (_hue_1, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0);  
      if (_hue_2 != 0) neoSetting.c2 = hsv2rgb (_hue_2, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0);   
      statu = true;     
    }      
    neoSetting.scale = scale;
    if ( _effectType == OET_HUETIMER || _effectType == OET_HTPG ) {
      if (_hue_1==0) {
        if (statu || _effectType == OET_HTPG) {
                          neoSetting.c1 = hsv2rgb (LAMPPTRGET()->_hsvLoop_1.hue(),        255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0); 
          if (_hue_2==0)  neoSetting.c2 = hsv2rgb (LAMPPTRGET()->_hsvLoop_1.hue()+_varia, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0);    
          if (_effectType == OET_HUETIMER) LAMPPTRGET()->_hsvLoop_1.hueChangeToggle();         
        } 
      }
      if (_hue_2==0 && _hue_1!=0) {
        if (statu || _effectType == OET_HTPG) {
                          neoSetting.c2 = hsv2rgb (LAMPPTRGET()->_hsvLoop_2.hue(),        255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0); 
          if (_hue_2!=0)  neoSetting.c2 = hsv2rgb (LAMPPTRGET()->_hsvLoop_2.hue()+_varia, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0);    
          if (_effectType == OET_HUETIMER) LAMPPTRGET()->_hsvLoop_2.hueChangeToggle();         
        } 
      }           
    } 
    if (_effectType == OET_PALGRAD || _effectType == OET_HTPG){
      // neoSetting.pPalette = cTargetPalette ;
      if ((millis()-_timer_toward)>40){
          nblendPaletteTowardPalette(  neoSetting.pPalette, _gTargetPalette, 48);
          _timer_toward = millis();
      }
      if (LAMPPTRGET()->_palGraddPos != _pPos) {
        _pPos = LAMPPTRGET()->_palGraddPos;
        if (_pPos>GRADIENT_PALETTS_COUNT){_pPos=0;LAMPPTRGET()->_palGraddPos=0;}
        _gTargetPalette =  gGradientPalettes[_pPos] ;
      }      
    }
    if (_effectType == OET_PAL ){
      if (LAMPPTRGET()->_palPos != _pPos) {
        _pPos = LAMPPTRGET()->_palPos;
        if (_pPos>22){_pPos=0;LAMPPTRGET()->_palPos=0;}
      }
      neoSetting.pPalette =  * palette_arr[_pPos] ;      
    }   
    
    _neoFxxPtr->service(statu, show);
} 

void EffectFx_neo::set_dynCtrl(UIControl*_val){
  if      (_val->getId()==3) { neoSetting.fade = EffectCalc::setDynCtrl(_val).toInt(); }
  else if (_val->getId()==4) { 
    LAMPPTRGET()->effectsLoop_TimerHue(EffectCalc::setDynCtrl(_val).toInt()); 
  }
  else if (_val->getId()==8) {
    boolean state = LAMPPTRGET()->_hsvLoop_1.hueChangeGet();
    LAMPPTRGET()->_hsvLoop_1.hueChangeSet(true);
    _hue_1 = EffectCalc::setDynCtrl(_val).toInt(); 
    if (_hue_1 != 0) {neoSetting.c1 = hsv2rgb (_hue_1, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0);} 
    LAMPPTRGET()->_hsvLoop_1.hueChangeSet(state);
  }
  else if (_val->getId()==9) {
    boolean state = LAMPPTRGET()->_hsvLoop_2.hueChangeGet();
    LAMPPTRGET()->_hsvLoop_2.hueChangeSet(true);
    _hue_2 = EffectCalc::setDynCtrl(_val).toInt(); 
    if (_hue_2 != 0) {neoSetting.c2 = hsv2rgb (_hue_2, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0);}     
    LAMPPTRGET()->_hsvLoop_2.hueChangeSet(state);
  }
  else if (_val->getId()==5) { _varia = EffectCalc::setDynCtrl(_val).toInt(); }
  else if (_val->getId()==6) { neoSetting.size = EffectCalc::setDynCtrl(_val).toInt(); }
  else EffectCalc::setDynCtrl(_val).toInt();
  #ifdef ALT_DEBUG_TARCE
    _val->print();  
  #endif
}
EffectFx_neo::~EffectFx_neo(){
  delete _neoFxxPtr;
  _neoFxxPtr= nullptr;
}   
void EffectFx_mode::load(uint8_t strip_pos){
  ALT_TRACEC("main", "_mod: %d\n", _mod);

  ALT_TRACEC("main", "neoSetting.segment: %d\n", neoSetting.segment);
  ALT_TRACEC("main", "_effType: %s\n", _effeNamIDList[_mod]._effType);
  ALT_TRACEC("main", "_effType: %d\n", effectType_toMod(_effeNamIDList[_mod]._effType));
  ALT_TRACEC("main", "_nameId: %s\n", _effeNamIDList[_mod]._nameId);
  ALT_TRACEC("main", "_cat: %s\n", _effeNamIDList[_mod]._cat);
  ALT_TRACEC("main", "_segment: %s\n", _effeNamIDList[_mod]._segment);

  setup(strip_pos,  effectType_toMod(_effeNamIDList[_mod]._effType) );
   
  neoSetting.segment = atoi(_effeNamIDList[_mod]._segment);
  // switch (_mod) {
  // case 13    : neoSetting.reverseStart = true  ; break;
  // }  

  instance();

  switch (_mod) {
  case 0    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_pride                 ; break;
  case 1    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_pride                 ; break;
  case 2    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_colorwaves            ; break;
  case 3    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_colorwaves            ; break;
  case 4    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_rainbow_cycle         ; break;
  case 5    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_rainbow_cycle         ; break;
  case 6    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_rainbow               ; break;
  case 7    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_color_wipe            ; break;
  case 8    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_color_wipe            ; break;
  case 9    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_color_wipe_rev_inv    ; break;
  case 10   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_color_wipe_rev_inv    ; break;
  case 11   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_color_wipe_random     ; break;
  case 12   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_color_wipe_random     ; break;
  case 13   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_tricolor_chase        ; break;
  case 14   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_tricolor_chase        ; break;
  case 15   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_breath                ; break;
  case 16   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_colortwinkle          ; break;
  case 17   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_fade                  ; break;
  case 18   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_fire_2012                  ; break;
  default:break;
  }   
}
bool EffectFx_mode::run()                                     {update(); return true;}
String EffectFx_mode::setDynCtrl(UIControl*_val)              {set_dynCtrl(_val); return String();}



void EffectFx_comm::setup(uint8_t strip_pos){ 
  _strip_pos = strip_pos;
  _effectType = OET_HUETIMER;

  leds_managePtrGet()->get_ledsIsTw(_strip_pos, _isTw);

  if (_isTw) _ledarray_w  = leds_managePtrGet()->get_crgwbArray(_strip_pos);
  else       _ledarray    = leds_managePtrGet()->get_crgbArray(_strip_pos);

  DevicePtrGet()->get_outputArray(_strip_pos)->set_effectType(OET_HUETIMER) ;

  leds_managePtrGet()->get_numLeds(_strip_pos, _numLeds);
  _isSegment = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.isSegment();

  if (DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.isSegment()  ) {
    DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_max(_maxSeg);
  }

}
boolean EffectFx_comm::dryrun(float n, uint8_t delay){
  if((millis() - _lastupdate) < (unsigned)(float(255 - speed) / n)) {
    _active=false;
  } else {
    _lastupdate = millis();
    _active=true;
  }
  return !_active;
}
boolean EffectFx_comm::dryrun(uint8_t s, float n, uint8_t delay){
  if((millis() - _lastupdate) < (unsigned)(float(255 - s) / n)) {
    _active=false;
  } else {
    _lastupdate = millis();
    _active=true;
  }
  return !_active;
}
void EffectFx_comm::set_dynCtrl(UIControl*_val){
  if      (_val->getId()==3) { _fade = EffectCalc::setDynCtrl(_val).toInt(); }
  else if (_val->getId()==4) { _hueTimer = EffectCalc::setDynCtrl(_val).toInt(); LAMPPTRGET()->effectsLoop_TimerHue(_hueTimer); }
  else if (_val->getId()==8) {
    _hue_1 = EffectCalc::setDynCtrl(_val).toInt(); 
    _hue_1V = _hue_1;
    // if (_hue_1 != 0) {neoSetting.c1 = hsv2rgb (_hue_1, 255, 255, 0);} 
  }
  else if (_val->getId()==9) {
    // _hue_2 = EffectCalc::setDynCtrl(_val).toInt(); 
    // if (_hue_2 != 0) {neoSetting.c2 = hsv2rgb (_hue_2, 255, 255, 0);}     
  }
  else if (_val->getId()==5)  { _varia = EffectCalc::setDynCtrl(_val).toInt(); }
  else if (_val->getId()==6)  { _size = EffectCalc::setDynCtrl(_val).toInt(); }
  else if (_val->getId()==10) { _rep = EffectCalc::setDynCtrl(_val).toInt(); }
  else EffectCalc::setDynCtrl(_val).toInt();
  #ifdef ALT_DEBUG_TARCE
    _val->print();  
  #endif
}
void EffectFx_comm::update(){
  if ( _effectType == OET_HUETIMER || _effectType == OET_HTPG ) {
    if (_hue_1==0) {
      _hue_1V = LAMPPTRGET()->_hsvLoop_1.hue(); 
    }
  } 
} 
// void fade2black(RgbColor * rgb) {
//   int j = (255-global_persistence);
//   if (j==0) j=1;
//   rgb->R = (rgb->R>j) ? rgb->R-j  : 0 ;
//   rgb->G = (rgb->G>j) ? rgb->G-j  : 0 ;
//   rgb->B = (rgb->B>j) ? rgb->B-j  : 0 ;
// }
// CRGB fade2black(CRGB rgb, uint8_t ammount = 127) ;
// CRGB fade2black(CRGB rgb, uint8_t ammount) {
//   CRGB result;
//   int j = (255-ammount);
//   if (j==0) j=1;
//   result.r = (rgb.r>j) ? rgb.r-j  : 0 ;
//   result.g = (rgb.g>j) ? rgb.g-j  : 0 ;
//   result.b = (rgb.b>j) ? rgb.b-j  : 0 ;
//   return result;
// }

String EffectFx_comets::setDynCtrl(UIControl*_val) {set_dynCtrl(_val); return String();}
void EffectFx_comets::load(uint8_t strip_pos){
  setup(strip_pos);
  speed = 180;
  _counter = 0;
}
bool EffectFx_comets::run(){
  update();

  static long     comet_next_start;
  static long     comet_birthday;
  static uint8_t  comet_length;        
  static uint8_t  comet_start_pos;
  static uint8_t  comet_hue;
  static int8_t   comet_speed;
  static int      comet_duration;
  static enum     {dead,alive} comet_status;
  // speed -> rotation pixel
  uint8_t len                 = 127;  // duration
  uint8_t rep                 = _size;   // apparition 
  uint8_t bright_max          = 255;
  uint8_t fade                = LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness);
  uint8_t sat                 = 255;

  uint8_t brightness_fade_ct;
  uint8_t sat_fade_ct;
  uint8_t comet_loc_in_pix;
  uint8_t comet_brightness;
  uint8_t comet_progress;
  int     comet_displacement;

  // if (dryrun(1)) return false;
  // if (dryrun(1)) return false;     

  _pixstart = 0;
  _pixend   = _numLeds-1;
  _pixrange = (_pixend - _pixstart+1);   

  // fadeToBlackBy(_ledarray, _pixrange, 220);
  if (_isTw) {
    for (int i=_pixstart; i <= _pixend; i++) {
      CRGBW old = _ledarray_w[i];
      CRGB c = fade2black(CRGB(old.r, old.g, old.b));
      _ledarray_w[i] = CRGBW(c.r, c.g, c.b, 0);
    } 
  }else {
    for (int i=_pixstart; i <= _pixend; i++) {
      CRGB old = _ledarray[i];
      _ledarray[i] = fade2black(old);
    }    
  }


  if (comet_status==dead) { // if comet inactive
    if (millis()>(uint32_t)comet_next_start) { // and time is elapsed -> we start a new one
      comet_length       =  random8(5, _pixrange); ; // len of the path
      comet_length       *= len/256;
      comet_hue          = _hue_1V;
      comet_start_pos    = random8(0, 255);
      comet_speed        = ( random(_pixstart, _pixrange) );
      comet_duration     = random(speed, map(len,0,255,120,10000)); // random durations from 100mS to 2000mS, adj with len slider
      comet_birthday     = millis();
      comet_status       = alive;
    }
  }
  if (comet_status==alive) { // if comet alive... let it live !!
    uint8_t comet_sat = sat;
    int     comet_age = millis()-comet_birthday;
    if (comet_age<comet_duration) {
            comet_progress      = map (comet_age, 0, comet_duration, 0, 255);     // we compute the progress (0=birthday, 255=death)
            comet_brightness    = comet_progress;                                 // for now, let make brightness simple
            bright_max          = map8(bright_max,0,240);
      if (sat==255) sat--;
            brightness_fade_ct  = map8(comet_length, bright_max, 241);
            sat_fade_ct         = map8(comet_length, sat, 255);
            comet_displacement  = map(comet_progress, 0, 255, 0, comet_speed);    // we suppose the speed reflects the total displacement
      int   comet_loc           = comet_start_pos + comet_displacement;
            comet_loc_in_pix    = map8(comet_loc,_pixstart,_pixend);
      int   comet_dir           = (comet_speed>0) ? -1 : +1;                      // the comet direction on the hoop

      while (comet_brightness>5) {  

        int pixel_offset        = map( (uint8_t)_counter, 0, 255, _pixstart, _pixend);  

        if (!_isTw) led_set_hsv_  (_ledarray,   comet_loc_in_pix, _pixrange, pixel_offset, comet_hue, comet_sat, comet_brightness, fade );
        else        led_set_hsv   (_ledarray_w, comet_loc_in_pix, _pixrange, pixel_offset, comet_hue, comet_sat, comet_brightness, fade );

            comet_loc_in_pix    += comet_dir%_pixrange;
            comet_brightness    = map8(comet_brightness, 0, brightness_fade_ct);
            comet_sat           = map8(comet_sat,        0, sat_fade_ct);
      }

      delay(1);

    } else {
      comet_status=dead;                                                // if comet too old compared to duration, it's time for euthanasy
      comet_next_start = millis() + (map(rep,0, 255, 10, 255)*128);   // next comet will start in 10...1500 mS
    }
  }

  if (!dryrun(3.0)) _counter++;

  return true;
}




String EffectFx_bounce::setDynCtrl(UIControl*_val) {set_dynCtrl(_val); return String();}
void EffectFx_bounce::load(uint8_t strip_pos){
  setup(strip_pos);
  speed = 100;
  _counter = 0;
}
bool EffectFx_bounce::run(){
  update();

  #define MAX_DOT 16

  static int speed        [MAX_DOT];
  static int pos          [MAX_DOT];
  static int mass         [MAX_DOT];
  static uint8_t          last_rep;

  if (dryrun(1)) return false;

  _pixstart = 0;
  _pixend   = _numLeds-1;
  _pixrange = (_pixend - _pixstart+1);   

  if (_isTw) fadeToBlackBy(_ledarray_w, _pixrange, 127);
  else fadeToBlackBy(_ledarray, _pixrange, 127);

  uint8_t rep     = 2;
  uint8_t hv      = _varia;
          _hue    = _hue_1V;

  if (last_rep != rep) {
    for (int i=0; i<MAX_DOT; i++) {
      mass[i]     = 4;
      speed[i]    = ((i&1)==0) ? 2000*(i+1)/rep : -2000*(i+1)/rep;
      pos[i]      = i * (_pixrange/rep);
      //Serial.printf("i:%3d mass:%3d speed:%4d\n",i, mass[i], speed[i]);
    }
    last_rep = rep;
  } 

  for (int dot_num=0; dot_num<rep; dot_num++) {
    // 1/ we calculate the new position from the older one and the current speed
    pos[dot_num] = (pos[dot_num] + speed[dot_num]/100)%(_pixrange*20); // new position = old position + signed speed
    if (pos[dot_num]<0) pos[dot_num]+=(_pixrange*20);
    // 2/ we detect collisions
    for (int j=0; j<rep; j++) { // now we search collisions for every dot
      if ( (j!=dot_num) && (abs(pos[j]-pos[dot_num])<10) ){ //now we found one
        int v1i = speed[dot_num];
        int v2i = speed[j];
        int m1  = mass[dot_num];
        int m2  = mass[j];        
        // we apply cinetic & constant motion formula (see http://www.grasp.ulg.ac.be/nvdw/NVdw/Documents_files/08.collisions.pdf)
        int v1f = (   v1i*(m1-m2)  +  v2i*(m2+m2)   )   /   (m1+m2);
        int v2f = (   v2i*(m1-m2)  +  v1i*(m1+m1)   )   /   (m1+m2);        
        speed[dot_num]  = v1f;
        speed[j]        = v2f;
      }   
    }
    // 03/ and at last we draw the dots on the ledstrip
    for (int dot_num=0; dot_num<rep; dot_num++) {
      int varicolor = (dot_num<<8)/rep;           // varies from 0 to 256
      uint8_t var = 0xff & ((varicolor*hv>>8)-(hv/2));
      uint8_t hue = _hue + var;
      int pixel_offset = map( (uint8_t)_counter, 0, 255, _pixstart, _pixend);
      if (_isTw)  led_set_hsv(_ledarray_w,pos[dot_num]/20,_pixrange,pixel_offset,hue, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 127);
      else led_set_hsv_(_ledarray,pos[dot_num]/20,_pixrange,pixel_offset,hue, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 127);
    }        
  }
  _counter = (_direction) ? _counter + 1 : _counter - 1;
  
  return true;
}

String EffectFx_ruggle::setDynCtrl(UIControl*_val) {set_dynCtrl(_val); return String();}
void EffectFx_ruggle::load(uint8_t strip_pos){
  setup(strip_pos);
  speed = 127;
  _counter = 0;
}
bool EffectFx_ruggle::run(){
  update();

  if (dryrun(1)) return false;

  _pixstart = 0;
  _pixend   = _numLeds-1;
  _pixrange = (_pixend - _pixstart+1);   

  if (_isTw) fadeToBlackBy(_ledarray_w, _pixrange, 127);
  else fadeToBlackBy(_ledarray, _pixrange, 127);

  uint8_t rep     = 3;
  uint8_t hv      = 120;
  uint8_t len     = map8(60, 1, _pixrange/rep);

  for (int dot_num=0; dot_num<rep; dot_num++) {
    int varicolor = (dot_num<<8)/rep; // varies from 0 to 256
    uint8_t var = 0xff & ((varicolor*hv>>8)-(hv/2));
    uint8_t hue = _hue + var;
    int x=256-sawtooth_wave(10+dot_num*2+rep-1); // select the dot position according to its number. Notice: the frequency depends on the dot selected so they are not in sync
    x=map8(x, _pixstart, _pixend);
    int pixel_offset = map( (uint8_t)_counter, 0, 255, _pixstart, _pixend);
    if (_isTw) 
      for (int j=0; j<len; j++)   led_set_hsv(_ledarray_w, x+j, _pixrange, pixel_offset, hue, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 127);
    else 
      for (int j=0; j<len; j++)   led_set_hsv_(_ledarray, x+j, _pixrange, pixel_offset, hue, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 127);
  }
  _counter = (_direction) ? _counter + 1 : _counter - 1;
  return true;
}

String EffectFx_Comet::setDynCtrl(UIControl*_val) {set_dynCtrl(_val); return String();}
void EffectFx_Comet::load(uint8_t strip_pos){
  setup(strip_pos);
  speed = 220;
  _counter = 0;
  _isSegmentEnabled = false;
}
bool EffectFx_Comet::run(){
  update();

  _color_fg = CHSV(_hue_1V, 255,  LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness));

  if (dryrun(0.5)) return false;
      
  boolean reverse = _reverseStart;
  for (int A = 0; A < _maxSeg; ++A) {
    if ( _isSegment && _isSegmentEnabled) { 
      _pixstart = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(A)->_start;
      _pixend   = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(A)->_end;  
    }else{
      _pixstart = 0;
      _pixend   = _numLeds-1;
    }
    _pixrange = (_pixend - _pixstart+1);  

    for (int i = _pixstart; i <= _pixend; i++) {

      uint32_t pixelPos = (reverse)? (_pixrange-1) - i : i;
      if (_maxSeg > 1 && !_reverseStart) pixelPos = (_pixrange-1) - pixelPos;

      float ratio;
      if (!scale) {
        ratio = cubicwave8((255 * (pixelPos - _pixstart) / _pixrange) + _counter) / 255.0;
      } else {
        ratio = triwave8((255 * (pixelPos - _pixstart) / _pixrange) + _counter) / 255.0;
      }

      CRGB wavecolor = CRGB(_color_fg.r * ratio, _color_fg.g * ratio, _color_fg.b * ratio);
      // _pixset[i] = wavecolor;
      if (_isTw)  _ledarray_w[pixelPos] = wavecolor;
      else        _ledarray[pixelPos]   = wavecolor;    
    }
    _counter = (_direction) ? _counter + 2 : _counter - 2;

    reverse = !reverse;
  }
  return true;
}




String EffectFx_rainbowWave::setDynCtrl(UIControl*_val) {set_dynCtrl(_val); return String();}
void EffectFx_rainbowWave::load(uint8_t strip_pos){
  setup(strip_pos);
  speed = 180;
  _counter = 0;
}
bool EffectFx_rainbowWave::run(){
  if (dryrun(1)) return false;
      
  boolean reverse = false;
  for (int A = 0; A < _maxSeg; ++A) {
    if ( _isSegment && _isSegmentEnabled ) { 
      _pixstart = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(A)->_start;
      _pixend   = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(A)->_end;  
    }else{
      _pixstart = 0;
      _pixend   = _numLeds-1;
    }
    _pixrange = (_pixend - _pixstart+1);   

    float ratio = 255.0  / (_pixrange);
    for (uint16_t i = _pixstart; i < _pixend+1; i++) {

      // uint32_t pixelPos = i;
      uint16_t pixelPos = (reverse)? (_pixstart+_pixend) - i : i;
      // if (_maxSeg > 1 && !_reverseStart) pixelPos = (_pixrange-1) - pixelPos;
      // Serial.printf("reverse: %d | range: %-4d | start: %-4d | end: %-4d | pix: %-4d\n", reverse, _pixrange, _pixstart, _pixend+1, pixelPos);
      // uint16_t pPos = ( _pixstart + (_counter/scale) + i );
      uint16_t pPos = ( _pixstart + _counter ) * map(scale, 0, 255, 1, 6) + i;

      // uint8_t angle = (_varia == 255) ? 255 : (_varia > 127) ? 30 : 36 ;
// 30 36
      CRGB color = CHSV((uint8_t)( pPos   * ratio  )   , 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness));

      // CRGB color = CHSV((uint8_t)((60) * ratio), 255, 255);
      if (_isTw)  _ledarray_w[pixelPos] = color;
      else        _ledarray[pixelPos] = color;

      _counter = (_direction) ? _counter + 1 : _counter - 1;
    }
    
    
    reverse = (reverse)? false : true;
  }
  return true;
}


String EffectFx_ArmoireOutsideToInside::setDynCtrl(UIControl*_val) {set_dynCtrl(_val); return String();}
void EffectFx_ArmoireOutsideToInside::load(uint8_t strip_pos){
  setup(strip_pos);
  speed = 180;
}
bool EffectFx_ArmoireOutsideToInside::run(){
  
  LAMPPTRGET()->effectsLoop_TimerHue(_hueTimer*2.5);
  update();
      
  float hueInit = _hue_1V;
  float hue = hueInit;
  uint8_t segs = 0;
  uint8_t bri = LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness);
  uint8_t bri_max = bri;
  uint8_t bri_l = bri_max;
  uint8_t bri_r = bri_max;


  aramoire_segemntPtr_get()->get_sideSegmentCnt(aramoire_side_left, segs);
  for (int j = 0; j < segs / 2; ++j){
    aramoire_segemnts * ptr = aramoire_segemntPtr_get()->get_sideSegment(aramoire_side_left, j);
    for (uint32_t i = ptr->_start; i < ptr->_stop; ++i){
      if (_isTw)  _ledarray_w[i]  = CHSV(hue,255,bri_l);
      else        _ledarray[i]    = CHSV(hue,255,bri_l);      
      hue+=map(_varia, 0, 255, 0, 15);
      bri_l -= bri_max/((ptr->_stop-ptr->_start)*2)-1 ;
    }
  }
  hue = hueInit;
  for (int j = 0; j < segs / 2; ++j){
    aramoire_segemnts * ptr = aramoire_segemntPtr_get()->get_sideSegment(aramoire_side_right, j);
    for (uint32_t i = ptr->_stop; i > ptr->_start; --i){
      if (_isTw)  _ledarray_w[i]  = CHSV(hue,255,bri_r);
      else        _ledarray[i]    = CHSV(hue,255,bri_r);   
      hue+=map(_varia, 0, 255, 0, 15);
      bri_r -= bri_max/((ptr->_stop-ptr->_start)*2)-1 ;
    }
  }
  uint8_t hueStart = hue;
  for (int j = segs / 2; j < segs; ++j){
    aramoire_segemnts * ptr = aramoire_segemntPtr_get()->get_sideSegment(aramoire_side_left, j);
    for (uint32_t i = ptr->_start; i < ptr->_stop; ++i){
      if (_isTw)  _ledarray_w[i]  = CHSV(hue,255,bri_l);
      else        _ledarray[i]    = CHSV(hue,255,bri_l);   
      hue-=map(_varia, 0, 255, 0, 15);
      bri_l += bri_max/((ptr->_stop-ptr->_start)*2)-1 ;
    }
    // hue+=3.5;
  } 
  hue = hueStart;
  for (int j = segs / 2; j < segs; ++j){
    aramoire_segemnts * ptr = aramoire_segemntPtr_get()->get_sideSegment(aramoire_side_right, j);
    for (uint32_t i = ptr->_stop; i > ptr->_start; --i){
      if (_isTw)  _ledarray_w[i]  = CHSV(hue,255,bri_r);
      else        _ledarray[i]    = CHSV(hue,255,bri_r);   
      hue-=map(_varia, 0, 255, 0, 15);
      bri_r += bri_max/((ptr->_stop-ptr->_start)*2)-1 ;
    }
  }
  return true;
}

/*
bool EffectFx_glow::run(){
  unsigned long now = millis();
  if (now - _lastupdate > _delay) {
    _lastupdate = now;

    if (DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.isSegment()  ) { 

      uint8_t maxSeg = 0 ;
      DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_max(maxSeg);

      for (int A = 0; A < maxSeg; ++A) {

        uint8_t start   = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(A)->_start;
        uint8_t end     = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(A)->_end;  

        _pixstart = start;
        _pixend = end;

        _pixrange = (end - start+1);   
        _pixaoe = 1;



          if (_pixrange % 2 == 0 && _pixaoe % 2 != 0) {
            _pixaoe++;
          } else if (_pixrange % 2 != 0 && _pixaoe % 2 == 0) {
            _pixaoe--;
          }

          float ratio = _counter / 100.0;
          CRGB glowcolor;
          glowcolor.r = _color_fg.r * ratio;
          glowcolor.g = _color_fg.g * ratio;
          glowcolor.b = _color_fg.b * ratio;

          if (_direction == _forward) {
            _counter++;
            if (_counter >= 100) _direction = _reverse;
          } else {
            _counter--;
            if (_counter <= 0) {
              Serial.println("RESET");
              _direction = _forward;
              if (!_repeat) {
                _lastupdate = 0;
              }
            }
          }          

          int glow_area_half = (_pixrange - _pixaoe) / 2;
          for (int i = 0; i < glow_area_half ; i++) {
            uint8_t denom = glow_area_half + 1 - i;
            CRGB tempcolor = CRGB(glowcolor.r / denom, glowcolor.g / denom, glowcolor.b / denom);

            if (_isTw) {
              _ledarray_w[_pixstart + i] = tempcolor;
              _ledarray_w[_pixend - i] = tempcolor;      
            }
            else {
              _ledarray[_pixstart + i] = tempcolor;
              _ledarray[_pixend - i] = tempcolor;       
            }
            for (int i = 0; i < _pixaoe; i++) {
              if (_isTw) {
                _ledarray_w[_pixstart + glow_area_half + i] = glowcolor;
              }
              else {
                _ledarray[_pixstart + glow_area_half + i] = glowcolor;
              }     
            }
          }
      }
    }
  }
  return true;
}
*/