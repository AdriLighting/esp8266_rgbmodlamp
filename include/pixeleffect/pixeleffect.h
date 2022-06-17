#ifndef PIXELEFFECT_H
#define PIXELEFFECT_H

	#include "../fastled/base.h"  
	// #include "../pixel/NeoPixelEffects.h"
	#include "../pixel/w2812fx/fx.h"
	#include "../pixel/effectcore.h"
	#include "../device/device.h"

	class EffectFx_neo : public EffectCalc {
	protected:
		CRGBPalette16 			_gTargetPalette;		
		int 								_pPos 				= 0;
		unsigned long 			_timer_toward	=	 0; 
		WS2812FX						* _neoFxxPtr	= nullptr;
		CRGBW								* ledarray_w	= nullptr;	
		CRGB								* ledarray		= nullptr;	
		uint16_t						numLeds				= 0;			
		WS2812FX_setting 		neoSetting;
		uint8_t 						oldBpm 				= 0;	
		uint8_t 						oldBri 				= 0;	
		boolean 						statu 				= false;	
		boolean 						show 					= true;
		uint8_t 						_strip_pos;		
		OUTPUT_EFFECTTYPE  	_effectType;
		uint8_t							_varia 				= 60;
		uint8_t							_hue_1 				= 0;
		uint8_t							_hue_2 				= 0;
		uint8_t							_mod 					= 0;
	public:
		~EffectFx_neo();
		virtual void setup( uint8_t strip_pos , OUTPUT_EFFECTTYPE mod = OET_NONE);
		virtual void update();
		virtual void instance();
		virtual void set_dynCtrl(UIControl*_val);
		
	};
	class EffectFx_mode :  public EffectFx_neo {
	public:
		EffectFx_mode(uint8_t mod = 0){_mod = mod;};
	  void load(uint8_t) override;
	  bool run() override;
	  String setDynCtrl(UIControl*_val) override;
	};


	class EffectFx_comm : public EffectCalc {
	protected:
		OUTPUT_EFFECTTYPE _effectType = OET_HUETIMER;
		CRGBW			* _ledarray_w			= nullptr;	
		CRGB			* _ledarray				= nullptr;	
		uint16_t	_numLeds					= 0;
		uint8_t		_strip_pos				= 0;		
		boolean		_isTw 						= false;
		boolean		_isSegment 				= false;
		boolean		_isSegmentEnabled = true;
		uint8_t		_mod 							= 0;
		uint8_t		_maxSeg 					= 1 ;
    boolean		_reverseStart 		= true;
    boolean		_active 					= false;
    uint32_t	_lastupdate 			= 0;
    // uint32_t	_speed 						= 60;
		uint8_t		_varia 						= 60;
		uint8_t		_hue_1 						= 0;
		uint8_t		_hue_1V 					= 0;
		uint32_t	_hueTimer					= 0;
		// uint8_t		_hue_2 						= 0;
		// uint8_t		_hue_2V 					= 0;
		uint8_t		_fade 						= 0;
		uint8_t		_size 						= 0;
		uint8_t		_rep 							= 0;

  public:
  	~EffectFx_comm(){};
		virtual void setup(uint8_t strip_pos);
		boolean dryrun(float n, uint8_t delay = 16) ;	
		boolean dryrun(uint8_t s, float n, uint8_t delay = 16) ;	
		virtual void set_dynCtrl(UIControl*_val);
		virtual void update();
		
	};

	class EffectFx_comets : public EffectFx_comm {
    boolean _direction = false;
    uint8_t _hue = 80;
    int
      _pixstart,            
      _pixend,              
      _pixrange,            
      _counter;
  	public:
  		EffectFx_comets(uint8_t mod = 0){_mod = mod;};
	    void load(uint8_t) override;
	    bool run() override;
	    String setDynCtrl(UIControl*_val) override;
	};	

	class EffectFx_bounce : public EffectFx_comm {
    boolean _direction = true;
    uint8_t _hue = 80;
    int
      _pixstart,            
      _pixend,              
      _pixrange,            
      _pixaoe,              
      _counter;
  	public:
  		EffectFx_bounce(uint8_t mod = 0){_mod = mod;};
	    void load(uint8_t) override;
	    bool run() override;
	    String setDynCtrl(UIControl*_val) override;
	};	

	class EffectFx_ruggle : public EffectFx_comm {
    boolean _direction = true;
    uint8_t _hue = 80;
    int
      _pixstart,            
      _pixend,              
      _pixrange,            
      _pixaoe,              
      _counter;
  	public:
  		EffectFx_ruggle(uint8_t mod = 0){_mod = mod;};
	    void load(uint8_t) override;
	    bool run() override;
	    String setDynCtrl(UIControl*_val) override;
	};	

	class EffectFx_rainbowWave : public EffectFx_comm {
    boolean _direction = false;
    int
      _pixstart,            
      _pixend,              
      _pixrange,            
      _pixaoe,              
      _counter;
  	public:
  		EffectFx_rainbowWave(uint8_t mod = 0){_mod = mod;};
	    void load(uint8_t) override;
	    bool run() override;
	    String setDynCtrl(UIControl*_val) override;
	};

	class EffectFx_Comet : public EffectFx_comm {
		CRGB _color_fg = CRGB(0,255,0);
		uint8_t _subtype = 1;          // Defines sub type to be used
    boolean _direction = false;
    int
      _pixstart,            
      _pixend,              
      _pixrange,            
      _pixaoe,              
      _counter;
  	public:
  		EffectFx_Comet(uint8_t mod = 0){_mod = mod;};
	    void load(uint8_t) override;
	    bool run() override;
	    String setDynCtrl(UIControl*_val) override;

	};

	class EffectFx_ArmoireOutsideToInside : public EffectFx_comm {

  	public:
  		EffectFx_ArmoireOutsideToInside(uint8_t mod = 0){_mod = mod;};
	    void load(uint8_t) override;
	    bool run() override;
	    String setDynCtrl(UIControl*_val) override;

	};

  enum EffectStatus {
    INACTIVE,
    ACTIVE,
    NUM_EFFECTSTATUS
  };
	class EffectFx_glow : public EffectCalc {
		CRGBW				* _ledarray_w	= nullptr;	
		CRGB				* _ledarray		= nullptr;	
		uint16_t			_numLeds			= 0;
		uint8_t 			_strip_pos;		
		boolean				_isTw;
		uint8_t				_mod = 0;
		uint8_t 			_maxSeg = 1 ;


    bool _forward =true;
    bool _reverse =false;
    boolean _reverseStart = false;
    EffectStatus _status;


      CRGB _color_fg = CRGB(0,255,0);
      CRGB _color_bg;
      int
        _pixstart,            // First NeoPixel in range of effect
        _pixend,              // Last NeoPixel in range of effect
        _pixrange,            // Length of effect area
        _pixaoe,              // The length of the effect that takes place within the range
        _pixcurrent,          // Head pixel that indicates current pixel to base effect on
        _counter;
      uint8_t _subtype;          // Defines sub type to be used
      uint8_t _bri = 255;          // Defines sub type to be used
      bool
        _repeat,              // Whether or not the effect loops in area
        _direction;           // Whether or not the effect moves from start to end pixel
      unsigned long
        _lastupdate,          // Last update time, in milliseconds since sys reboot
        _delay;               // Period at which effect should update, in milliseconds
  	public:
  		EffectFx_glow(uint8_t mod = 0){_mod = mod;};
	    void load(uint8_t) override;
	    bool run() override;
	    String setDynCtrl(UIControl*_val) override;

	};						
#endif // PIXELEFFECT_H