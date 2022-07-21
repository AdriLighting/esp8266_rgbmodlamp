#ifndef REMOTE_H
#define REMOTE_H

  #include "../libextern.h"
  #include "effectslist.h"
  #include "../alml_webserver.h"

  typedef enum : uint8_t {
    RIRMOD_INTERN = 0,
    RIRMOD_TFT
  } RemoteControlIrMod_t;

  class RemoteControl_ir  {
    typedef std::function<void(const String & v1)> callback_function_t;


    IRrecv* irrecv;
    //change pin in NpbWrapper.h

    decode_results results;

    unsigned long irCheckedTime         = 0;
    uint32_t      lastValidCode         = 0;
    byte          lastRepeatableAction  = ACTION_NONE;
    int           lastRepeatableValue   = 0;
    uint16_t      irTimesRepeated       = 0;
    uint8_t       lastIR6ColourIdx      = 0;
    int8_t        irPin                 = 4;
    uint8_t       irEnabled             = 1;

    RemoteControlIrMod_t _irMod = RIRMOD_INTERN;


    boolean decode(uint32_t);
    void decode44(uint32_t);
    void decode24(uint32_t);
    void changeBrightness(int8_t);
    void changeEffSpeed(int8_t);
    void changeHue(int8_t);
    void applyRepeatActions();

  public:
    RemoteControl_ir(){/*_EffectslistPtr = EffectslistPtrGet();*/};
    RemoteControl_ir(int8_t pin){irPin = pin;/*_EffectslistPtr = EffectslistPtrGet();*/};
    RemoteControl_ir(int8_t pin, RemoteControlIrMod_t mod){
      irPin = pin;
      _irMod = mod;
      // _EffectslistPtr = EffectslistPtrGet();
    };
    ~RemoteControl_ir(){};

    // Effectslist * _EffectslistPtr;
    callback_function_t _cb_udpSend = nullptr;
    void set_cb_udpSend(callback_function_t f) { _cb_udpSend = std::move(f); };

    RemoteControlIrMod_t get_mod() {return _irMod;}

    void    begin();
    boolean handle();

  };
  class RemoteControl_udp : public UdpMulti, public Udp {
    boolean multiSend_jsonDevice = true;
    unsigned long timerMultiSend_jsonDevice;      
  public:
    RemoteControl_udp(){};
    ~RemoteControl_udp(){};
    virtual void send_appi(boolean eff, boolean prog, boolean reset = false, uint8_t effLoad = false);
    virtual void send_toIp(const String & transmit_buffer, IPAddress ip, uint16_t port);
    virtual void handleJson(DynamicJsonDocument & doc, boolean udpMulti = true);
    virtual void handleJson(uint8_t op);

  protected:
    virtual void handle();
    virtual void begin();
    
  private:
  };
  // #ifdef ADS_PIN_IR
  // class RemoteControl : public  Effectslist, public RemoteControl_ir, public RemoteControl_udp { 
  // #else
  

  class RemoteControl : public  Effectslist, public RemoteControl_udp {
    RemoteControl_ir * _Ir_intern = nullptr;
    RemoteControl_ir * _Ir_TFT    = nullptr;
  public:
    RemoteControl();
    ~RemoteControl(){};
    void handle_ir(); 
    // void irSend(RemoteControl_ir * ptr, DynamicJsonDocument & doc);
    void handle_udp();
    void begin();
  };  



  RemoteControl * RemoteControlPtrGet();  

  void keyboard_print() ;
  void keyboard_getter(const String & v1) ;
#endif // REMOTE_H