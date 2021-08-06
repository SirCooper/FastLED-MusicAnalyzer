
#ifndef MusicAnalyzer_H
#define MusicAnalyzer_H

#include <Arduino.h>
#include "arduinoFFT.h" // Standard Arduino FFT library https://github.com/kosme/arduinoFFT


typedef struct  
{  
  bool beat;
  bool peakNow[10];
  uint8_t freq[10];
  uint8_t peak[10];
  uint8_t resolution;
  uint8_t loudness_computed=0;
  uint8_t loudness=0;
  uint8_t loudness_old;
  uint8_t max_loudness;
} frq;


class MusicAnalyzer
{
public:

MusicAnalyzer(uint8_t pin, double *vReal,double *vImag,frq *pass_frequency, uint16_t samples, uint16_t sampling_frequency);

void sample();
void splitAuxValues(uint8_t reduce,uint8_t min_max_loudness);
void beatRecognize();

private:

  arduinoFFT _FFT;
  
  uint8_t _pin;  
  uint8_t _resolution;
  
  uint16_t _samples;
  uint16_t _sampling_frequency;
  
  unsigned long readTime;
  
  double *_vReal;
  double *_vImag;
    
  frq *_pass_frequency;
  
  uint8_t setResolution();
  
};

#endif
