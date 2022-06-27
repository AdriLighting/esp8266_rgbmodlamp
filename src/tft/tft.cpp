#include "../../include/tft/tft.h"


#ifdef ALML_TFT

	Adafruit_ILI9341		_almlTft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
	U8G2_FOR_ADAFRUIT_GFX	_almlU8g2;
	const uint8_t 			* font_default = u8g2_font_helvR14_tf;

	Tft_txt 			* _txt_deviceIp = nullptr;
	Tft_lblValue 	* _screen_output_infos;

	namespace almlTft {
		uint16_t color565(Adafruit_ILI9341 * _tft, uint8_t r, uint8_t g, uint8_t b){
			return _tft->color565(r, g, b);
		}


		void begin(uint8_t rotation){
		  _almlTft.begin();
		  _almlTft.setRotation(rotation);
		  _almlTft.fillScreen(ILI9341_BLACK);
		  _almlU8g2.begin(_almlTft); 

			Tft_txt temp(&_almlTft, _almlU8g2);
			temp.textAlignment(CENTER);
			temp.drawString(_almlTft.width()/2, 25, "Connection en cour", _almlTft.color565(160, 200, 200), 0, &ArialRoundedMTBold_10);

		}
		void wifiConnect_display() {
			_almlTft.fillScreen(ILI9341_BLACK);

			Tft_txt temp(&_almlTft, _almlU8g2);
			temp.textAlignment(CENTER);
			temp.drawString(_almlTft.width()/2, 35, "Connection en cour", _almlTft.color565(160, 200, 200), 0, &ArialRoundedMTBold_10);
			temp.textAlignment(LEFT);
			_almlTft.drawBitmap(110, 65, wifiBitmap, 100, 100, _almlTft.color565(30, 30, 160));
		}

		uint16_t _ox;
		void wifiConnect_loop(int x) {
			uint8_t _l = (_almlTft.width()/2) - 75 ;
			uint8_t _t = 180;
			uint8_t _w = 150;
			// uint8_t _h = 30;
			uint8_t _ball_dia = 10;
			x = map(x/30, 0, 255, _l, (_w*30)+_l);
			if(_ox!=x) {
				_almlTft.fillCircle(_ox, _t, _ball_dia, 0); 
				_almlTft.fillCircle(x, _t, 	_ball_dia, _almlTft.color565(50,	50, 50)); 
				_almlTft.drawCircle(x, _t, 	_ball_dia, _almlTft.color565(50,	50, 50));
				_ox = x;
			}
		}
		void clear(){_almlTft.fillScreen(ILI9341_BLACK);}
		
		uint32_t testTimer = 0;
		void testTxt(){
			// static uint8_t cnt_1 = 0;
			// if ( (millis() - testTimer) < 500 ) return;
			// testTimer = millis();
			// // TestTxt_1->u8g2_drawRect("test line 1 - " + String(cnt_1));
			// String s;
			// s = (cnt_1 < 15) ? "test line 2 - "  : "line - " ;
			// TestTxt_2->u8g2_drawRect(s + String(cnt_1++));
			// // TestTxt_3->u8g2_drawRect("test line 3 - " + String(cnt_1));

		}
	}

#endif