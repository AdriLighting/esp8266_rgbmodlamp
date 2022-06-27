#ifndef _TFT_H_
#define _TFT_H_
  #include "../libextern.h"
	

	#ifdef ALML_TFT
		#include "tfttxt.h"
	  #include "icons.h"
	  #include "fonts/ArialRoundedMTBold_10.h"
	  // #include "fonts/ArialRoundedMTBold_12.h"
	  // #include "fonts/ArialRoundedMTBold_14.h"
	  // #include "fonts/ArialRoundedMTBold_15.h"
	  // #include "fonts/ArialRoundedMTBold_16.h"
	  #include "fonts/SansSerif_plain_14.h"
	  #include "fonts/SansSerif_plain_13.h"
	  #include "fonts/SansSerif_plain_12.h"
	  // #include "fonts/SansSerif_plain_11.h"
	  #include "fonts/SansSerif_plain_10.h"	


		extern Adafruit_ILI9341				_almlTft;
		extern U8G2_FOR_ADAFRUIT_GFX	_almlU8g2;
		extern const uint8_t * font_default;	

		namespace almlTft {
			void wifiConnect_display();
			void wifiConnect_loop(int);
			void clear();
			void testTxt();
			void begin(uint8_t);
		}		
	#endif

#endif // TFT_H