#ifndef DEFLEDS_H
  #define DEFLEDS_H

  #if defined __has_include
  #  if __has_include("user_config.h")
  #    include "user_config.h" // <- пользовательские настройки, пожалуйста меняйте все что требуется там, ЭТОТ ФАЙЛ (config.h) НЕ МЕНЯЙТЕ
  #  endif
  #endif



  //color mangling macros
  #define RGBW32(r,g,b,w) (uint32_t((byte(w) << 24) | (byte(r) << 16) | (byte(g) << 8) | (byte(b))))
  #define R(c) (byte((c) >> 16))
  #define G(c) (byte((c) >> 8))
  #define B(c) (byte(c))
  #define W(c) (byte((c) >> 24))

  #define DIM(c)      (uint32_t)((c >> 2) & 0x3f3f3f3f) // color at 25% intensity
  #define DARK(c)     (uint32_t)((c >> 4) & 0x0f0f0f0f) // color at  6% intensity





  #define DFTINTF_00A "Effet"
  #define DFTINTF_020 "Microphone"
  #define DFTINTF_084 "Palette"
  #define DFTINTF_088 "Echelle"
  #define DFTINTF_0D0 "Couleur"
  #define DFTINTF_0D1 "Palette/Échelle"
  #define DFTINTF_0D2 "Effet (0: cyclique)"
  #define DFTINTF_0D3 "Décalage des couleurs (0: cyclique)"
  #define DFTINTF_0D4 "Sous-pixel"
  #define DFTINTF_0D5 "Quantité"
  #define DFTINTF_0D6 "Générateur de couleurs"
  #define DFTINTF_0D7 "Teinte"
  #define DFTINTF_0D8 "Follement !"
  #define DFTINTF_0D9 "Quantité/Couleur (lampe de nuit)"
  #define DFTINTF_0DA "Saturation"
  #define DFTINTF_0DB "Gagner"
  #define DFTINTF_0DC "Balayer"
  #define DFTINTF_0DD "Ondulation"
  #define DFTINTF_0DE "Flou"
  #define DFTINTF_0DF "Flutter"
  #define DFTINTF_0E0 "Lampe de nuit"
  #define DFTINTF_0E1 "Anneaux"
  #define DFTINTF_0E2 "Éblouissement (0: Éteint 1: Aléatoire)"
  #define DFTINTF_0E3 "Mode"
  #define DFTINTF_0E4 "Vitesse linéaire"
  #define DFTINTF_0E5 "Mode (1: aléatoire)"
  #define DFTINTF_0E6 "Décalage"
  #define DFTINTF_0E7 "Quantité/Direction/Position"
  #define DFTINTF_0E8 "Prédateur"
  #define DFTINTF_0E9 "Masse"
  #define DFTINTF_0EA "Vitesse (Verticale)"
  #define DFTINTF_0EB "Vitesse (Horizontale)"
  #define DFTINTF_0EC "Motif (1: aléatoire)"
  #define DFTINTF_0ED "Ombrage"
  #define DFTINTF_0EE "Énergie"
  #define DFTINTF_0EF "Densité"
  #define DFTINTF_0F0 "Couleur/Rotation"
  #define DFTINTF_0F1 "Taille/Piste (1-85-170-255)"
  #define DFTINTF_0F2 "Fréquence de démarrage"
  #define DFTINTF_0F3 "Rotation"
  #define DFTINTF_0F4 "Palette (0: générateur)"
  #define DFTINTF_0F5 "Lissage"
  #define DFTINTF_0F6 "Remplir"
  #define DFTINTF_0F7 "Épaisseur"
  #define DFTINTF_0F9 "Comportement: 1/2"
  #define DFTINTF_0FA "Couleur: 1/2"
  #define DFTINTF_0FB "Palette (0: pas de couleur)"
  #define DFTINTF_0FC "Couleur (1: aléatoire, 255: blanc)"
  #define DFTINTF_0FD "Chute de neige/Chute d'étoiles/Météores"
  #define DFTINTF_0FE "Quantité"
  #define DFTINTF_0FF "Type 1/2, Flou 3/4"
  #define DFTINTF_100 "Palette (0: couleur aléatoire)"
  #define DFTINTF_101 "Couleur 1 (0: cyclique)"
  #define DFTINTF_102 "Nuage"
  #define DFTINTF_103 "Flou (0: non)"
  #define DFTINTF_104 "Balayer"
  #define DFTINTF_105 "Filtre"
  #define DFTINTF_106 "Physique"
  #define DFTINTF_107 "Cercles"
  #define DFTINTF_108 "Option 1/2"
  #define DFTINTF_109 "Clignote"
  #define DFTINTF_10A "Glitch"
  #define DFTINTF_10B "Orage"
  #define DFTINTF_10C "Quantité de coques"
  #define DFTINTF_10D "Taille (largeur)"
  #define DFTINTF_10E "Décalage de couleur (0: aléatoire)"
  #define DFTINTF_10F "Taille (hauteur)"
  #define DFTINTF_110 "Classique"
  #define DFTINTF_111 "Une couleur"
  #define DFTINTF_112 "Couleurs inversées"
  #define DFTINTF_113 "Couleur 2 (0: cyclique)"
  #define DFTINTF_114 "Qualité/Vitesse"
  #define DFTINTF_115 "Flou (1-4 avec vent)"
  #define DFTINTF_116 "Couleur (0: aléatoire, >0: cycle de couleur)"
  #define DFTINTF_117 "Couleur (0: aléatoire, 1: cyle, 2: couleur, 3: blanc)"
  #define DFTINTF_119 "Couleur (0: aléatoire, 1: cycle, 2: couleur)"
  #define DFTINTF_118 "cyle/couleur" 
  #define DFTINTF_120 "Palette (0: Cyle)"
  #define DFTINTF_121 "Cyle de rotation des Palettes"
  #define DFTINTF_122 "Cyle de rotation des coleurs"
  #define DFTINTF_123 "Variation"
  #define DFTINTF_124 "Couleur 1"
  #define DFTINTF_125 "Couleur 2"
  #define DFTINTF_126 "Taille"
  #define DFTINTF_127 "Double couleur"
  #define DFTINTF_128 "Angle"
  #define DFTINTF_129 "Apparition"


//Infrared codes for 24-key remote from http://woodsgood.ca/projects/2015/02/13/rgb-led-strip-controllers-ir-codes/
#define IR24_BRIGHTER  0xF700FF
#define IR24_DARKER    0xF7807F
#define IR24_OFF       0xF740BF
#define IR24_ON        0xF7C03F
#define IR24_RED       0xF720DF
#define IR24_REDDISH   0xF710EF
#define IR24_ORANGE    0xF730CF
#define IR24_YELLOWISH 0xF708F7
#define IR24_YELLOW    0xF728D7
#define IR24_GREEN     0xF7A05F
#define IR24_GREENISH  0xF7906F
#define IR24_TURQUOISE 0xF7B04F
#define IR24_CYAN      0xF78877
#define IR24_AQUA      0xF7A857
#define IR24_BLUE      0xF7609F
#define IR24_DEEPBLUE  0xF750AF
#define IR24_PURPLE    0xF7708F
#define IR24_MAGENTA   0xF748B7
#define IR24_PINK      0xF76897
#define IR24_WHITE     0xF7E01F
#define IR24_FLASH     0xF7D02F
#define IR24_STROBE    0xF7F00F 
#define IR24_FADE      0xF7C837 
#define IR24_SMOOTH    0xF7E817 

// 44-key defs, to be done later
#define IR44_BPLUS         0xFF3AC5  // 
#define IR44_BMINUS        0xFFBA45  // 
#define IR44_OFF           0xFF827D  // 
#define IR44_ON            0xFF02FD  // 
#define IR44_RED           0xFF1AE5  // 
#define IR44_GREEN         0xFF9A65  // 
#define IR44_BLUE          0xFFA25D  // 
#define IR44_WHITE         0xFF22DD  // natural white
#define IR44_REDDISH       0xFF2AD5  // 
#define IR44_GREENISH      0xFFAA55  // 
#define IR44_DEEPBLUE      0xFF926D  // 
#define IR44_WARMWHITE2    0xFF12ED  // warmest white
#define IR44_ORANGE        0xFF0AF5  // 
#define IR44_TURQUOISE     0xFF8A75  // 
#define IR44_PURPLE        0xFFB24D  // 
#define IR44_WARMWHITE     0xFF32CD  // warm white
#define IR44_YELLOWISH     0xFF38C7  // 
#define IR44_CYAN          0xFFB847  // 
#define IR44_MAGENTA       0xFF7887  // 
#define IR44_COLDWHITE     0xFFF807  // cold white
#define IR44_YELLOW        0xFF18E7  // 
#define IR44_AQUA          0xFF9867  // 
#define IR44_PINK          0xFF58A7  // 
#define IR44_COLDWHITE2    0xFFD827  // coldest white
#define IR44_REDPLUS       0xFF28D7  // 
#define IR44_GREENPLUS     0xFFA857  // 
#define IR44_BLUEPLUS      0xFF6897  // 
#define IR44_QUICK         0xFFE817  // 
#define IR44_REDMINUS      0xFF08F7  // 
#define IR44_GREENMINUS    0xFF8877  // 
#define IR44_BLUEMINUS     0xFF48B7  // 
#define IR44_SLOW          0xFFC837  // 
#define IR44_DIY1          0xFF30CF  // 
#define IR44_DIY2          0xFFB04F  // 
#define IR44_DIY3          0xFF708F  // 
#define IR44_AUTO          0xFFF00F  // 
#define IR44_DIY4          0xFF10EF  // 
#define IR44_DIY5          0xFF906F  // 
#define IR44_DIY6          0xFF50AF  // 
#define IR44_FLASH         0xFFD02F  // 
#define IR44_JUMP3         0xFF20DF  // 
#define IR44_JUMP7         0xFFA05F  // 
#define IR44_FADE3         0xFF609F  // 
#define IR44_FADE7         0xFFE01F  // 

#define ACTION_NONE             0
#define ACTION_BRIGHT_UP        1
#define ACTION_BRIGHT_DOWN      2
#define ACTION_SPEED_UP         3
#define ACTION_SPEED_DOWN       4
#define ACTION_INTENSITY_UP     5
#define ACTION_INTENSITY_DOWN   6
#define ACTION_POWER            7

  #define COLOR_WHITE         0xFFFFFF
  #define COLOR_BLACK         0x000000
  #define COLOR_RED           0xFF0000
  #define COLOR_REDDISH       0xFF7800
  #define COLOR_ORANGE        0xFFA000
  #define COLOR_YELLOWISH     0xFFC800
  #define COLOR_YELLOW        0xFFFF00
  #define COLOR_GREEN         0x00FF00
  #define COLOR_GREENISH      0x00FF78
  #define COLOR_TURQUOISE     0x00FFA0
  #define COLOR_CYAN          0x00FFDC
  #define COLOR_AQUA          0x00C8FF
  #define COLOR_BLUE          0x00A0FF
  #define COLOR_DEEPBLUE      0x0000FF
  #define COLOR_PURPLE        0xAA00FF
  #define COLOR_MAGENTA       0xFF00DC
  #define COLOR_PINK          0xFF00A0
  // #define COLOR_WHITE         0xFFFFFFFF
  #define COLOR_WARMWHITE2    0xFFFFAA69
  #define COLOR_WARMWHITE     0xFFFFBF8E
  #define COLOR_NEUTRALWHITE  0xFFFFD4B4
  #define COLOR_COLDWHITE     0xFFFFE9D9
  #define COLOR_COLDWHITE2    0xFFFFFFFF




  #ifndef MAX_FPS
      #define MAX_FPS         (60U)   
  #endif     
  #define EFFECTS_RUN_TIMER   (uint16_t)(1000 / MAX_FPS)    
  #define LED_SHOW_DELAY      1


  #define FASTLED_PALETTS_COUNT 22
  #define GRADIENT_PALETTS_COUNT 42

  #define SET_ALL_EFFFLAGS 3


  #ifndef BRIGHTNESS
    #define BRIGHTNESS              (255U)                        // стандартная максимальная яркость (0-255)
  #endif
  #ifndef CURRENT_LIMIT
    #define CURRENT_LIMIT           (2000U)                       // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит
  #endif

  #ifndef FADE_STEPTIME
    #define FADE_STEPTIME           (50U)                         // default time between fade steps, ms (2 seconds with max steps)
  #endif
  #ifndef FADE_TIME
    #define FADE_TIME               (2000U)                       // Default fade time, ms
  #endif
  #ifndef FADE_MININCREMENT
    #define FADE_MININCREMENT       (3U)                          // Minimal increment for brightness fade
  #endif
  #ifndef FADE_MINCHANGEBRT
    #define FADE_MINCHANGEBRT       (30U)                         // Minimal brightness for effects changer
  #endif

  #ifndef MAX_FPS
    #define MAX_FPS         (60U)                         // Максимальное число обсчитываемых и выводимых кадров в секунду
  #endif
  #define EFFECTS_RUN_TIMER       (uint16_t)(1000 / MAX_FPS)     // период обработки эффектов - при 10 это 10мс, т.е. 1000/10 = 100 раз в секунду, при 20 = 50 раз в секунду, желательно использовать диапазон 10...40

  #ifndef DEFAULT_DEMO_TIMER
      #define DEFAULT_DEMO_TIMER  (60U)                         // интервал смены демо по-умолчанию
  #endif

  #ifndef DEFAULT_TEXT_SPEED
    #define DEFAULT_TEXT_SPEED    (100U)                        // скорость движения текста, в миллисекундах - меньше == быстрее
  #endif

  #define SF(s) __SFSTR(s)
  #define __SFSTR(s) #s

  typedef struct {
    union {
      uint16_t flags;
      struct {
        bool isDebug:1;
        bool isRandDemo:1;
      };
    };
  } LAMPSTATE;
  /**
   * @enum      OUTPUT_EFFECTTYPE
   * 
   * @brief     mod por le type t'effet en cour de lecture
   * @details   
   * @see       https://github.com/AdriLighting/
   * 
   * 
   * @var OUTPUT_EFFECTTYPE::OET_NONE
   * null
   * @var OUTPUT_EFFECTTYPE::OET_HUETIMER
   *  rotation des couleures via HSV
   * @var OUTPUT_EFFECTTYPE::OET_PAL
   *  rotation des couleur avec CRGBPalette16
   * @var OUTPUT_EFFECTTYPE::OET_PALGRAD
   * rotation des couleur avec CRGBPalette32 
   * 
   * @author    Adrilighting
   * @date      mar. 28 déc. 2021 16:25:30
   */
  typedef enum _OUTPUT_EFFECTTYPE { 
    OET_NONE = 0,   // 
    OET_HUETIMER,   // 
    OET_PAL,        // 
    OET_PALGRAD,    // 
    OET_HTPG,    // 
  } OUTPUT_EFFECTTYPE;

#endif

