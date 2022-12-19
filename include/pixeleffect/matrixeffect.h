#ifndef MATRIXEFFECT_H
	#define MATRIXEFFECT_H

#ifdef ALS_LAMPMATRIX

  #include "../fastled/base.h"  
  #include "../pixel/effectcore.h"
  #include "../device/device.h"
	#include "pixeleffect.h"
	#include "../pixel/effectmath.h"
	#include "../display/lamp.h"


	class MEffectRainbow : public EffectFx_comm {
	private:
	    float hue; // вещественное для малых скоростей, нужно приведение к uint8_t по месту

	    bool rainbowHorVertRoutine(bool isVertical);
	    bool rainbowDiagonalRoutine(CRGB *leds);

	public:
      MEffectRainbow(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;
      String setDynCtrl(UIControl*_val) override;
	};

	// ------------- светлячки -------------
	class EffectLighters : public EffectFx_comm {
	protected:
	    uint8_t cnt = 1;
	    bool subPix = false;
	    uint16_t lightersIdx;
	    float lightersSpeed[2U][LIGHTERS_AM];
	    uint8_t lightersColor[LIGHTERS_AM];
	    float lightersPos[2U][LIGHTERS_AM];
	    byte light[LIGHTERS_AM];
	private:
	    bool lightersRoutine(CRGB *leds);
	    String setDynCtrl(UIControl*_val) override;
	public:

      void load(uint8_t) override;
      bool run() override;
	};

	// ------------- Эффект "New Матрица" ---------------
	class MEffectMatrix : public EffectLighters {
	private:
	    boolean timer = false;
	    boolean colorCycle = false;
	    bool matrixRoutine(CRGB *leds);
	    uint8_t _scale = 1;
	    byte gluk = 1;
	    uint8_t hue;
	    bool randColor = false;
	    bool white = false;
	    float count;
	    String setDynCtrl(UIControl*_val) override;
	public:
			MEffectMatrix(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;
	};



#endif

#endif // MATRIXEFFECT_H