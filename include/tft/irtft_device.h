#ifndef _IRTFT_DEVICE_H
#define _IRTFT_DEVICE_H
#ifdef ALML_IRTFT

#include "../libextern.h"
#include "tft.h"


class Irtft_effectListItem
{
public:
  String _name;
  uint8_t _id;  
  Irtft_effectListItem(const String &  v1, uint8_t v2) : _name(v1), _id(v2) {}
};
struct Irtft_effectValue
{
  String _name;
  uint8_t _value;
  uint8_t _state;
};

class Irtft_outputUpdate  {
  public:

    uint8_t _hsv_h  = 80;
    // uint8_t _hsv_hOld  = 0;
    uint8_t _hsv_s  = 255;
    uint8_t _hsv_v  = 255;

    uint8_t _rgb_r  = 255;
    uint8_t _rgb_g  = 255;
    uint8_t _rgb_b  = 0;

    uint8_t _tw     = 255;
    boolean _isTw = false; 

    uint8_t _bri  = 100;
    // uint8_t _briOld  = 0;

    boolean _select = true;
    boolean _onoff  = true; 
    boolean _onoffLast  = true; 

    // uint8_t   _type = 0;
    // uint8_t   _pin  = 0;
    // uint16_t  _size = 0; 
    // String  _name     = "device";

    uint8_t _tw_v   = 255;

    boolean _eff_activ = true;
    boolean _eff_activLast = true;

    uint8_t   _effen = 0;
    uint8_t   _effenLast = 0;


    // uint8_t _armoireEtage = 0;
    // boolean _isArmoire = false;

    Irtft_effectValue _eff_bri ;
    Irtft_effectValue _eff_speed ;
    Irtft_effectValue _eff_scale ;

    Irtft_outputUpdate(){};
    ~Irtft_outputUpdate(){};

};

class Irtft_display
{
  Tft_txt       * _txt_deviceIp = nullptr;
  Tft_lblValue  * _output_infos;


public:
  // Irtft_display();
  // ~Irtft_display();
protected:
  virtual void set_deviceIp(const String & ip);
  virtual void set_output();
  virtual void set_effect();
public:
  virtual void begin();  
};

class Irtft_device : public Irtft_display
{

  typedef std::function<void(const String & v1)> callback_function_t;

  LList<Irtft_effectListItem *>  _Effect_list;

  String _device_ip = "";

  String  _name = "";
  String  _cli = "";
  uint8_t _oc = 1;

  boolean _isRandDemoOn         = false;
  boolean _isLampOn             = false;
  boolean _isGlobalBrightnessOn = false;
  boolean _isFaderOn            = false;

  uint32_t output_lastUpdate = 0;

  void get_patternList();
  void get_outputJSON();

  boolean update_output(JsonObject & doc);
  boolean update_outputEffect(JsonObject & doc);

  Irtft_outputUpdate _Output;  
public:
  Irtft_device();
  ~Irtft_device(){};
  void update();
  void update_device(const String & doc);
  void set_outputLastUpdate();
  Irtft_outputUpdate * get_output() {return &_Output;}
  boolean get_effectName(uint8_t id, String & ret);


  callback_function_t _cb_udpSend = nullptr;
  void set_cb_udpSend(callback_function_t f) { _cb_udpSend = std::move(f); };


};

Irtft_device * Irtft_devicePtrGet();  
#endif
#endif // _IR_DEVICE_H