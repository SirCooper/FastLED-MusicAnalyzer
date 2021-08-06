#ifndef FastLedFX_H
#define FastledFX_H

#include <Arduino.h>
#include <FastLED.h>
#include "MusicAnalyzer.h"
struct SpotifyPass
{
  bool SpotifyRead=false;
  uint32_t nextBeat,nextBars,SpotifyEnd;
  uint16_t BeatsPointer,BarsPointer;
  
  int statusCode;
};
class FastLEDFX
{
  public:

    FastLEDFX(frq *pass_value,CHSV *FXLED);
    
    void musicPalette(CHSVPalette16& recievePalette);
    void visualize(struct CRGB*pfirstLed, uint16_t start_led, uint16_t num_leds, bool reverse, uint8_t display_prgm,uint8_t reduce_prgm, bool loud, bool position_fixed, uint8_t reduce);
    
    uint8_t peakLength=4;
    
    CHSVPalette16 musicPalette(CRGBPalette16& recievePalette);
    
  private:
  
    typedef int (FastLEDFX::*f)(uint16_t, uint16_t, bool, bool);
    typedef int (FastLEDFX::*f2)(uint16_t, uint16_t, uint8_t);
    
    int nothing(uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed);
    int FreqSimple(uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed);
    int FreqDouble(uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed);
    int FreqFixedLength(uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed);
    int PeaksSimple(uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed);
    int PeaksDouble(uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed);
    int PeaksFixedLength(uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed);
    int PeaksAsSpots(uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed);
    int BeatSpeed(uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed);
    int BeatPush(uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed);
    int DisplayLoudness(uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed);
    
    int ReduceSimple(uint16_t start_led, uint16_t num_leds, uint8_t reduce);
    int ReduceComputed(uint16_t start_led, uint16_t num_leds, uint8_t reduce);
    
    static const f Programm [];
    static const f2 Reduce [];
    
    CHSVPalette16 palette;
    
    frq *_pass_value;
    
    CHSV *_FXLED;
    
};
#endif
