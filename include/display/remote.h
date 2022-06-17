#ifndef REMOTE_H
#define REMOTE_H

  #include "../libextern.h"
  #include "effectslist.h"
	#include "../alml_webserver.h"

	class RemoteControl_ir  {
		IRrecv* irrecv;
		//change pin in NpbWrapper.h

		decode_results results;

		unsigned long	irCheckedTime 				= 0;
		uint32_t 			lastValidCode					= 0;
		byte 					lastRepeatableAction 	= ACTION_NONE;
		int 					lastRepeatableValue 	= 0;
		uint16_t 			irTimesRepeated 			= 0;
		uint8_t 			lastIR6ColourIdx 			= 0;
		int8_t 				irPin 								= 4;
		uint8_t 			irEnabled 						= 1;


		boolean decode(uint32_t);
		void decode44(uint32_t);
		void decode24(uint32_t);
		void changeBrightness(int8_t);
		void changeEffSpeed(int8_t);
		void applyRepeatActions();

	public:
		RemoteControl_ir(){_EffectslistPtr = EffectslistPtrGet();};
		~RemoteControl_ir(){};

		Effectslist * _EffectslistPtr;

	protected:
		virtual void begin();
		virtual boolean handle();

	};
	class RemoteControl_udp : public UdpMulti, public Udp {
	  boolean multiSend_jsonDevice = true;
	  unsigned long timerMultiSend_jsonDevice;  		
	public:
		RemoteControl_udp(){};
		~RemoteControl_udp(){};
		virtual void send_appi(boolean eff, boolean prog, boolean reset = false, uint8_t effLoad = false);

	protected:
		virtual void handle();
		virtual void begin();
	private:
		void handleJson(uint8_t op);
		void handleJson(DynamicJsonDocument & doc, boolean udpMulti = true);
	};
	#ifdef ADS_PIN_IR
	class RemoteControl : public  Effectslist, public RemoteControl_ir, public RemoteControl_udp {	
	#else
	class RemoteControl : public  Effectslist, public RemoteControl_udp {
	#endif
	public:
		RemoteControl();
		~RemoteControl(){};
		#ifdef ADS_PIN_IR
		void handle_ir();	
		#endif
		void handle_udp();
		void begin();
	};
	RemoteControl * RemoteControlPtrGet();	

#endif // REMOTE_H