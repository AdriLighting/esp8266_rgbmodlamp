#ifndef EFFECTSLIST_H
#define EFFECTSLIST_H

  #include "../libextern.h"



	class Effectslist {
	  const char**  _list    = nullptr; 
	  uint8_t *     _listId  = nullptr; 
		Program * 		_Program = nullptr;


	public:
		Effectslist();
		~Effectslist(){};
		Program * get_program();

		void eff_next();
		void eff_prev();
		void eff_byId(uint8_t);
		void eff_byName(const char*);
		void toggle();
		
	};
	Effectslist * EffectslistPtrGet();



	
#endif // EFFECTSLIST_H