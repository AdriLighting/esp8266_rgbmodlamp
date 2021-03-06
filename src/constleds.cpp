#include "../include/constleds.h"
#include "libextern.h"

/*
 ordre a respecter
 fx -> EffectFx_mode::load(uint8_t strip_pos) - line 166
 al -> void EffectWorker::workerset(...) - line 296

*/

PROGMEM effeNamIDList _effeNamIDList [] = {
{ALMLPT_EFF_Pride,                          E_FX_Wave,              "NONE",         "1", "0", -1}, /* EffectFx_mode_pride                 - 0 - 1 */
{ALMLPT_EFF_PrideSyncro,                    E_FX_Wave,              "NONE",         "1", "1", -1}, /* EffectFx_mode_pride                 - 0 - 1 */
{ALMLPT_EFF_ColorWave,                      E_FX_Wave,              "PALGRAD",      "1", "0", -1}, /* EffectFx_mode_colorwaves            - 0 - 1 */
{ALMLPT_EFF_ColorWaveSyncro,                E_FX_Wave,              "PALGRAD",      "1", "1", -1}, /* EffectFx_mode_colorwaves            - 0 - 1 */
{ALMLPT_EFF_RainbowCycle,                   E_FX_RainbowCycle,      "NONE",         "1", "0", -1}, /* EffectFx_mode_rainbow_cycle         - 0 - 1 */
{ALMLPT_EFF_RainbowCycleSyncro,             E_FX_RainbowCycleSync,  "NONE",         "1", "1", -1}, /* EffectFx_mode_rainbow_cycle         - 0 - 1 */
{ALMLPT_EFF_Rainbow,                        E_FX_Rainbow,           "NONE",         "1", "0", -1}, /* EffectFx_mode_rainbow               - 1 - 1 */
{ALMLPT_EFF_ColorWipe,                      E_FX_ColorWipe,         "HUETIMER",     "1", "0", -1}, /* EffectFx_mode_color_wipe            - 0 - 1 */
{ALMLPT_EFF_ColorWipeSyncro,                E_FX_ColorWipe,         "HUETIMER",     "1", "1", -1}, /* EffectFx_mode_color_wipe            - 0 - 1 */
{ALMLPT_EFF_ColorWipeReverseinverse,        E_FX_ColorWipe,         "HUETIMER",     "1", "0", -1}, /* EffectFx_mode_color_wipe_rev_inv    - 0 - 1 */
{ALMLPT_EFF_ColorWipeReverseinverseSyncro,  E_FX_ColorWipe,         "HUETIMER",     "1", "1", -1}, /* EffectFx_mode_color_wipe_rev_inv    - 0 - 1 */
{ALMLPT_EFF_ColorWipeRandom,                E_FX_ColorWipeRnd,      "NONE",         "1", "0", -1}, /* EffectFx_mode_color_wipe_random     - 0 - 1 */
{ALMLPT_EFF_ColorWipeRandomSyncro,          E_FX_ColorWipeRnd,      "NONE",         "1", "1", -1}, /* EffectFx_mode_color_wipe_random     - 0 - 1 */
{ALMLPT_EFF_TricolorChase,                  E_FX_TricolorChase,     "HTPG",         "1", "0", -1}, /* EffectFx_mode_tricolor_chase        - 0 - 1 */
{ALMLPT_EFF_TricolorChasesyncro,            E_FX_TricolorChaseSync, "HTPG",         "1", "1", -1}, /* EffectFx_mode_tricolor_chase        - 0 - 1 */
{ALMLPT_EFF_Breath,                         E_FX_Breath,            "HTPG",         "1", "0", -1}, /* EffectFx_mode_breath                - 0 - 1 */
{ALMLPT_EFF_Colortwinkle,                   E_FX_Colortwinkle,      "NONE",         "1", "0", -1}, /* EffectFx_mode_colortwinkle          - 0 - 1 */
{ALMLPT_EFF_Fade,                           E_FX_Fade,              "HUETIMER",     "1", "0", -1}, /* EffectFx_rainbowWave                - 0 - 2 */
{ALMLPT_EFF_Scan,                           E_FX_Scan,              "PAL",          "1", "0", -1}, /* EffectFx_rainbowWave                - 0 - 2 */
{ALMLPT_EFF_Commets,                        E_FX_Commets,           "HUETIMER",     "2", "0", -1}, /* EffectFx_comets                     - 0 - 2 */
{ALMLPT_EFF_Commet,                         E_FX_Commet,            "HUETIMER",     "2", "0", -1}, /* EffectFx_Comet                      - 0 - 2 */
{ALMLPT_EFF_Ruggle,                         E_FX_Move,              "HUETIMER",     "2", "0", -1}, /* EffectFx_ruggle                     - 0 - 2 */
{ALMLPT_EFF_RainbowWave,                    E_FX_RainbowWave,       "NONE",         "2", "0", -1}, /* EffectFx_rainbowWave                - 0 - 2 */
{ALMLPT_EFF_ArmoireOutsideToInside,         E_FX_Armoire_1,         "HUETIMER",     "2", "0", -1}, /* EffectFx_rainbowWave                - 0 - 2 */
};

void effectType_toString(OUTPUT_EFFECTTYPE type, String & result){
  switch (type) {
      case OUTPUT_EFFECTTYPE::OET_NONE:     result = "OET_NONE"; break;
      case OUTPUT_EFFECTTYPE::OET_PALGRAD:  result = "OET_PALGRAD"; break;
      case OUTPUT_EFFECTTYPE::OET_PAL:      result = "OET_PAL"; break;
      case OUTPUT_EFFECTTYPE::OET_HTPG:     result = "OET_HTPG"; break;
      case OUTPUT_EFFECTTYPE::OET_HUETIMER: result = "OET_HUETIMER"; break;
      default: result = ""; break;
  }
}
OUTPUT_EFFECTTYPE effectType_toMod(const char * name){
  if (al_tools::ch_toString(name) == "NONE")      return  OET_NONE;
  if (al_tools::ch_toString(name) == "PALGRAD")   return  OET_PALGRAD;
  if (al_tools::ch_toString(name) == "PAL")       return  OET_PAL;
  if (al_tools::ch_toString(name) == "HTPG")      return  OET_HTPG;
  if (al_tools::ch_toString(name) == "HUETIMER")  return  OET_HUETIMER;
  return  OET_NONE;
}
uint8_t _effeNamIDList_cnt = ARRAY_SIZE(_effeNamIDList);