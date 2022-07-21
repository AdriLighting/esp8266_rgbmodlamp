#ifndef CONSTLEDS_H
#define CONSTLEDS_H

#include <Arduino.h>
#include <FastLED.h>
#include "defleds.h"

// static const uint8_t T_EFFVER[] PROGMEM = {
//   3, 4, 6, 4, 4, 5, 3, 3, 7, 5, 5, 6, 5, 9, 5, 8, // 0-15
//   5, 5, 3, 7, 3, 7, 5, 5, 7, 3, 3, 7, 7, 5, 1, 5, // 16-31
//   5, 5, 5, 3, 7, 3, 4, 3, 3, 9, 4, 5, 7, 7, 3, 3, // 32 - 47
//   5, 6, 6, 6, 4, 7, 5, 4, 5, 5, 6, 9, 5, 5, 7, 6, // 48 - 63
//   5, 1, 1, 5, 4, 1, 7, 5, 3, 1, 1, 1, 0, 0, 0, 0, // 64 - 79
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 80 - 95
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 96 - 111
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 112 - 127
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 128 - 143
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 144 - 159
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 160 - 175
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 176 - 191
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 192 - 207
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 208 - 223
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 224 - 239
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 4, 6, // 240 - 255
// };

// #define COTNROLS_PREFIX "\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":"
// static const char E_DEFUI[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[]}";

#define COTNROLS_PREFIX "\"nb\":@nb@,\"name\":\"@name@\""
static const char E_DEFUI[]     PROGMEM = "{" COTNROLS_PREFIX ",\"ctrls\":[]}";




static const char ALMLPT_LEDS_001[] PROGMEM = DFTINTF_084;//Palette
static const char ALMLPT_LEDS_002[] PROGMEM = DFTINTF_020;//Microphone
static const char ALMLPT_LEDS_003[] PROGMEM = "Refresh effects-list";
static const char ALMLPT_LEDS_004[] PROGMEM = "Save";
static const char ALMLPT_LEDS_005[] PROGMEM = "Luminosité";
static const char ALMLPT_LEDS_006[] PROGMEM = "Vitesse";
static const char ALMLPT_LEDS_007[] PROGMEM = DFTINTF_088; //Echelle
static const char ALMLPT_LEDS_008[] PROGMEM = "Hue"; //
static const char ALMLPT_LEDS_009[] PROGMEM = "Couleur statique"; //


// static const char TCONST_0012[] PROGMEM = "bright";
// static const char TCONST_0013[] PROGMEM = "Vitess";
// static const char TCONST_0014[] PROGMEM = "scale";

// static const char PGidxtemplate[] PROGMEM  = "%s{\"n\":%d,\"f\":%d}";

// static const char TEFF_000[] PROGMEM = "";  // "пустышка"

static const char ALMLPT_EFF_Pride                            [] PROGMEM = "Pride";
static const char ALMLPT_EFF_PrideSyncro                      [] PROGMEM = "Pride Syncro";
static const char ALMLPT_EFF_ColorWave                        [] PROGMEM = "ColorWave";
static const char ALMLPT_EFF_ColorWaveSyncro                  [] PROGMEM = "ColorWave Syncro";
static const char ALMLPT_EFF_RainbowCycle                     [] PROGMEM = "Rainbow cycle";
static const char ALMLPT_EFF_RainbowCycleSyncro               [] PROGMEM = "Rainbow cycle Syncro";
static const char ALMLPT_EFF_Rainbow                          [] PROGMEM = "Rainbow";
static const char ALMLPT_EFF_RainbowSyncro                    [] PROGMEM = "Rainbow Syncro";
static const char ALMLPT_EFF_ColorWipe                        [] PROGMEM = "ColorWipe";
static const char ALMLPT_EFF_ColorWipeSyncro                  [] PROGMEM = "ColorWipe Syncro";
static const char ALMLPT_EFF_ColorWipeInverse                 [] PROGMEM = "ColorWipe inverse";
static const char ALMLPT_EFF_ColorWipeInverseSyncro           [] PROGMEM = "ColorWipe inverse Syncro";
static const char ALMLPT_EFF_ColorWipeReverseinverse          [] PROGMEM = "ColorWipe reverse inverse";
static const char ALMLPT_EFF_ColorWipeReverseinverseSyncro    [] PROGMEM = "ColorWipe reverse inverse Syncro";
static const char ALMLPT_EFF_ColorWipeRandom                  [] PROGMEM = "ColorWipe random";
static const char ALMLPT_EFF_ColorWipeRandomSyncro            [] PROGMEM = "ColorWipe random Syncro";
static const char ALMLPT_EFF_TricolorChase                    [] PROGMEM = "Tricolor Chase";
static const char ALMLPT_EFF_TricolorChasesyncro              [] PROGMEM = "Tricolor Chase Syncro";
static const char ALMLPT_EFF_Breath                           [] PROGMEM = "Breath";
static const char ALMLPT_EFF_BreathSyncro                     [] PROGMEM = "Breath Syncro";
static const char ALMLPT_EFF_Colortwinkle                     [] PROGMEM = "Colortwinkle";
static const char ALMLPT_EFF_Commets                          [] PROGMEM = "Commets";
static const char ALMLPT_EFF_Commet                           [] PROGMEM = "Commet";
static const char ALMLPT_EFF_Bounce                           [] PROGMEM = "Bounce";
static const char ALMLPT_EFF_Ruggle                           [] PROGMEM = "Ruggle";
static const char ALMLPT_EFF_RainbowWave                      [] PROGMEM = "Rainbow Wave";
static const char ALMLPT_EFF_Fade                             [] PROGMEM = "fade";
static const char ALMLPT_EFF_Scan                             [] PROGMEM = "scan";


static const char ALMLPT_EFF_ChaseBlackoutrainbow             [] PROGMEM = "Chase blackout rainbow";
static const char ALMLPT_EFF_TheaterChaserainbow              [] PROGMEM = "Theater chase rainbow";
static const char ALMLPT_EFF_ChaseBlackout                    [] PROGMEM = "Chase blackout";
static const char ALMLPT_EFF_ColorSweeprandom                 [] PROGMEM = "Color sweep random";
static const char ALMLPT_EFF_LarsonScanner                    [] PROGMEM = "Larson scanner";
static const char ALMLPT_EFF_Glow                             [] PROGMEM = "Glow";
static const char ALMLPT_EFF_Pulse                            [] PROGMEM = "Pulse";
static const char ALMLPT_EFF_SineWave                         [] PROGMEM = "SineWave";
static const char ALMLPT_EFF_TriWave                          [] PROGMEM = "TriWave";
static const char ALMLPT_EFF_Wave                             [] PROGMEM = "Wave";
static const char ALMLPT_EFF_ColorPalette                     [] PROGMEM = "ColorPalette";
static const char ALMLPT_EFF_ColorPaletteSyncro               [] PROGMEM = "ColorPalette syncro";
static const char ALMLPT_EFF_Twinklefox                       [] PROGMEM = "Twinklefox";
static const char ALMLPT_EFF_ArmoireStatiquehue               [] PROGMEM = "Armoire statique hue";
static const char ALMLPT_EFF_ArmoireStatiquernd               [] PROGMEM = "Armoire statique rnd";
static const char ALMLPT_EFF_ArmoireCyclehue                  [] PROGMEM = "Armoire cycle hue";
static const char ALMLPT_EFF_ArmoireOutsideToInside           [] PROGMEM = "ArmoireOutsideToInside";

/*
0   bri
1   speed
2   scale
3   fade
4   Cyle timer 1
5   variation
6   Taille
7   collor mod      rangew0
8   collor 1        rangew0
9   collor 2        rangew0
10  rep

*/
static const char E_FX_ColorWipe []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":105,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":4,\"type\":0,\"val\":40,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  " {\"id\":5,\"type\":0,\"val\":60,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_123 "\"},"
  // " {\"id\":7,\"type\":3,\"val\":1,\"min\":0,\"max\":2,\"name\":\"" DFTINTF_119 "\"},"
  " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"},"
  " {\"id\":9,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_113 "\"}"
  "]}";

static const char E_FX_Fade []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":105,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":4,\"type\":0,\"val\":40,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  // " {\"id\":7,\"type\":3,\"val\":1,\"min\":0,\"max\":2,\"name\":\"" DFTINTF_119 "\"},"
  " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"}"
  "]}";

// static const char E_FX_Scan []  PROGMEM = 
//   "{" COTNROLS_PREFIX ",\"ctrls\":["
//   " {\"id\":1,\"val\":105,\"min\":0,\"max\":255},"
//   " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
//   " {\"id\":4,\"type\":0,\"val\":40,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
//   " {\"id\":5,\"type\":0,\"val\":60,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_123 "\"},"
//   // " {\"id\":7,\"type\":3,\"val\":1,\"min\":0,\"max\":2,\"name\":\"" DFTINTF_119 "\"},"
//   " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"},"
//   " {\"id\":9,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_113 "\"}"
//   "]}";
static const char E_FX_Scan []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":105,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":255}"
  "]}";

static const char E_FX_ColorWipeRnd []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":105,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3}"
  "]}";

static const char E_FX_Breath []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":100,\"min\":0,\"max\":255,\"state\":3},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":4,\"type\":0,\"val\":255,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  " {\"id\":5,\"type\":0,\"val\":60,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_123 "\"},"
  // " {\"id\":7,\"type\":3,\"val\":1,\"min\":0,\"max\":2,\"name\":\"" DFTINTF_119 "\"},"
  " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"},"
  " {\"id\":9,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_113 "\"}"
  "]}";

static const char E_FX_Move []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":127,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":5,\"type\":0,\"val\":60,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_123 "\"},"
  " {\"id\":4,\"type\":0,\"val\":40,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  // " {\"id\":7,\"type\":3,\"val\":1,\"min\":0,\"max\":2,\"name\":\"" DFTINTF_119 "\"},"
  " {\"id\":8,\"type\":3,\"val\":80,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"}"
  "]}";

static const char E_FX_Commet []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":127,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":1},"
  " {\"id\":4,\"type\":0,\"val\":255,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"}"
  "]}";

static const char E_FX_Commets []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":127,\"min\":0,\"max\":255},"
  " {\"id\":2,\"state\":3,\"val\":0,\"min\":0,\"max\":0},"
  " {\"id\":6,\"type\":0,\"val\":127,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_129 "\"},"
  " {\"id\":4,\"type\":0,\"val\":255,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"}"
  "]}";

static const char E_FX_RainbowCycle []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":60,\"min\":0,\"max\":255},"
  " {\"id\":2,\"type\":0,\"val\":255,\"min\":0,\"max\":255},"
  " {\"id\":3,\"type\":0,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_0ED "\"}"
  "]}";
static const char E_FX_RainbowCycleSync []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":60,\"min\":0,\"max\":255},"
  " {\"id\":2,\"type\":0,\"val\":255,\"min\":0,\"max\":255},"
  " {\"id\":3,\"type\":0,\"val\":30,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_0ED "\"}"
  "]}";

static const char E_FX_Wave []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":0,\"val\":127,\"min\":1,\"max\":255},"
  " {\"id\":1,\"val\":100,\"min\":1,\"max\":255},"
  " {\"id\":2,\"state\":3,\"val\":0,\"min\":0,\"max\":0}"
  "]}";

static const char E_FX_Rainbow []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":1,\"type\":3,\"val\":16,\"min\":0,\"max\":255}"
  "]}"; 

static const char E_FX_RainbowWave []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":2,\"val\":69,\"min\":0,\"max\":255},"
  " {\"id\":1,\"val\":40,\"min\":0,\"max\":255}"
  "]}";

static const char E_FX_Colortwinkle []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":42,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":3,\"type\":0,\"val\":255,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_0ED "\"}"
  "]}";

static const char E_FX_TricolorChaseSync []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":80,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":6,\"type\":0,\"val\":3,\"min\":0,\"max\":5,\"name\":\"" DFTINTF_126 "\"},"
  " {\"id\":5,\"type\":0,\"val\":30,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_123 "\"},"
  " {\"id\":4,\"type\":0,\"val\":255,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  // " {\"id\":7,\"type\":3,\"val\":1,\"min\":0,\"max\":2,\"name\":\"" DFTINTF_119 "\"},"
  " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"},"
  " {\"id\":9,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_113 "\"}"  
  "]}";
static const char E_FX_TricolorChase []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":60,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":6,\"type\":0,\"val\":2,\"min\":0,\"max\":5,\"name\":\"" DFTINTF_126 "\"},"
  " {\"id\":5,\"type\":0,\"val\":30,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_123 "\"},"
  " {\"id\":4,\"type\":0,\"val\":250,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  // " {\"id\":7,\"type\":3,\"val\":1,\"min\":0,\"max\":2,\"name\":\"" DFTINTF_119 "\"},"
  " {\"id\":8,\"type\":3,\"val\":80,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"},"
  " {\"id\":9,\"type\":3,\"val\":130,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_113 "\"}"  
  "]}";

static const char E_FX_Armoire_1 []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":5,\"type\":0,\"val\":30,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_123 "\"},"
  " {\"id\":4,\"type\":0,\"val\":255,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"}"
  "]}";

// static const char* const T_EFFUICFG[] PROGMEM = {
// /*  0   - ALMLPT_EFF_Pride                          - EffectFx_mode_pride                 - OET_NONE      - 0 - 1 */ E_DEFUI,
// /*  1   - ALMLPT_EFF_PrideSyncro                    - EffectFx_mode_pride                 - OET_NONE      - 0 - 1 */ E_DEFUI,
// /*  10  - ALMLPT_EFF_ColorWave                      - EffectFx_mode_colorwaves            - OET_PALGRAD   - 0 - 1 */ E_DEFUI,
// /*  11  - ALMLPT_EFF_ColorWaveSyncro                - EffectFx_mode_colorwaves            - OET_PALGRAD   - 0 - 1 */ E_DEFUI,          
// /*  12  - ALMLPT_EFF_RainbowCycle                   - EffectFx_mode_rainbow_cycle         - OET_NONE      - 0 - 1 */ E_FX_RainbowCycle,
// /*  13  - ALMLPT_EFF_RainbowCycleSyncro             - EffectFx_mode_rainbow_cycle         - OET_NONE      - 0 - 1 */ E_FX_RainbowCycle,
// /*  14  - ALMLPT_EFF_Rainbow                        - EffectFx_mode_rainbow               - OET_NONE      - 1 - 1 */ E_FX_Rainbow,
// /*  15  - ALMLPT_EFF_RainbowSyncro                  - EffectFx_mode_rainbow               - OET_NONE      - 1 - 1 */ E_FX_Rainbow,
// /*  2   - ALMLPT_EFF_ColorWipe                      - EffectFx_mode_color_wipe            - OET_HUETIMER  - 0 - 1 */ E_FX_ColorWipe,
// /*  3   - ALMLPT_EFF_ColorWipeSyncro                - EffectFx_mode_color_wipe            - OET_HUETIMER  - 0 - 1 */ E_FX_ColorWipe,
// /*  4   - ALMLPT_EFF_ColorWipeInverse               - EffectFx_mode_color_wipe_inv        - OET_HUETIMER  - 0 - 1 */ E_FX_ColorWipe,        
// /*  5   - ALMLPT_EFF_ColorWipeInverseSyncro         - EffectFx_mode_color_wipe_inv        - OET_HUETIMER  - 0 - 1 */ E_FX_ColorWipe,        
// /*  6   - ALMLPT_EFF_ColorWipeReverseinverse        - EffectFx_mode_color_wipe_rev_inv    - OET_HUETIMER  - 0 - 1 */ E_FX_ColorWipe,              
// /*  7   - ALMLPT_EFF_ColorWipeReverseinverseSyncro  - EffectFx_mode_color_wipe_rev_inv    - OET_HUETIMER  - 0 - 1 */ E_FX_ColorWipe,              
// /*  8   - ALMLPT_EFF_ColorWipeRandom                - EffectFx_mode_color_wipe_random     - OET_NONE      - 0 - 1 */ E_FX_ColorWipe,    
// /*  9   - ALMLPT_EFF_ColorWipeRandomSyncro          - EffectFx_mode_color_wipe_random     - OET_NONE      - 0 - 1 */ E_FX_ColorWipe, 
// /*  19  - ALMLPT_EFF_TricolorChase                  - EffectFx_mode_tricolor_chase        - OET_HTPG      - 1 - 1 */ E_FX_TricolorChase,
// /*  20  - ALMLPT_EFF_TricolorChasesyncro            - EffectFx_mode_tricolor_chase        - OET_HTPG      - 1 - 1 */ E_FX_TricolorChase,
// /*  17  - ALMLPT_EFF_Breath                         - EffectFx_mode_breath                - OET_HUETIMER  - 0 - 1 */ E_FX_Breath,
// /*  18  - ALMLPT_EFF_BreathSyncro                   - EffectFx_mode_breath                - OET_HUETIMER  - 0 - 1 */ E_FX_Breath,
// /*  16  - ALMLPT_EFF_Colortwinkle                   - EffectFx_mode_colortwinkle          - OET_NONE      - 0 - 1 */ E_FX_Colortwinkle,  
// /*      - ALMLPT_EFF_Commets                        - EffectFx_comets                     - OET_HUETIMER  - 0 - 2 */ E_FX_Move,        
// /*      - ALMLPT_EFF_Commet                         - EffectFx_Comet                      - OET_HUETIMER  - 0 - 2 */ E_FX_Move,  
// /*      - ALMLPT_EFF_Bounce                         - EffectFx_bounce                     - OET_HUETIMER  - 0 - 2 */ E_FX_Move,    
// /*      - ALMLPT_EFF_Ruggle                         - EffectFx_ruggle                     - OET_HUETIMER  - 0 - 2 */ E_FX_Move,  
// /*      - ALMLPT_EFF_RainbowWave                    - EffectFx_rainbowWave                - OET_HUETIMER  - 0 - 2 */ E_DEFUI
// };
// typedef enum : uint8_t {
//   EFFID_CAT_NONE=0,  
//   EFFID_CAT_NEO,    
//   EFFID_CAT_COMM  
// } effeNamIDCat_t; 

struct effeNamIDList {
  const char * _nameId;
  const char * _defaultCfg;
  const char * _effType;
  const char *  _cat;
  const char * _segment;
  int _spd;
} ;
// static const char* const T_EFFNAMEID[] PROGMEM = {
// ALMLPT_EFF_Pride,
// ALMLPT_EFF_PrideSyncro,
// ALMLPT_EFF_ColorWave,
// ALMLPT_EFF_ColorWaveSyncro,
// ALMLPT_EFF_RainbowCycle,
// ALMLPT_EFF_RainbowCycleSyncro,
// ALMLPT_EFF_Rainbow,
// ALMLPT_EFF_RainbowSyncro,
// ALMLPT_EFF_ColorWipe,
// ALMLPT_EFF_ColorWipeSyncro,
// ALMLPT_EFF_ColorWipeInverse,
// ALMLPT_EFF_ColorWipeInverseSyncro,
// ALMLPT_EFF_ColorWipeReverseinverse,
// ALMLPT_EFF_ColorWipeReverseinverseSyncro,
// ALMLPT_EFF_ColorWipeRandom,
// ALMLPT_EFF_ColorWipeRandomSyncro,
// ALMLPT_EFF_TricolorChase,
// ALMLPT_EFF_TricolorChasesyncro,
// ALMLPT_EFF_Breath,
// ALMLPT_EFF_BreathSyncro,
// ALMLPT_EFF_Colortwinkle,
// ALMLPT_EFF_Commets,
// ALMLPT_EFF_Commet,
// ALMLPT_EFF_Bounce,
// ALMLPT_EFF_Ruggle,
// ALMLPT_EFF_RainbowWave
// };
extern PROGMEM effeNamIDList _effeNamIDList [] ;
extern uint8_t _effeNamIDList_cnt ;
void effectType_toString(OUTPUT_EFFECTTYPE type, String & result);
OUTPUT_EFFECTTYPE effectType_toMod(const char * name);

static const TProgmemRGBPalette16 HeatColors2_p FL_PROGMEM = {    0x000000, 0x330000, 0x660000, 0x990000, 0xCC0000, 0xFF0000, 0xFF3300, 0xFF6600, 0xFF9900, 0xFFCC00, 0xFFFF00, 0xFFFF33, 0xFFFF66, 0xFFFF99, 0xFFFFCC, 0xFFFFFF};
static const TProgmemRGBPalette16 WoodFireColors_p FL_PROGMEM = {CRGB::Black, 0x330e00, 0x661c00, 0x992900, 0xcc3700, CRGB::OrangeRed, 0xff5800, 0xff6b00, 0xff7f00, 0xff9200, CRGB::Orange, 0xffaf00, 0xffb900, 0xffc300, 0xffcd00, CRGB::Gold};             //* Orange
static const TProgmemRGBPalette16 NormalFire_p FL_PROGMEM = {CRGB::Black, 0x330000, 0x660000, 0x990000, 0xcc0000, CRGB::Red, 0xff0c00, 0xff1800, 0xff2400, 0xff3000, 0xff3c00, 0xff4800, 0xff5400, 0xff6000, 0xff6c00, 0xff7800};                             // пытаюсь сделать что-то более приличное
static const TProgmemRGBPalette16 NormalFire2_p FL_PROGMEM = {CRGB::Black, 0x560000, 0x6b0000, 0x820000, 0x9a0011, CRGB::FireBrick, 0xc22520, 0xd12a1c, 0xe12f17, 0xf0350f, 0xff3c00, 0xff6400, 0xff8300, 0xffa000, 0xffba00, 0xffd400};                      // пытаюсь сделать что-то более приличное
static const TProgmemRGBPalette16 LithiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x240707, 0x470e0e, 0x6b1414, 0x8e1b1b, CRGB::FireBrick, 0xc14244, 0xd16166, 0xe08187, 0xf0a0a9, CRGB::Pink, 0xff9ec0, 0xff7bb5, 0xff59a9, 0xff369e, CRGB::DeepPink};        //* Red
static const TProgmemRGBPalette16 SodiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x332100, 0x664200, 0x996300, 0xcc8400, CRGB::Orange, 0xffaf00, 0xffb900, 0xffc300, 0xffcd00, CRGB::Gold, 0xf8cd06, 0xf0c30d, 0xe9b913, 0xe1af1a, CRGB::Goldenrod};           //* Yellow
static const TProgmemRGBPalette16 CopperFireColors_p FL_PROGMEM = {CRGB::Black, 0x001a00, 0x003300, 0x004d00, 0x006600, CRGB::Green, 0x239909, 0x45b313, 0x68cc1c, 0x8ae626, CRGB::GreenYellow, 0x94f530, 0x7ceb30, 0x63e131, 0x4bd731, CRGB::LimeGreen};     //* Green
static const TProgmemRGBPalette16 AlcoholFireColors_p FL_PROGMEM = {CRGB::Black, 0x000033, 0x000066, 0x000099, 0x0000cc, CRGB::Blue, 0x0026ff, 0x004cff, 0x0073ff, 0x0099ff, CRGB::DeepSkyBlue, 0x1bc2fe, 0x36c5fd, 0x51c8fc, 0x6ccbfb, CRGB::LightSkyBlue};  //* Blue
static const TProgmemRGBPalette16 RubidiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x0f001a, 0x1e0034, 0x2d004e, 0x3c0068, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, 0x3c0084, 0x2d0086, 0x1e0087, 0x0f0089, CRGB::DarkBlue};        //* Indigo
static const TProgmemRGBPalette16 PotassiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x0f001a, 0x1e0034, 0x2d004e, 0x3c0068, CRGB::Indigo, 0x591694, 0x682da6, 0x7643b7, 0x855ac9, CRGB::MediumPurple, 0xa95ecd, 0xbe4bbe, 0xd439b0, 0xe926a1, CRGB::DeepPink}; //* Violet
static const TProgmemRGBPalette16 WaterfallColors_p FL_PROGMEM = {0x000000, 0x060707, 0x101110, 0x151717, 0x1C1D22, 0x242A28, 0x363B3A, 0x313634, 0x505552, 0x6B6C70, 0x98A4A1, 0xC1C2C1, 0xCACECF, 0xCDDEDD, 0xDEDFE0, 0xB2BAB9};        //* Orange
static const TProgmemRGBPalette16 ZeebraColors_p FL_PROGMEM = {CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black};
// Добавил "белую" палитру для "Огонь 2012", в самом конце 4-го ползунка, огонь горит белым цветом, - красиво для белой динамической подсветки
static const TProgmemRGBPalette16 WhiteBlackColors_p FL_PROGMEM = {CRGB::Black, CRGB::Gray, CRGB::White, CRGB::WhiteSmoke};
// stepko нашел и перерисовал палитры в https://colorswall.com/
static const TProgmemRGBPalette16 AcidColors_p FL_PROGMEM = {0xffff00, 0xd2b328, 0xd25228, 0x711323, 0x6c000c, 0x5a0c00, 0x6d373a, 0xaa5a62, 0x604564, 0x313164, 0x332765, 0x3a2465, 0x4b1665, 0x4b0069, 0x31004c, 0x200046};
static const TProgmemRGBPalette16 StepkosColors_p FL_PROGMEM = {0x0000ff, 0x0f00f0, 0x1e00e1, 0x2d00d2, 0x3c00c3, 0x4b00b4, 0x5a00a5, 0x690096, 0x780087, 0x870078, 0x9600cd, 0xa50050, 0xb40041, 0xc30032, 0xd20023, 0xe10014};
static const TProgmemRGBPalette16 AutumnColors_p FL_PROGMEM = {0xbc2300, 0xc84416, 0xdc642c, 0xe69664, 0xfbb979, 0xca503d, 0x882c1c, 0x9a3631, 0xa9624e, 0xcc9762, 0xdcc0b5, 0xc1a29f, 0x826468, 0x4a3334, 0x231a1a, 0x161113};
static const TProgmemRGBPalette16 NeonColors_p FL_PROGMEM = {0x00b1d0, 0x0f93ec, 0x3572ff, 0x4157ff, 0x6162ff, 0x686cff, 0x7473ff, 0x8689e5, 0x9e9dc6, 0x9694ac, 0x979b9b, 0x888b8c, 0x767680, 0x596160, 0x6c736f, 0x7b7359};
static const TProgmemRGBPalette16 EveningColors_p FL_PROGMEM = {0x1e0443, 0x6d0081, 0x8200ac, 0x8200ac, 0x8200ac, 0x8200ac, 0x8200ac, 0x8200ac, 0x7900a1, 0x820055, 0xc80000, 0xe57b00, 0xff9d5a, 0xc58b32, 0xd8d400, 0xffff00};
static const TProgmemRGBPalette16 OrangeColors_p FL_PROGMEM = {0xffff00, 0xfff100, 0xffe100, 0xffd100, 0xffc100, 0xffb100, 0xffa100, 0xff9100, 0xff8100, 0xff7100, 0xff6100, 0xff5100, 0xff4100, 0xff3100, 0xff2100, 0xff1100};

// Удачная палитра от Stepko заменю ка я ею RainbowStripeColors_p, которая "рябит" на большинстве эффектов
static const TProgmemRGBPalette16 AuroraColors_p FL_PROGMEM = {0x00ff00, 0x00c040, 0x008080, 0x0040c0, 0x0000ff, 0x4000c0, 0x800080, 0xc00040, 0xff0000, 0xff4000, 0xff8000, 0xd6c000, 0xffff00, 0xc0ff00, 0x80ff00, 0x40ff00};
static const TProgmemRGBPalette16 HolyLightsColors_p FL_PROGMEM = {0xff0000, 0xff4000, 0xff8000, 0xd6c000, 0xffff00, 0xc0ff00, 0x80ff00, 0x40ff00, 0x00ff00, 0x00c040, 0x008080, 0x0040c0, 0x0000ff, 0x4000c0, 0x800080, 0xc00040};


// static const TProgmemRGBPalette16 alcohol_p FL_PROGMEM = {0x000000, 0x000033, 0x000066, 0x000099, 0x0000CC, 0x0000FF, 0x0026FF, 0x004CFF, 0x0072FF, 0x0098FF, 0x00BFFF, 0x1BC2FE, 0x36C5FD, 0x51C8FC, 0x6CCBFB, 0x87CEFA};
// static const TProgmemRGBPalette16 copper_p FL_PROGMEM = {0x000000, 0x001900, 0x003300, 0x004C00, 0x006600, 0x008000, 0x229909, 0x45B212, 0x67CC1C, 0x8AE525, 0xADFF2F, 0x94F52F, 0x7BEB30, 0x63E130, 0x4AD731, 0x32CD31};
// static const TProgmemRGBPalette16 lithium_p FL_PROGMEM = {0x000000, 0x230606, 0x470D0D, 0x6A1414, 0x8E1B1B, 0xB22222, 0xC14143, 0xD06165, 0xE08087, 0xEFA0A9, 0xFFC0CB, 0xFF9DBF, 0xFF7BB4, 0xFF58A9, 0xFF369E, 0xFF1493};
// static const TProgmemRGBPalette16 potassium_p FL_PROGMEM = {0x000000, 0x0F001A, 0x1E0034, 0x2D004E, 0x3C0068, 0x4B0082, 0x591693, 0x672CA5, 0x7643B7, 0x8459C9, 0x9370DB, 0xA85DCC, 0xBE4BBE, 0xD338AF, 0xE926A1, 0xFE1493};
// static const TProgmemRGBPalette16 rubidium_p FL_PROGMEM = {0x000000, 0x0F001A, 0x1E0034, 0x2D004E, 0x3C0068, 0x4B0082, 0x4B0082, 0x4B0082, 0x4B0082, 0x4B0082, 0x4B0082, 0x3C0083, 0x2D0085, 0x1E0087, 0x0F0089, 0x00008A};
// static const TProgmemRGBPalette16 sodium_p FL_PROGMEM = {0x000000, 0x332100, 0x664200, 0x996300, 0xCC8400, 0xFFA500, 0xFFAF00, 0xFFB900, 0xFFC300, 0xFFCD00, 0xFFD700, 0xF7CD06, 0xF0C30C, 0xE8B913, 0xE1AF19, 0xDAA51F};
// static const TProgmemRGBPalette16 wood_p FL_PROGMEM = {0x000000, 0x330D00, 0x661B00, 0x992900, 0xCC3700, 0xFF4500, 0xFF5800, 0xFF6B00, 0xFF7E00, 0xFF9100, 0xFFA500, 0xFFAF00, 0xFFB900, 0xFFC300, 0xFFCD00, 0xFFD700};
// static const TProgmemRGBPalette16 bluewhite_p_p FL_PROGMEM = {0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x808080, 0x808080, 0x808080};
// static const TProgmemRGBPalette16 cloudcolors_p_p FL_PROGMEM = {0x0000FF, 0x00008B, 0x00008B, 0x00008B, 0x00008B, 0x00008B, 0x00008B, 0x00008B, 0x0000FF, 0x00008B, 0x87CEEB, 0x87CEEB, 0xADD8E6, 0xFFFFFF, 0xADD8E6, 0x87CEEB};

// static const TProgmemRGBPalette16 fairylight_p_p FL_PROGMEM = {0xFFE42D, 0xFFE42D, 0xFFE42D, 0xFFE42D, 0x7F7216, 0x7F7216, 0xFFE42D, 0xFFE42D, 0x3F390B, 0x3F390B, 0xFFE42D, 0xFFE42D, 0xFFE42D, 0xFFE42D, 0xFFE42D, 0xFFE42D};
static const TProgmemRGBPalette16 forestcolors_p_p FL_PROGMEM = {0x006400, 0x006400, 0x556B2F, 0x006400, 0x008000, 0x228B22, 0x6B8E23, 0x008000, 0x2E8B57, 0x66CDAA, 0x32CD32, 0x9ACD32, 0x90EE90, 0x7CFC00, 0x66CDAA, 0x228B22};
// static const TProgmemRGBPalette16 heatcolors_p_p FL_PROGMEM = {0x000000, 0x330000, 0x660000, 0x990000, 0xCC0000, 0xFF0000, 0xFF3300, 0xFF6600, 0xFF9900, 0xFFCC00, 0xFFFF00, 0xFFFF33, 0xFFFF66, 0xFFFF99, 0xFFFFCC, 0xFFFFFF};
// static const TProgmemRGBPalette16 holly_p_p FL_PROGMEM = {0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0xB00402};
// static const TProgmemRGBPalette16 icecolors_p_p FL_PROGMEM = {0x000000, 0x000033, 0x000066, 0x000099, 0x0000CC, 0x0000FF, 0x0033FF, 0x0066FF, 0x0099FF, 0x00CCFF, 0x00FFFF, 0x33FFFF, 0x66FFFF, 0x99FFFF, 0xCCFFFF, 0xFFFFFF};
// static const TProgmemRGBPalette16 icyblue_p_p FL_PROGMEM = {0x0C1040, 0x0C1040, 0x0C1040, 0x0C1040, 0x0C1040, 0x0C1040, 0x0C1040, 0x0C1040, 0x0C1040, 0x0C1040, 0x0C1040, 0x0C1040, 0x182080, 0x182080, 0x182080, 0x5080C0};
// static const TProgmemRGBPalette16 lavacolors_p_p FL_PROGMEM = {0x000000, 0x800000, 0x000000, 0x800000, 0x8B0000, 0x800000, 0x8B0000, 0x8B0000, 0x8B0000, 0xFF0000, 0xFFA500, 0xFFFFFF, 0xFFA500, 0xFF0000, 0x8B0000, 0x000000};
// static const TProgmemRGBPalette16 oceancolors_p_p FL_PROGMEM = {0x191970, 0x00008B, 0x191970, 0x000080, 0x00008B, 0x0000CD, 0x2E8B57, 0x008080, 0x5F9EA0, 0x0000FF, 0x008B8B, 0x6495ED, 0x7FFFD4, 0x2E8B57, 0x00FFFF, 0x87CEFA};
// static const TProgmemRGBPalette16 partycolors_p_p FL_PROGMEM = {0x5500AB, 0x84007C, 0xB5004B, 0xE5001B, 0xE81700, 0xB84700, 0xAB7700, 0xABAB00, 0xAB5500, 0xDD2200, 0xF2000E, 0xC2003E, 0x8F0071, 0x5F00A1, 0x2F00D0, 0x0007F9};
// static const TProgmemRGBPalette16 rainbowcolors_p_p FL_PROGMEM = {0xFF0000, 0xD52A00, 0xAB5500, 0xAB7F00, 0xABAB00, 0x56D500, 0x00FF00, 0x00D52A, 0x00AB55, 0x0056AA, 0x0000FF, 0x2A00D5, 0x5500AB, 0x7F0081, 0xAB0055, 0xD5002B};
// static const TProgmemRGBPalette16 rainbowstripecolors_p_p FL_PROGMEM = {0xFF0000, 0x000000, 0xAB5500, 0x000000, 0xABAB00, 0x000000, 0x00FF00, 0x000000, 0x00AB55, 0x000000, 0x0000FF, 0x000000, 0x5500AB, 0x000000, 0xAB0055, 0x000000};
// static const TProgmemRGBPalette16 redgreenwhite_p_p FL_PROGMEM = {0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x808080, 0x808080, 0x008000, 0x008000, 0x008000, 0x008000};
// static const TProgmemRGBPalette16 redwhite_p_p FL_PROGMEM = {0xFF0000, 0xFF0000, 0x808080, 0x808080, 0xFF0000, 0xFF0000, 0x808080, 0x808080, 0xFF0000, 0xFF0000, 0x808080, 0x808080, 0xFF0000, 0xFF0000, 0x808080, 0x808080};
// static const TProgmemRGBPalette16 retroc9_p_p FL_PROGMEM = {0xB80400, 0x902C02, 0xB80400, 0x902C02, 0x902C02, 0xB80400, 0x902C02, 0xB80400, 0x046002, 0x046002, 0x046002, 0x046002, 0x070758, 0x070758, 0x070758, 0x606820};
// static const TProgmemRGBPalette16 snowcolors_p FL_PROGMEM = {0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x555555, 0x555555, 0x555555, 0x555555, 0x555555, 0x555555, 0x555555, 0x555555, 0x555555, 0x555555, 0x555555, 0x555555};
// static const TProgmemRGBPalette16 snow_p_p FL_PROGMEM = {0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0xE0F0FF};


#endif // CONSTLEDS_H