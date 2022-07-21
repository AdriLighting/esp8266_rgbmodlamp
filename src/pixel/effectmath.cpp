#include "../../include/pixel/effectmath.h"
uint16_t XY(uint8_t sP, uint8_t x, uint8_t y) {
    bool roate;
    leds_managePtrGet()->get_matrix(sP)->get_rotatedMatrix(roate);
    if (roate)  return leds_managePtrGet()->getPixelNumber(sP, y, x);
    else        return leds_managePtrGet()->getPixelNumber(sP, x, y);
}
CRGB EffectMath::getLed(uint8_t sP, uint16_t idx) { return leds_managePtrGet()->get_crgbArray(sP)[idx]; }
CRGBW EffectMath::getLedW(uint8_t sP, uint16_t idx) { return leds_managePtrGet()->get_crgwbArray(sP)[idx]; }





// void EffectMath::fadePixel(uint8_t sP, uint8_t i, uint8_t j, uint8_t step) {
//     int32_t pixelNum = leds_managePtrGet()->getPixelNumber(sP, i, j);
//     if (EffectMath::getPixColor(pixelNum) == 0U) return;

//     CRGB led = EffectMath::getLed(sP, pixelNum);
//     if (led.r >= 30U ||
//         led.g >= 30U ||
//         led.b >= 30U){
//         EffectMath::setLedsfadeToBlackBy(pixelNum,step);
//     }
//     else{
//         EffectMath::setLed(pixelNum, 0U);
//     }
// }
