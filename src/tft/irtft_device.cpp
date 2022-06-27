
#ifdef ALML_IRTFT
#include "../../include/tft/irtft_device.h"
#include "../../include/display/remote.h"
#include "../../include/tft/tft.h"	
namespace {
	String  _string_to_split(String name, String value, String sep){
	    return name + sep + value;
	}
	String  _string_to_split(String name, String value){
	    return name + ";" + value + "\n";
	}		
}
Irtft_device * _Irtft_devicePtr = nullptr;
Irtft_device * Irtft_devicePtrGet() { return _Irtft_devicePtr;}
Irtft_device::Irtft_device(){_Irtft_devicePtr = this;}
void Irtft_device::get_patternList(){
	WiFiClient client;
	HTTPClient http;

	http.begin(client, "http://8.8.8.8/apapi");
	http.addHeader("Content-Type", "application/json");

	DynamicJsonDocument request(1024);
	request.createNestedArray(F("set")); 
	JsonArray getter = request.createNestedArray(F("get")); 
	getter.add("list_lbid");
	request[F("op")] = 0;
	request[F("type")] = "HTTP_POST";

	String httpRequestData;    
	// serializeJsonPretty(request, Serial) ; Serial.println();     
	serializeJson(request, httpRequestData) ;   

	// int 		httpResponseCode 	= http.POST(httpRequestData);
	http.POST(httpRequestData);
	String 	payload 					= http.getString(); 

	DynamicJsonDocument effectList(3000);
	deserializeJson(effectList, payload);
	JsonArray arr = effectList[F("list_lbid")][F("items")];
	for (uint8_t i = 0; i < arr.size(); i++) { 
		JsonArray item = arr[i];
		_Effect_list.add(new Irtft_effectListItem(item[0].as<String>(), item[1].as<uint8_t>()));
	}
	for (int i = 0; i < _Effect_list.size(); ++i) {
		Serial.printf("%3d | name: %-30s - id: %-3d\n", i, _Effect_list[i]->_name.c_str(), _Effect_list[i]->_id);
	}      

	http.end();  
}

void Irtft_device::update(){
	// static boolean getEffectList = false;

	if (_Effect_list.size() == 0) get_patternList();
	if (_device_ip == "") {
		String ret;
		al_httptools::get_httpdata(ret, "http://8.8.8.8/wcapi?&ALSI=6");
		DynamicJsonDocument doc(1024);
		deserializeJson(doc, ret);
		_device_ip = doc[F("system_infos")][F("network")][F("Station IP")].as<String>();
		Serial.println(_device_ip);
		set_deviceIp(_device_ip);
	}
	get_outputJSON();
}

void Irtft_device::get_outputJSON(){
	if (( millis() - output_lastUpdate ) > 3500) {
		output_lastUpdate = millis();
		DynamicJsonDocument request(1024);
		request[F("op")] 		= 8;
		request[F("from")] 	= 1;		
	  String json;
	  serializeJson(request, json);
	  // serializeJsonPretty(request, Serial);Serial.println();
	  // _Udp.send_toIp(json, {8,8,8,8}, 9100);  	
	  if (_cb_udpSend) _cb_udpSend(json);  	
	}
}

void Irtft_device::set_outputLastUpdate(){
	output_lastUpdate = millis();
}

boolean Irtft_device::get_effectName(uint8_t id, String & ret){
  for (int i = 0; i < _Effect_list.size(); ++i) {
    if (id == _Effect_list[i]->_id) {
    	ret = _Effect_list[i]->_name;
      return true;
    }
  }
  return false;
}

boolean Irtft_device::update_output(JsonObject & output) {
	boolean update = false;

	uint8_t v1;
	boolean v2;

	v1 = output[F("0")].as<uint8_t>();
	if (_Output._hsv_h 				!= 	v1) 	update = true;
	_Output._hsv_h 						=		v1;

	v1 = output[F("1")].as<uint8_t>();
	if (_Output._hsv_s 				!= 	v1) 	update = true;
	_Output._hsv_s 						= 	v1;

	v1 = output[F("2")].as<uint8_t>();
	if (_Output._hsv_v 				!=	v1) 	update = true;
	_Output._hsv_v 						= 	v1;

	v1 = output[F("3")].as<uint8_t>();
	if (_Output._rgb_r 				!= 	v1) 	update = true;
	_Output._rgb_r 						= 	v1;

	v1 = output[F("4")].as<uint8_t>();
	if (_Output._rgb_g 				!= 	v1) 	update = true;
	_Output._rgb_g 						= 	v1;

	v1 = output[F("5")].as<uint8_t>();
	if (_Output._rgb_b 				!= 	v1) 	update = true;
	_Output._rgb_b 						= 	v1;

	v1 = output[F("6")].as<uint8_t>();
	if (_Output._tw 					!=	v1) 	update = true;
	_Output._tw 							=		v1;

	v2 = output[F("7")].as<uint8_t>();
	if (_Output._isTw 				!=	v2)		update = true;
	_Output._isTw 						=		v2;

	v1 = output[F("19")].as<uint8_t>();
	if (_Output._bri 					!=	v1) 	update = true;
	_Output._bri 							=		v1;

	v2 = output[F("9")].as<uint8_t>();
	if (_Output._select				!=	v2)		update = true;
	_Output._select						=		v2;

	v2 = output[F("10")].as<uint8_t>();
	if (_Output._onoff 				!=	v2)		update = true;
	_Output._onoff 						=		v2;

	v1 = output[F("15")].as<uint8_t>();
	if (_Output._tw_v 				!=	v1) 	update = true;
	_Output._tw_v 						= 	v1;

	v2 = output[F("16")].as<uint8_t>();
	if (_Output._eff_activ		!=	v2)		update = true;
	_Output._eff_activ				= 	v2;

	v1 = output[F("17")].as<uint8_t>();
	if (_Output._effen 				!=	v1)		update = true;
	_Output._effen 						= 	v1;

	return update;
}

boolean Irtft_device::update_outputEffect(JsonObject & effects) {
	uint8_t size = effects[F("size")].as<uint8_t>();
	JsonObject ctrls = effects[F("ctrls")];
	boolean result = false;
	for(int i = 0; i < size; ++i) {
		if (ctrls[String(i)][F("0")].as<uint8_t>() == 0) {
			if (_Output._eff_bri._value !=  ctrls[String(i)][F("3")].as<uint8_t>()) result = true;
			if (_Output._eff_bri._state !=  ctrls[String(i)][F("7")].as<uint8_t>()) result = true;
			_Output._eff_bri._value	= ctrls[String(i)][F("3")].as<uint8_t>();
			_Output._eff_bri._name	= ctrls[String(i)][F("2")].as<String>();
			_Output._eff_bri._state	= ctrls[String(i)][F("7")].as<uint8_t>();
		}
		if (ctrls[String(i)][F("0")].as<uint8_t>() == 1) {
			if (_Output._eff_speed._value !=  ctrls[String(i)][F("3")].as<uint8_t>()) result = true;
			if (_Output._eff_speed._state !=  ctrls[String(i)][F("7")].as<uint8_t>()) result = true;
			_Output._eff_speed._value	= ctrls[String(i)][F("3")].as<uint8_t>();
			_Output._eff_speed._name	= ctrls[String(i)][F("2")].as<String>();
			_Output._eff_speed._state	= ctrls[String(i)][F("7")].as<uint8_t>();
		}
		if (ctrls[String(i)][F("0")].as<uint8_t>() == 2) {
			if (_Output._eff_scale._value !=  ctrls[String(i)][F("3")].as<uint8_t>()) result = true;
			if (_Output._eff_scale._state !=  ctrls[String(i)][F("7")].as<uint8_t>()) result = true;
			_Output._eff_scale._value	= ctrls[String(i)][F("3")].as<uint8_t>();
			_Output._eff_scale._name	= ctrls[String(i)][F("2")].as<String>();
			_Output._eff_scale._state	= ctrls[String(i)][F("7")].as<uint8_t>();
		}

	}
	return result;
}

void Irtft_device::update_device(const String & s) {
	DynamicJsonDocument doc(3072);
	DeserializationError error = deserializeJson(doc,s);
	if (error) return;
	
	if (doc.containsKey(F("lamp"))) {
		_isRandDemoOn 				= doc[F("lamp")][F("1")	].as<bool>();
		_isLampOn 						= doc[F("lamp")][F("2")	].as<bool>();
		_isGlobalBrightnessOn = doc[F("lamp")][F("3")	].as<bool>();
		_isFaderOn 						= doc[F("lamp")][F("4")	].as<bool>();
	}

	if (!doc.containsKey(F("outputs"))) return;
	if (!doc[F("outputs")][F("0")]) return;
	JsonObject output = doc[F("outputs")][F("0")];
	if (update_output(output)) {
		Serial.printf_P(PSTR("_select: %d - _onoff: %d\n"), 
			_Output._select, _Output._onoff
		);		
		set_output();
	}
	if (_Output._effenLast != _Output._effen || _Output._onoffLast != _Output._onoff  || _Output._eff_activLast != _Output._eff_activ ) {
		_Output._effenLast 			= _Output._effen;
		_Output._onoffLast 			= _Output._onoff;
		_Output._eff_activLast	= _Output._eff_activ;
		_Output._eff_bri._value 	= 0;
		_Output._eff_speed._value = 0;
		_Output._eff_scale._value = 0;
	}
	JsonObject effects = doc[F("outputs")][F("0")][F("effects")];
	if (!effects) 						return;
	if (!_Output._onoff) 			return;
	if (!_Output._eff_activ) 	return;
	if (update_outputEffect(effects)) {
		set_effect();
	}
}


void Irtft_display::begin(){
  _almlTft.begin();
  _almlTft.setRotation(3);
  _almlTft.fillScreen(ILI9341_BLACK);
  _almlU8g2.begin(_almlTft); 	

	_output_infos = new Tft_lblValue("Output", &_almlTft, _almlU8g2);
	_output_infos->_c_sep 			= _almlTft.color565(255, 255, 0);;
	_output_infos->_c_value 		= _almlTft.color565(255, 255, 255);
	_output_infos->_c_name 			= _almlTft.color565(200, 200, 200);
	_output_infos->_u8g2Font 		= font_default;	
	_output_infos->_w_name_max	= 100;	
	_output_infos->_font 				= &SansSerif_plain_10;	
	_output_infos->_u8g2_text 	= true;	
}
void Irtft_display::set_deviceIp(const String & ip){
	Tft_txt temp_2(&_almlTft, _almlU8g2);
	if (ip != "(IP unset)") {
		temp_2.drawString(5, 30, "La lampe est connecter au routeur",		_almlTft.color565(0, 		255, 0), 		&SansSerif_plain_14);
		temp_2.drawString(5, 50, "L'adresse de la lampe est: "  + ip,		_almlTft.color565(255, 	255, 255), 	&SansSerif_plain_13);
	} else {
		temp_2.drawString(5, 30, "La lampe est en point d'acces",				_almlTft.color565(0, 		255, 0), 		&SansSerif_plain_14);
		temp_2.drawString(5, 50, "L'adresse de la lampe est: 8.8.8.8",	_almlTft.color565(255, 	255, 255), 	&SansSerif_plain_14);
	}
		
	_almlTft.startWrite();
	_almlTft.writeFastHLine(40, 65, _almlTft.width()-80, _almlTft.color565(127, 127, 127));
	_almlTft.endWrite();
}



void Irtft_display::set_output(){
	Tft_txt temp_2(&_almlTft, _almlU8g2);

	_almlTft.fillRect(
			0, 
			70, 
			_almlTft.width(), 
			85, 
			ILI9341_BLACK
		);
		_almlTft.fillRect(
				5, 
				140, 
				_almlTft.width(), 
				_almlTft.height()-140, 
				ILI9341_BLACK
			);	

	Irtft_outputUpdate * ptr = Irtft_devicePtrGet()->get_output();
	_almlTft.drawBitmap(5, 80, lightBitmap, 40, 40, ptr->_onoff ? _almlTft.color565(0,255,0) : _almlTft.color565(255,0,0));
	if (ptr->_onoff) {
		if (ptr->_eff_activ) {
			String name;
			Irtft_devicePtrGet()->get_effectName(ptr->_effen, name);
			_almlTft.drawBitmap(55, 80, epd_bitmap, 40, 40, _almlTft.color565(255,255,255));
			temp_2.drawString(105, 100, "Effet en cour:", _almlTft.color565(255, 255, 255), &SansSerif_plain_12);
			temp_2.drawString(105, 115, name, _almlTft.color565(255, 255, 255), &SansSerif_plain_12);
		}else{
			CRGB newColor = CHSV(ptr->_hsv_h, ptr->_hsv_s, 255);
			_almlTft.drawBitmap(55, 80, rgbsBitmap, 40, 40, (!ptr->_isTw) ? _almlTft.color565(newColor.r, newColor.g, newColor.b) : _almlTft.color565(255, 255, 255));
			temp_2.drawString(105, 115, "Couleur statique", _almlTft.color565(255,255,255), &SansSerif_plain_14);


			String str;
												str += _string_to_split("Luminositée",	String(ptr->_bri));
			if (!ptr->_isTw) 	str += _string_to_split("Hue", 					String(ptr->_hsv_h));
			str+="\n";
			_output_infos->lines_clear();	
			_output_infos->lineSplit_add(str);	
			_output_infos->xy(5,140);	
			_output_infos->rect_clear(0);	
			_output_infos->lines_create(5,140);				
		}
	} else {
			_almlTft.fillRect(
					5, 
					140, 
					_almlTft.width(), 
					_almlTft.height()-140, 
					ILI9341_BLACK
				);			
	}
}

void Irtft_display::set_effect(){
	Tft_txt temp_2(&_almlTft, _almlU8g2);

	_almlTft.fillRect(
			5, 
			140, 
			_almlTft.width(), 
			_almlTft.height()-140, 
			ILI9341_BLACK
		);

	Irtft_outputUpdate * ptr = Irtft_devicePtrGet()->get_output();
	String str;
	if (ptr->_eff_bri._state == 0) 		str+=_string_to_split("Luminositée",	String(ptr->_eff_bri._value));
	if (ptr->_eff_speed._state == 0) 	str+=_string_to_split("Vitesse", 			String(ptr->_eff_speed._value));
	if (ptr->_eff_scale._state == 0) 	str+=_string_to_split("Echelle", 			String(ptr->_eff_scale._value));
	str+="\n";
	_output_infos->lines_clear();	
	_output_infos->lineSplit_add(str);	
	_output_infos->xy(5,140);	
	_output_infos->rect_clear(0);	
	_output_infos->lines_create(5,140);	
}	
#endif

