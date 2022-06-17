#ifndef DEVICE_H
  #define DEVICE_H

	#include "../fastled/base.h"	
	#include "output.h"	
  #include "../alml_task.h" 



  struct taskCbList {
    void (* _task) ();
    int _duration;
  } ;


/* 
  classe device

  class principal

  de cette classe nous  initialiseront le system

  attributation des variables principal

  instance statique pour de la class leds_manage
    pour l'initialisation et settting des strip

  instance statique EffectList
    pour la gestion des effect sour forme de list

  instance statique LAMP
    pour l'annimation des leds en genral

  instance statique Logger
    pour debug

  instance statique Deviceserver
    pour la connection wifi
    serveur http, udp
    gestion en ammont des requette

  INITIALISATION DES OUTPUTS

  la classe gere de manierre genral 
  le contenu (effect, color, sauvegarde, etc..) des requette prefiltrer par Deviceserver

    deterine le type de requette
      passe le resulta a la class output si besion
      gerr directemn le changement d'effet

  generalise le paramettre d'effet (rotation des palette, couleures, etc..) pour une meilleur syncronisation au niveau du device
    par la suite recevra les infos direcetemnt d'un module/server maitre
 */

  /**
   * @class     Device
   * 
   * @brief   class pour la gestion global du programmes
   * @see         https://github.com/AdriLighting/
   * 
   * @author      Adrilighting
   * @date      mar. 28 déc. 2021 16:25:30
   */
  class Device 
  {
      al_taskScheduler * _TaskScheduler;

      Program * _Program = nullptr;

      /** \brief current esp ip */
      IPAddress _ip = {192, 168, 0, 1};   

      /** \brief nombre total de strip */
      uint8_t _output_count = 0;          

      /** \brief nom unique servant de hostname */
      const char *  _name = "DEVICENAME";     

      /** \brief class Output  par strip */
      Output * _outputArray  = nullptr;        

      /** \brief chipset strip */
      leds_manage * _leds_manage  = nullptr;  

      uint8_t _effLoad = true;


      boolean isColorRequest(const String & cmd);
      boolean isEffectRequest(const String & cmd);
      boolean isEffectLoadRequest(const String & cmd);


      void parseJson_output(uint8_t p , String & nextEffect, const String & req, JsonObject val);
    public:

      al_taskScheduler * get_taskSheduler();
      TaskSimple * get_task(uint8_t);

      JsonArray OutputSavArray;
      boolean _outputsSav = true;
      unsigned long _timerOutPutsSav = 0;

      
      Device(const char * name, uint8_t oc);

      void get_outputCount(uint8_t & ret);
      Output * get_outputArray(uint8_t p);
      const char * get_name();
      
      void loop();


      void parseJson_outpitListByDn(DynamicJsonDocument & doc);
      void parseJson_output(DynamicJsonDocument & doc);
      void parseJson_device(DynamicJsonDocument & doc);
      

      void output_toJson(uint8_t pos, JsonObject & oject, boolean shortKey = false, boolean fs = true, boolean eff = false);
      void outputs_toJson(JsonObject & oject, boolean shortKey = true, boolean fs = true, boolean eff = false);
      void outputs_effect_toJson(JsonObject & oject);
      void output_effect_toJson(uint8_t pos, JsonObject & oject);

      void outputs_toNode(String & out);
      void outputs_toAppi(String & out, boolean eff, boolean prog, uint8_t effLoad = 0);
      void outputs_effect_toAppi(String & out);

      void outputs_savForce();
      void outputs_sav();
      boolean outputs_requestEffect();

      void output_toSpiff(uint8_t pos, const String & path = "/outputs/lamp.txt" );
      void armoiretofs(uint8_t pos);
      
      void preset_sav(DynamicJsonDocument & doc);
      void preset_load(DynamicJsonDocument & doc);

      void bp_briLoop();
      void bp_onoff();
      void bp_onoff(uint8_t value);

      void restart_requiered();
      void format(uint8_t);


  };
  Device * DevicePtrGet();


#endif // DEVICE_H