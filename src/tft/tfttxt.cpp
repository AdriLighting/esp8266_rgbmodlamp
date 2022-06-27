#include "../../include/tft/tfttxt.h"


#ifdef ALML_TFT

namespace {
	int explode(String s, char sep, String * list) {
	    // Serial.printf("\nexplode s: %s\n", s.c_str() );
	    String  t           = s + sep;
	    int     str_index   = 0;
	    int     list_index  = 0;
	    int     j;
	    String  sub;
	    int size = t.length();
	    while (str_index < size ) {
	        j=t.indexOf(sep,str_index);
	        if (j!=-1) {
	            sub=t.substring(str_index, j);
	            // Serial.printf("explode index:%d %s\n", str_index,sub.c_str() );
	            list[list_index]=sub;
	            list_index++;
	            str_index=j+1;
	        }
	    }
	    list[list_index]="";
	    return list_index;
	}	
}

Tft_txt::Tft_txt(Adafruit_ILI9341 * tft, U8G2_FOR_ADAFRUIT_GFX u8g2){
	_tft 	= tft;
	_u8g2 	= u8g2;
}

void Tft_txt::textAlignment(TextAlignment alignment) {
	_alignment = alignment;
}
void Tft_txt::textColor(uint16_t c) {
	_tc = _bc = c;
}
void Tft_txt::textColor(uint16_t c, uint16_t bg) {
	_tc = c;
	_bc = bg;
}
void Tft_txt::textPos(int16_t x, int16_t y) {
	_text_x = x;
	_text_y = y;
}
void Tft_txt::textAlign(const String & text) {

	char buf[text.length()+2];
	text.toCharArray(buf, text.length() + 1);
	int16_t x1, y1;
	uint16_t w, h;	
	_tft->getTextBounds(buf, 0, 0, &x1, &y1, &w, &h);
	_text_x = (_canvas_w/2)-(w/2);
	// _text_y = (_canvas_h/2);
}
void Tft_txt::textPos(int16_t x, int16_t y, int16_t w, int16_t h){
	_text_x 	= x;
	_text_y 	= y;
	_text_w 	= w;
	_text_h 	= h;
}
void Tft_txt::text(const String & text) {
	_text = text;
}
void Tft_txt::textGet_xy(int16_t & x, int16_t & y) {
	x = _text_x;           		
	y = _text_y;           		
}

void Tft_txt::text_font(const GFXfont * font) {
	_font = font;
}


void Tft_txt::drawString_getWh(const String & text, uint16_t & w, uint16_t & h) {
	char buf[text.length()+2];
	text.toCharArray(buf, text.length() + 1);
	int16_t x1, y1;
	_tft->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
}

void Tft_txt::text_setup() {
	_tft->setTextColor(_tc, _bc);
	_tft->setFont(_font);                		
}

void Tft_txt::drawString(int16_t x, int16_t y, const String & text, uint16_t c, const GFXfont * font) {
	_tc 	= _bc = c;
	_font 	= font;
	_text 	= text;                     		
	_tft->setTextColor(_tc, _bc);
	_tft->setFont(_font);
	uint16_t w, h;
	drawString_getWh(text, w, h);	
	switch (_alignment) {
		case LEFT:
			x = x;
			break;
		case CENTER:
			x = x - w / 2;
			break;
		case RIGHT:
			x = x - w;
			break;
	}
	_text_x = x;
	_text_y = y;		
	_tft->setCursor(x, y);
	_tft->print(text);
}
void Tft_txt::drawString(int16_t x, int16_t y, const String & text, uint16_t c, uint16_t bc, const GFXfont * font) {
	_tc 	= c;
	_bc 	= bc;
	_font 	= font;
	_text 	= text;                     		
	_tft->setTextColor(_tc, _bc);
	_tft->setFont(_font);
	uint16_t w, h;
	drawString_getWh(text, w, h);
	switch (_alignment) {
		case LEFT:
			x = x;
			break;
		case CENTER:
			x = x - w / 2;
			break;
		case RIGHT:
			x = x - w;
			break;
	}	
	_text_x = x;
	_text_y = y;	
	_tft->fillRect(x, y - h -1, w + 1, h + 4, _bc);
	_tft->setCursor(x, y);
	_tft->print(text);
}
void Tft_txt::drawStringAling(const String & text, TextAlignment pos) {
	_tft->setFont(_font);

	uint16_t wo, ho;
	drawString_getWh(_text, wo, ho);
	
	int x = _text_x_ref;
	switch (pos) {
		case LEFT:
			x = x;
			break;
		case CENTER:
			x = x - wo / 2;
			break;
		case RIGHT:
			x = x - wo;
			break;
	}	
	_tft->fillRect(x, _text_y - ho -1, wo + 1, ho + 4, _bc);

	_text = text;

	textAlignment(pos);
	drawString(_text_x_ref, _text_y, text, _tc, _bc, _font);
	textAlignment(LEFT);

}

void Tft_txt::drawString(const String & text) {
	_tft->setFont(_font);
	_tft->setCursor(_text_x, _text_y);

	uint16_t wo, ho;
	drawString_getWh(_text, wo, ho);

	uint16_t w, h;
	drawString_getWh(text, w, h);

	if (_tc != _bc) {  
		if (wo>w) 	_tft->fillRect(_text_x, _text_y - ho -1, 	wo + 1, ho + 4, _bc);
		else  		_tft->fillRect(_text_x, _text_y - h -1, 	w + 1, 	h + 4, 	_bc);
	} 

	_text 	= text; 

	_tft->print(text);
}

void Tft_txt::drawString() {
	_tft->setFont(_font);
	_tft->setCursor(_text_x, _text_y);

	uint16_t w, h;
	drawString_getWh(_text, w, h);

	if (_tc != _bc) _tft->fillRect(_text_x, _text_y - h -1, w + 1, 	h + 4, 	_bc);

	_tft->print(_text);
}
void Tft_txt::drawString(const String & text, int16_t w2, int16_t h2) {
	_text 	= text;    
	_tft->fillRect(_text_x, _text_y - (h2/2)-4 , w2, h2, _bc);
	_tft->setFont(_font);
	_tft->setCursor(_text_x, _text_y);
	_tft->print(_text);
}


void Tft_txt::canvas_create(int16_t x, int16_t y, int16_t w, int16_t h){
	_canvas_x 	= x;
	_canvas_y 	= y;
	_canvas_w 	= w;
	_canvas_h 	= h;
	_text_y 	= _canvas_h-2;
	_canvas 	= new GFXcanvas1(_canvas_w, _canvas_h);
}
void Tft_txt::canvas_pos(int16_t x, int16_t y, int16_t w, int16_t h){
	_canvas_x 	= x;
	_canvas_y 	= y;
	_canvas_w 	= w;
	_canvas_h 	= h;
}
void Tft_txt::canvas_display() {
	_tft->drawBitmap(_canvas_x, _canvas_y, _canvas->getBuffer(), _canvas_w, _canvas_h, _tc , _bc);	
	_canvas->fillScreen(ILI9341_BLACK);
	_canvas->setFont(_font);
	_canvas->setCursor(_text_x,  _text_y);
	_canvas->print(_text);
}
void Tft_txt::canvas_update() {
	this->_canvas->fillScreen(ILI9341_BLACK);
	this->_canvas->setCursor(_canvas_x,  _canvas_h-2);
	this->_canvas->print(_text);
}

void Tft_txt::u8g2_setup() {
	_u8g2.setFontMode(_u8g2_mod);                		
	_u8g2.setFontDirection(_u8g2_dir);                		
	_u8g2.setFont(_u8g2_font);                		
	if(_u8g2_mod==0)_u8g2.setBackgroundColor(_bc);                		
	_u8g2.setForegroundColor(_tc);                		
}
	
	
void Tft_txt::u8g2_mod(uint8_t v) {
	_u8g2_mod = v;
}
void Tft_txt::u8g2_dir(uint8_t v) {
	_u8g2_dir = v;
}
void Tft_txt::u8g2_font(const uint8_t * f) {
	_u8g2_font = f;
}


void Tft_txt::u8g2_draw(int16_t x, int16_t y, const String & v, uint16_t tc, uint16_t bc, const uint8_t * f) {
	_u8g2_font = f;
	_u8g2.setFont(f);	

	_u8g2_mod = 0;
	_u8g2.setFontMode(_u8g2_mod);

	_bc = bc;
	_u8g2.setBackgroundColor(bc);	

	_tc = tc;
	_u8g2.setForegroundColor(tc);

	_u8g2.setFontDirection(_u8g2_dir);                		

	_text_x = x;
	_text_y = y;
	_u8g2.setCursor(x,y);  

	_text = v;                     		
	_u8g2.print(v);	
}
void Tft_txt::u8g2_draw(int16_t x, int16_t y, const String & v, uint16_t tc, const uint8_t * f) {
	_u8g2_font = f;
	_u8g2.setFont(f);

	_u8g2_mod = 1;
	_u8g2.setFontMode(_u8g2_mod);

	_tc = tc;
	_u8g2.setForegroundColor(tc);	
	
	_u8g2.setFontDirection(_u8g2_dir);                		

	_text_x = x;
	_text_y = y;	
	_u8g2.setCursor(x,y);   

	_text = v;             		
	_u8g2.print(v);	
}	
void Tft_txt::u8g2_draw(int16_t x, int16_t y, const String & v) {
	_text 		= v;             		
	_text_x 	= x;
	_text_y 	= y;	
	_u8g2.setCursor(x,y);                		
	_u8g2.print(v);	
}
void Tft_txt::u8g2_draw(const String & v) {
	_u8g2.setCursor(_text_x, _text_y);   
	_text = v;             		
	_u8g2.print(_text);	
}
void Tft_txt::u8g2_draw() {
	_u8g2.setCursor(_text_x, _text_y);                		
	_u8g2.print(_text);	
}

void Tft_txt::u8g2_drawRect(){
	char buffer_text[100];
	sprintf(buffer_text, "%s", _text.c_str());
	int16_t width = _u8g2.getUTF8Width(buffer_text);
	int16_t height = _u8g2.getFontAscent() - _u8g2.getFontDescent();
	_text_w = width;
	_text_h = height;
	_tft->fillRect(_text_x, _text_y-_u8g2.getFontAscent()-1, width, height+2, _bc);	
}
void Tft_txt::u8g2_drawRect(int16_t h, int16_t w){
	u8g2_setup();
	char buffer_text[100];
	sprintf(buffer_text, "%s", _text.c_str());
	int16_t width 	= _u8g2.getUTF8Width(buffer_text);
	int16_t height 	= _u8g2.getFontAscent() - _u8g2.getFontDescent();
	if (w<width)w = width;
	_tft->fillRect(_text_x, _text_y-_u8g2.getFontAscent()-3, w+5, height+6, _bc);	
	u8g2_draw(_text);
}
void Tft_txt::u8g2_drawRect(const String & v){
	// char buffer_text[100];
	u8g2_drawRect();
	_text = v;
	u8g2_drawRect();
	u8g2_draw(v);
}
void Tft_txt::u8g2_drawRect(const String & v, int16_t x, int16_t y){
	char buffer_text[100];
	sprintf(buffer_text, "%s", v.c_str());
	int16_t width 	= _u8g2.getUTF8Width(buffer_text);
	int16_t height 	= _u8g2.getFontAscent() - _u8g2.getFontDescent();

	if ((x != _text_x) || (y > _text_y)) {
		u8g2_drawRect();   
		_text_x = x;    
		_text_y = y; 		
		u8g2_setup();         		
	}	
	if ((width > _text_w) || (height > _text_h)) {
		_text = v;    
		u8g2_setup();         		
	}

	u8g2_drawRect();
	u8g2_draw(v);
}
void Tft_txt::u8g2_get_wh(int16_t & w, int16_t & h){
	char buffer_text[100];
	sprintf(buffer_text, "%s", _text.c_str());
	int16_t width 	= _u8g2.getUTF8Width(buffer_text);
	int16_t height 	= _u8g2.getFontAscent() - _u8g2.getFontDescent();
	w = width;
	h = height;
}



#define LBLVALUE_MAX 9

Tft_lblValue::Tft_lblValue(const String & name, Adafruit_ILI9341 * tft, U8G2_FOR_ADAFRUIT_GFX u8g2){
	_tft = tft;
	_u8g2 = u8g2;
	// _ui = ui;
	_id = name;
}

void Tft_lblValue::lbl_get(){
	Serial.printf_P(PSTR("\n[lbl_get] [%s]\n"), _id.c_str());

	String split_1[2];

	for (int i = 0; i < _lines_cnt; ++i) {
		explode(_lines[i], _line_sep, split_1);
		Serial.printf_P(PSTR("\t[%d] name: %-20s : %s\n"), i, split_1[0].c_str(), split_1[1].c_str());
	}
}
void Tft_lblValue::lines_clear(){
	for (int i = 0; i < LBLVALUE_MAX; ++i) {
		_lines[i]="";
	}
	_lines_cnt=0;
}


int8_t Tft_lblValue::line_add(const String & str){
	if (str == "") return -1;

	for (int8_t i = 0; i < LBLVALUE_MAX; ++i)
	{
		if(_lines[i] == ""){
			_lines[i] = str;
			_lines_cnt++;
			return i;
		}
	}
	return -1;
}
int8_t Tft_lblValue::line_add(const String & name, const String & value){
	if ((name == "") && (value == "")) return -1;
	String result = name+_line_sep+value;
	return line_add(result);
}
int8_t Tft_lblValue::line_add(int8_t pos, const String & name, const String & value){
	if ((name == "") && (value == "")) 	return -1;
	if (pos >= LBLVALUE_MAX) return -1;
	String result = name+_line_sep+value;
	if (pos >= _lines_cnt) return line_add(result);
	else {
		_lines[pos] = name+_line_sep+value;
		return pos;
	}
}
int8_t Tft_lblValue::line_add(int8_t pos, const String & str){
	if (str == "") 						return -1;
	if (pos >= LBLVALUE_MAX)	return -1;
	if (pos >= _lines_cnt)		return line_add(str);
	else {
		_lines[pos] = str;
		return pos;
	}
}

void Tft_lblValue::lineSplit_add(const String & str){
	// Serial.printf("\n[lineSplit_add]\n%s\n", str.c_str());

	String 	split_1[50];

	explode(str, _lines_sep, split_1);

	for (int16_t i = 0; i < LBLVALUE_MAX; ++i)
	{
		String line = split_1[i];
		if (line != ""){
			line_add(line);
			// Serial.printf("[%d] %s\n", pos, line.c_str());
		}
	}
}

void Tft_lblValue::xy(int16_t x, int16_t y) {
	_x = x;
	_y = y;
}
void Tft_lblValue::string_getWh(const String & text, uint16_t & w, uint16_t & h) {
	char buf[text.length()+2];
	text.toCharArray(buf, text.length() + 1);
	int16_t x1, y1;
	_tft->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
}

void Tft_lblValue::rect_clear(uint16_t c){


	// int16_t x1, y1;
	// uint16_t w, h;

	String split_1[2];

	char buffer_text[200];

	if (_u8g2_text) {
		_u8g2.setFontMode(1);                		
		_u8g2.setFont(_u8g2Font);    
		_u8g2.setFontDirection(0);  		
	} else {
		_tft->setFont(_font);
	}
	
	uint16_t width, height;

	int y = _y;

	for (int i = 0; i < _lines_cnt; ++i) {
		int cnt = explode(_lines[i], _line_sep, split_1);
		if (cnt < 2) continue;

		if (_u8g2_text) {
			sprintf(buffer_text, "%s", split_1[1].c_str());

			width 	= _u8g2.getUTF8Width(buffer_text);
			height 	= _u8g2.getFontAscent() - _u8g2.getFontDescent();	

		} else {
			_tft->setCursor(_x, y);			
			string_getWh(split_1[1], width, height);
		}

			_tft->fillRect(_x-2, y-(_h_max)+3, _w_name_max+20+width, _h_max, c);

		y+= _h_max+_h_diff;
	}
}
void Tft_lblValue::rect_create(uint16_t c){


	String split_1[2];

	char buffer_text[200];

	if (_u8g2_text) {
		_u8g2.setFontMode(1);                		
		_u8g2.setFont(_u8g2Font);    
		_u8g2.setFontDirection(0);  		
	} else {
		_tft->setFont(_font);
	}
	
	uint16_t width, height;

	int y = _y;

	for (int i = 0; i < _lines_cnt; ++i) {
		int cnt = explode(_lines[i], _line_sep, split_1);
		if (cnt < 2) continue;

		if (_u8g2_text) {
			sprintf(buffer_text, "%s", split_1[1].c_str());

			width 	= _u8g2.getUTF8Width(buffer_text);
			height 	= _u8g2.getFontAscent() - _u8g2.getFontDescent();	
		} else {
			
			_tft->setCursor(_x, y);			
			string_getWh(split_1[1], width, height);
		}

		_tft->fillRect(_x-2, 				y-(_h_max)+3, 	_w_name_max+2, 	_h_max, c);
		_tft->fillRect(_x+_w_name_max+10, 	y-(_h_max)+3, 	width+3, 		_h_max, c);

		y+= _h_max+_h_diff;
	}
}

void Tft_lblValue::rect_create(int16_t x, int16_t y, uint16_t c){
	_x = x;
	_y = y;
	rect_create(c);
}

void Tft_lblValue::lines_create(int16_t x, int16_t y){
	String split_1[10];
	_x = x;
	_y = y;
	int16_t y2 = _y;

	if (_u8g2_text) {
		_u8g2.setFontMode(1);                		
		_u8g2.setFont(_u8g2Font);    
		_u8g2.setFontDirection(0);  		
	} else {
		_tft->setFont(_font);
	}
	
	Tft_txt temp(_tft, _u8g2);

	for (int8_t i = 0; i < _lines_cnt; ++i) {
		int8_t cnt = explode(_lines[i], _line_sep, split_1);
		if (cnt < 2) continue;

		if (_u8g2_text) {
			temp.u8g2_draw(	_x, 				y2, split_1[0], 	_c_name, 	_u8g2Font);
			temp.u8g2_draw(	_x+_w_name_max+2, 	y2, _sep, 			_c_sep, 	_u8g2Font);		
			temp.u8g2_draw(	_x+_w_name_max+12,	y2, split_1[1], 	_c_value, 	_u8g2Font);	
		} else {
			temp.drawString(	_x, 				y2, split_1[0], 	_c_name, 	_font);
			temp.drawString(	_x+_w_name_max+2, 	y2, _sep, 			_c_sep, 	_font);		
			temp.drawString(	_x+_w_name_max+12,	y2, split_1[1], 	_c_value, 	_font);	
		}

		y2+= _h_max+_h_diff;
	}
}
#endif
