/*
    music.splitAuxValue(reduceValue of peakValues, minimal_value of max_loudness)

    frq passvalue:
      bool beat;              /
      bool peakNow[10];
      uint8_t freq[10];
      uint8_t peak[10];
      uint8_t resolution;
      uint8_t loudness_computed=0;
      uint8_t loudness=0;
      uint8_t loudness_old;
      uint8_t max_loudness;

    FX.visualize(CRGB_ARRAY, start_led, number_of_leds, display_reverse?, programm_pointer, reduce_pointer, loudness_to_brightness, fixed_positions, brightness_reduce);

    programm_pointer:

      nothing = 0
      FreqSimple = 1
      FreqDouble = 2
      FreqFixedLength = 3
      PeaksSimple = 4
      PeaksDouble = 5
      PeaksFixedLength = 6
      PeakAsSpots = 7
      BeatSpeed = 8
      BeatPush = 9
      DisplayLoudness = 10

    reduce_pointer:
      reduce_fix = 0;
      reduce_computed = 1;

*/
#include <Arduino.h>
#include <FastLED.h>

#include "FastLEDFX.h"
#include "MusicAnalyzer.h"

#define AUX_PIN 34
#define DATA_PIN 18
#define NUM_LEDS 480

#define SAMPLES 1024             // Must be a power of 2
#define SAMPLING_FRQUENCY 40000 // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.

CRGB leds[NUM_LEDS];
CHSV FXLED[NUM_LEDS];
CRGBPalette16 currentPalette = RainbowColors_p;

double vReal[SAMPLES], vImag[SAMPLES];

frq passvalue;

FastLEDFX FX(&passvalue, FXLED);
MusicAnalyzer music(AUX_PIN, vReal, vImag, &passvalue, SAMPLES, SAMPLING_FRQUENCY);

uint8_t frequency_reduce = 10;
uint8_t min_max_loudness = 10;


uint8_t programm_pointer = 2;
uint8_t reduce_pointer = 1;
uint8_t brightness_reduce = 10;
bool fixed_positions = true;
bool loudness_to_brightness = false;

void setup()
{
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(200);
  FX.musicPalette(currentPalette);
}

void loop()
{
  music.sample();
  music.splitAuxValues(frequency_reduce, min_max_loudness);
  music.beatRecognize();
  EVERY_N_MILLISECONDS(10)
  {
    FX.visualize(leds, 0, NUM_LEDS, 0, programm_pointer, reduce_pointer, loudness_to_brightness, fixed_positions, brightness_reduce);
    FastLED.show();
  }
}
