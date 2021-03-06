#ifndef EFFECTMATH_H
#define EFFECTMATH_H

// #include "defleds.h"

// #include "../lamp.h"

// #ifdef ALS_LAMPMATRIX

#include "../fastled/base.h"

#define M_PI_2  1.57079632679489661923

#define MAP_SIN 1
#define MAP_COS 0
#define MOVE_X 1
#define MOVE_Y 0

static const PROGMEM float LUT[102] = {
     0,           0.0099996664, 0.019997334, 0.029991005, 0.039978687,
     0.049958397, 0.059928156,  0.069885999, 0.079829983, 0.089758173,
     0.099668652, 0.10955953,   0.11942893,  0.12927501,  0.13909595,
     0.14888994,  0.15865526,   0.16839015,  0.17809294,  0.18776195,
     0.19739556,  0.20699219,   0.21655031,  0.22606839,  0.23554498,
     0.24497867,  0.25436807,   0.26371184,  0.27300870,  0.28225741,
     0.29145679,  0.30060568,   0.30970293,  0.31874755,  0.32773849,
     0.33667481,  0.34555557,   0.35437992,  0.36314702,  0.37185606,
     0.38050637,  0.38909724,   0.39762798,  0.40609807,  0.41450688,
     0.42285392,  0.43113875,   0.43936089,  0.44751999,  0.45561564,
     0.46364760,  0.47161558,   0.47951928,  0.48735857,  0.49513325,
     0.50284320,  0.51048833,   0.51806855,  0.52558380,  0.53303409,
     0.54041952,  0.54774004,   0.55499572,  0.56218672,  0.56931317,
     0.57637525,  0.58337301,   0.59030676,  0.59717667,  0.60398299,
     0.61072594,  0.61740589,   0.62402308,  0.63057774,  0.63707036,
     0.64350110,  0.64987046,   0.65617871,  0.66242629,  0.66861355,
     0.67474097,  0.68080884,   0.68681765,  0.69276786,  0.69865984,
     0.70449406,  0.71027100,   0.71599114,  0.72165483,  0.72726268,
     0.73281509,  0.73831260,   0.74375558,  0.74914461,  0.75448018,
     0.75976276,  0.76499283,   0.77017093,  0.77529752,  0.78037310,
     0.78539819,  0.79037325};


class EffectMath {
public:
  static CRGB overrun;
  static uint8_t mapsincos8(bool map, uint8_t theta, uint8_t lowest = 0, uint8_t highest = 255);
  // static void MoveFractionalNoise(bool scale, const uint8_t noise3d[][WIDTH][HEIGHT], int8_t amplitude, float shift = 0);
  static void fadePixel(uint8_t i, uint8_t j, uint8_t step);
  static void fader(uint8_t step);
  static uint8_t ceil8(const uint8_t a, const uint8_t b);
  static CRGB makeBrighter( const CRGB& color, fract8 howMuchBrighter = 5);
  static CRGB makeDarker( const CRGB& color, fract8 howMuchDarker = 5);
  static float randomf(float min, float max);
  static bool isInteger(float val);
  static void addGlitter(uint8_t chanceOfGlitter = 127);
  static void nightMode(CRGB *leds);

/*
  static CRGB& piXY(CRGB *leds, byte x, byte y);
  static void screenscale( accum88 a, byte N, byte& screen, byte& screenerr);
  static void plot88(CRGB *leds, byte x, byte y, CRGB& color);
  static int16_t scale15by8_local( int16_t i, fract8 scale );
*/

    static uint32_t getPixColor(uint8_t sP, uint32_t thisSegm); 
    static uint32_t getPixColorXY(uint16_t x, uint16_t y); 
    static void fillAll(const CRGB &color); 
    static void fillAll(const CHSV &color); 
    static void drawPixelXY(int16_t x, int16_t y, const CRGB &color); 
    static void wu_pixel(uint32_t x, uint32_t y, CRGB col);
    static void drawPixelXYF(float x, float y, const CRGB &color, uint8_t darklevel=25); 
    static void drawPixelXYF_Y(uint16_t x, float y, const CRGB &color, uint8_t darklevel=50);
    static void drawPixelXYF_X(float x, uint16_t y, const CRGB &color, uint8_t darklevel=50);

    static CRGB getPixColorXYF_X(float x, uint16_t y);
    static CRGB getPixColorXYF_Y(uint16_t x, float y);
    static CRGB getPixColorXYF(float x, float y);

    static void drawLine(int x1, int y1, int x2, int y2, const CRGB &color);
    static void drawLineF(float x1, float y1, float x2, float y2, const CRGB &color);
    static void drawCircle(int x0, int y0, int radius, const CRGB &color);
    static void drawCircleF(float x0, float y0, float radius, const CRGB &color, float step = 0.25);
    static void setLedsfadeToBlackBy(uint16_t idx, uint8_t val);
    static void setLedsNscale8(uint16_t idx, uint8_t val);
    static void dimAll(uint8_t value);
    static CRGB getLed(uint8_t Sp, uint16_t idx);
    static CRGBW getLedW(uint8_t Sp, uint16_t idx);
    static void blur2d(uint8_t val);
    static CRGB *setLed(uint16_t idx, CHSV val);
    static CRGB *setLed(uint16_t idx, CRGB val);


    static double fmap(const double x, const double in_min, const double in_max, const double out_min, const double out_max){
        return (out_max - out_min) * (x - in_min) / (in_max - in_min) + out_min;
    }

    static float distance(float x1, float y1, float x2, float y2){
        float dx = x2 - x1, dy = y2 - y1;
        return EffectMath::sqrt((dx * dx) + (dy * dy));
    }
    static float sqrt(float x){
        union{
            int i;
            float x;
        } u;

        u.x = x;
        // u.i = (1<<29) + (u.i >> 1) - (1<<22);
        // u.i = 0x20000000 + (u.i >> 1) - 0x400000;
        u.i = (u.i >> 1) + 0x1FC00000;
        return u.x;
    }

    static float tan2pi_fast(float x) {
        float y = (1 - x*x);
        return x * (((-0.000221184 * y + 0.0024971104) * y - 0.02301937096) * y + 0.3182994604 + 1.2732402998 / y);
        //float y = (1 - x*x);
        //return x * (-0.0187108 * y + 0.31583526 + 1.27365776 / y);
    }

    static float atan2_fast(float y, float x)
    {
        //http://pubs.opengroup.org/onlinepubs/009695399/functions/atan2.html
        //Volkan SALMA

        const float ONEQTR_PI = PI / 4.0;
        const float THRQTR_PI = 3.0 * PI / 4.0;
        float r, angle;
        float abs_y = fabs(y) + 1e-10f;      // kludge to prevent 0/0 condition
        if ( x < 0.0f )
        {
            r = (x + abs_y) / (abs_y - x);
            angle = THRQTR_PI;
        }
        else
        {
            r = (x - abs_y) / (x + abs_y);
            angle = ONEQTR_PI;
        }
        angle += (0.1963f * r * r - 0.9817f) * r;
        if ( y < 0.0f )
            return( -angle );     // negate if in quad III or IV
        else
            return( angle );
    }

    static float atan_fast(float x){
        /*
        A fast look-up method with enough accuracy
        */
        if (x > 0) {
            if (x <= 1) {
            int index = round(x * 100);
            return LUT[index];
            } else {
            float re_x = 1 / x;
            int index = round(re_x * 100);
            return (M_PI_2 - LUT[index]);
            }
        } else {
            if (x >= -1) {
            float abs_x = -x;
            int index = round(abs_x * 100);
            return -(LUT[index]);
            } else {
            float re_x = 1 / (-x);
            int index = round(re_x * 100);
            return (LUT[index] - M_PI_2);
            }
        }
    }

    static float mapcurve(const float x, const float in_min, const float in_max, const float out_min, const float out_max, float (*curve)(float,float,float,float)){
        if (x <= in_min) return out_min;
        if (x >= in_max) return out_max;
        return curve((x - in_min), out_min, (out_max - out_min), (in_max - in_min));
    }
    static float linear(float t, float b, float c, float d) { return c * t / d + b; }
    static float InQuad(float t, float b, float c, float d) { t /= d; return c * t * t + b; }
    static float OutQuad(float t, float b, float c, float d) { t /= d; return -c * t * (t - 2) + b; }
    static float InOutQuad(float t, float b, float c, float d) {
        t /= d / 2;
        if (t < 1) return c / 2 * t * t + b;
        --t;
        return -c / 2 * (t * (t - 2) - 1) + b;
    }
    static float InCubic(float t, float b, float c, float d) { t /= d; return c * t * t * t + b; }
    static float OutCubic(float t, float b, float c, float d) { t = t / d - 1; return c * (t * t * t + 1) + b; }
    static float InOutCubic(float t, float b, float c, float d) {
        t /= d / 2;
        if (t < 1) return c / 2 * t * t * t + b;
        t -= 2;
        return c / 2 * (t * t * t + 2) + b;
    }
    static float InQuart(float t, float b, float c, float d) { t /= d; return c * t * t * t * t + b; }
    static float OutQuart(float t, float b, float c, float d) { t = t / d - 1; return -c * (t * t * t * t - 1) + b; }
    static float InOutQuart(float t, float b, float c, float d) {
        t /= d / 2;
        if (t < 1) return c / 2 * t * t * t * t + b;
        t -= 2;
        return -c / 2 * (t * t * t * t - 2) + b;
    }
    static float InQuint(float t, float b, float c, float d) { t /= d; return c * t * t * t * t * t + b; }

    static float fixed_to_float(int input){
        return ((float)input / (float)(1 << 16));
    }
    static int float_to_fixed(float input){
        return (int)(input * (1 << 16));
    }
    static float OutQuint(float t, float b, float c, float d) {
        t = t / d - 1;
        return c * (t * t * t * t * t + 1) + b;
    }
    static float InOutQuint(float t, float b, float c, float d) {
        t /= d / 2;
        if (t < 1) return  c / 2 * t * t * t * t * t + b;
        t -= 2;
        return c / 2 * (t * t * t * t * t + 2) + b;
    }
    // static float InSine(float t, float b, float c, float d) { return -c * Math.cos(t/d * (Math.PI/2)) + c + b; }
    // static float OutSine(float t, float b, float c, float d) { return c * Math.sin(t/d * (Math.PI/2)) + b; }
    // static float InOutSine(float t, float b, float c, float d) { return -c/2 * (Math.cos(Math.PI*t/d) - 1) + b; }
    static float InExpo(float t, float b, float c, float d) { return (t==0) ? b : c * powf(2, 10 * (t/d - 1)) + b; }
    static float OutExpo(float t, float b, float c, float d) { return (t==d) ? b+c : c * (-powf(2, -10 * t/d) + 1) + b; }
    static float InOutExpo(float t, float b, float c, float d) {
        if (t==0) return b;
        if (t==d) return b + c;
        t /= d / 2;
        if (t < 1) return c/2 * powf(2, 10 * (t - 1)) + b;
        --t;
        return c/2 * (-powf(2, -10 * t) + 2) + b;
    }
    static float InCirc(float t, float b, float c, float d) { t /= d; return -c * (sqrt(1 - t * t) - 1) + b; }
    static float OutCirc(float t, float b, float c, float d) { t = t / d - 1; return c * sqrt(1 - t * t) + b; }
    static float InOutCirc(float t, float b, float c, float d) {
        t /= d / 2;
        if (t < 1) return -c/2 * (sqrt(1 - t*t) - 1) + b;
        t -= 2;
        return c/2 * (sqrt(1 - t*t) + 1) + b;
    }
};



// #endif // 
#endif // EFFECTMATH_H