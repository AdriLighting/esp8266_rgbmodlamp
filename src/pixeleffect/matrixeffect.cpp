#include "../../include/pixeleffect/matrixeffect.h"

#ifdef ALS_LAMPMATRIX
        uint16_t getmaxDim() {return ((WIDTH>HEIGHT)?WIDTH:HEIGHT);}
        uint16_t getminDim() {return ((WIDTH<HEIGHT)?WIDTH:HEIGHT);}  



// радуги 2D
// ------------- радуга вертикальная/горизонтальная ----------------
//uint8_t hue;
bool MEffectRainbow::rainbowHorVertRoutine(bool isVertical)
{
  for (uint8_t i = 0U; i < (isVertical?WIDTH:HEIGHT); i++)
  {
#ifdef MIC_EFFECTS
    uint8_t micPeak = myLamp->getMicMapMaxPeak();
    CHSV thisColor = CHSV((uint8_t)(hue + i * (micPeak > map(speed, 1, 255, 100, 10) and isMicOn() ? (scale - micPeak) : scale) % 170), 255, 255); // 1/3 без центральной между 1...255, т.е.: 1...84, 170...255
#else
    CHSV thisColor = CHSV((uint8_t)(hue + i * scale%170), 255, LAMPPTRGET()->get_globalBrightness(0, brightness));
#endif
    for (uint8_t j = 0U; j < (isVertical?HEIGHT:WIDTH); j++)
    {
      EffectMath::drawPixelXY((isVertical?i:j), (isVertical?j:i), thisColor);
    }
  }
  return true;
}

void MEffectRainbow::load(uint8_t strip_pos){
  setup(strip_pos);
  speed = 220;
}
String MEffectRainbow::setDynCtrl(UIControl*_val) {set_dynCtrl(_val); return String();}

bool MEffectRainbow::run(){
  return rainbowDiagonalRoutine(*&_ledarray);
}

bool MEffectRainbow::rainbowDiagonalRoutine(CRGB *leds)
{
  // коэф. влияния замаплен на скорость, 4 ползунок нафиг не нужен
  hue += (6.0 * (speed / 255.0) + 0.05 ); // скорость смещения цвета зависит от кривизны наклна линии, коэф. 6.0 и 0.05

  if(scale<85){
    rainbowHorVertRoutine(false);
    return true;
  } else if (scale>170){
    rainbowHorVertRoutine(true);
    return true;
  }

  for (uint8_t i = 0U; i < WIDTH; i++)
  {
    for (uint8_t j = 0U; j < HEIGHT; j++)
    {
      float twirlFactor = EffectMath::fmap((float)scale, 85, 170, 8.3, 24);      // на сколько оборотов будет закручена матрица, [0..3]
#ifdef MIC_EFFECTS
      twirlFactor *= myLamp->getMicMapMaxPeak() > map(speed, 1, 255, 80, 10) and isMicOn() ? 1.5f * ((float)myLamp->getMicMapFreq() / 255.0f) : 1.0f;
#endif
      CRGB thisColor = CHSV((uint8_t)(hue + ((float)WIDTH / (float)HEIGHT * i + j * twirlFactor) * ((float)255 / (float)getmaxDim())), 255, LAMPPTRGET()->get_globalBrightness(0, brightness));
      EffectMath::drawPixelXY(i, j, thisColor);
    }
  }
  return true;
}


void EffectLighters::load(uint8_t strip_pos){
  randomSeed(millis());
  for (uint8_t i = 0U; i < LIGHTERS_AM; i++)
  {
    lightersIdx=0;
    lightersPos[0U][i] = random(0, WIDTH);
    lightersPos[1U][i] = random(0, HEIGHT);
    lightersSpeed[0U][i] = (float)random(-200, 200) / 10.0f;
    lightersSpeed[1U][i] = (float)random(-200, 200) / 10.0f;
    lightersColor[i] = random(0U, 255U);
    light[i] = random(1U, 3U)*127U;
  }
}

bool EffectLighters::run(){
  return lightersRoutine(*&_ledarray);
}

String EffectLighters::setDynCtrl(UIControl*_val) {
  if(_val->getId()==3) cnt = EffectCalc::setDynCtrl(_val).toInt();
  else if(_val->getId()==4) subPix = EffectCalc::setDynCtrl(_val).toInt();
  else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
  return String();
}

bool EffectLighters::lightersRoutine(CRGB *leds)
{
  float speedfactor = (float)speed / 4096.0f + 0.001f;

 // EffectMath::blur2d(speed/10);
  //EffectMath::dimAll(50 + speed/10);
  memset8( leds, 0, NUM_LEDS * 3);

  EVERY_N_MILLIS(333)
  {
    lightersIdx = (lightersIdx+1)%constrain(cnt,1,LIGHTERS_AM);
    lightersSpeed[0U][lightersIdx] += random(-10, 10);
    lightersSpeed[1U][lightersIdx] += random(-10, 10);
    lightersSpeed[0U][lightersIdx] = fmod(lightersSpeed[0U][lightersIdx], 21);
    lightersSpeed[1U][lightersIdx] = fmod(lightersSpeed[1U][lightersIdx], 21);
    light[lightersIdx] = random(255U-(cnt*8),255U);
    if(!random(cnt+3))
      light[lightersIdx] = 127;
  }

  for (uint8_t i = 0U; i < constrain(cnt,1,LIGHTERS_AM); i++) // масштабируем на LIGHTERS_AM, чтобы не было выхода за диапазон
  {
    // EVERY_N_SECONDS(1)
    // {
    //   LOG.printf_P("S0:%d S1:%d P0:%3.2f P1:%3.2f, cnt:%3.2f\n", lightersSpeed[0U][i], lightersSpeed[1U][i],lightersPos[0U][i],lightersPos[1U][i],speedfactor);
    // }
    lightersPos[0U][i] += lightersSpeed[0U][i]*speedfactor;
    lightersPos[1U][i] += lightersSpeed[1U][i]*speedfactor;

    if (lightersPos[0U][i] < 0) lightersPos[0U][i] = (float)(WIDTH - 1);
    if (lightersPos[0U][i] >= (float)WIDTH) lightersPos[0U][i] = 0.0f;

    if (lightersPos[1U][i] <= 0.0f)
    {
      lightersPos[1U][i] = 0.0f;
      lightersSpeed[1U][i] = -lightersSpeed[1U][i];
      lightersSpeed[0U][i] = -lightersSpeed[0U][i];
    }
    if (lightersPos[1U][i] >= (int32_t)(HEIGHT - 1))
    {
      lightersPos[1U][i] = (HEIGHT - 1U);
      lightersSpeed[1U][i] = -lightersSpeed[1U][i];
      lightersSpeed[0U][i] = -lightersSpeed[0U][i];
    }

    if (subPix)
      EffectMath::drawPixelXYF(lightersPos[0U][i], lightersPos[1U][i], CHSV(lightersColor[i], 255U-(i*2), light[i]), 0);
    else
      EffectMath::drawPixelXY((uint8_t)lightersPos[0U][i], (uint8_t)lightersPos[1U][i], CHSV(lightersColor[i], 255U-(i*2), light[i]));
  }
  return true;
}


String MEffectMatrix::setDynCtrl(UIControl*_val)
{
  set_dynCtrl(_val);
  if(_val->getId()==2)      _scale = EffectCalc::setDynCtrl(_val).toInt();
  else if(_val->getId()==5) hue = EffectCalc::setDynCtrl(_val).toInt();
  // else if(_val->getId()==5) _cMod = EffectCalc::setDynCtrl(_val).toInt();
  else if(_val->getId()==7) gluk = EffectCalc::setDynCtrl(_val).toInt();


  if (hue == 2) {
    randColor   = false;
    white       = false;
    timer       = true;    
    colorCycle  = false;
  } else if (hue == 1) {
    timer       = false;
    randColor   = false; 
    white       = false; 
    colorCycle  = true; 
  } else if (hue == 0) {
    randColor   = true;
    white       = false;
    timer       = false;  
    colorCycle  = false; 
  }
  return String();
}

bool MEffectMatrix::run(){
  //fpsmeter();
  return matrixRoutine(*&_ledarray);
}

void MEffectMatrix::load(uint8_t strip_pos){
  setup(strip_pos);
  speed = 220;  
  randomSeed(analogRead(millis()));
  for (uint8_t i = 0U; i < LIGHTERS_AM; i++)
  {
    lightersPos[0U][i] = random(0, WIDTH);
    lightersPos[1U][i] = EffectMath::randomf(HEIGHT - HEIGHT /2, HEIGHT);
    lightersSpeed[0U][i] = 1;
    lightersSpeed[1U][i] = (float)random(10, 20) / 10.0f;
    lightersColor[i] = hue;
    light[i] = random(196,255);
  }
}

bool MEffectMatrix::matrixRoutine(CRGB *leds)
{

  float speedfactor = EffectMath::fmap((float)speed, 1., 255., 0.06, 0.4);
  EffectMath::dimAll(map(speed, 1, 255, 252, 240));
  
  CHSV color;

  for (uint8_t i = 0U; i < map(_scale, 1, 32, 1, LIGHTERS_AM); i++)
  {
    lightersPos[1U][i] -= lightersSpeed[1U][i]*speedfactor;

    if (white) {
      color = rgb2hsv_approximate(CRGB::Gray);
      color.val = light[i];
    } else if (randColor) {
      EVERY_N_MILLIS((600 / speedfactor)*2) {
        hue = random(1, 250);
      }
      color = CHSV(hue, 255, light[i]);
    } else if (colorCycle) {
      color = CHSV(_hue_1V, 255, light[i]);   
    } else {
      color = CHSV(hue, 255, light[i]);
    }


    EffectMath::drawPixelXYF_Y(lightersPos[0U][i], lightersPos[1U][i], color);

    count += speedfactor;

    if (gluk > 1 and (uint8_t)count%2 == 0) 
      if (random8() < gluk) {
        lightersPos[0U][i] = lightersPos[0U][i] + random(-1, 2);
        light[i] = random(196,255);
      }

    if(lightersPos[1U][i] < -1) {
      lightersPos[0U][i] = random(0, WIDTH);
      lightersPos[1U][i] = EffectMath::randomf(HEIGHT - HEIGHT /2, HEIGHT);
      lightersSpeed[1U][i] = EffectMath::randomf(1.5, 2.5); 
      light[i] = random(127U, 255U);
      lightersColor[i] = hue;
    }
  }

  return true;
}
#endif