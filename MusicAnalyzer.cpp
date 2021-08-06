#include "MusicAnalyzer.h"

MusicAnalyzer::MusicAnalyzer(uint8_t pin, double *vReal, double *vImag, frq *pass_frequency, uint16_t samples, uint16_t samplingFrequency)
{
  this->_pin = pin;
  this->_vReal = vReal;
  this->_vImag = vImag;
  this->_pass_frequency = pass_frequency;
  this->_samples = samples;
  this->_sampling_frequency = samplingFrequency;
  this->_resolution = setResolution();
  this->_FFT = arduinoFFT(vReal, vImag, samples, samplingFrequency);
}

void MusicAnalyzer::sample()
{
  // Sampling
  unsigned int sampling_period_us;
  sampling_period_us = round(1000000 * (1.0 / this->_sampling_frequency));
  for (int i = 0; i < this -> _samples; i++)
  {
    //arraypointer1[i]= 0;
    readTime = micros();
    this->_vReal[i] = analogRead(this->_pin);
    this->_vImag[i] = 0;
    //Serial.println(arraypointer1[i]);
    while ((micros() - (readTime)) < sampling_period_us) {
      /* do nothing to wait */
    }
  }
  //FFT
  this-> _FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  this->_FFT.Compute(FFT_FORWARD);
  this->_FFT.ComplexToMagnitude();
}

void MusicAnalyzer::splitAuxValues(uint8_t reduce, uint8_t min_max_loudness)
{
  this->_pass_frequency->resolution = _resolution;

  uint8_t i = 1;                  //pointer to final partion
  uint16_t j = 2;                 //counter for samples
  uint16_t  temploudness = 0;

  while (i <= _resolution)
  {
    uint32_t vTemp = 0;
    uint16_t count = 0;

    while (((j - 1) >> i) == 0 && j < this->_samples / 2)
    {
      if (this->_vReal[j] > 1500)
      {
        vTemp += this->_vReal[j] / 16;
        count += 1;
      }
      j++;
    }
    if (vTemp != 0) {
      vTemp = ((vTemp / count));
    }
    this->_pass_frequency->freq[i - 1] = vTemp;

    if (this->_pass_frequency->peak[i - 1] < this->_pass_frequency->freq[i - 1])
    {
      this->_pass_frequency->peak[i - 1] = this->_pass_frequency->freq[i - 1];
      this->_pass_frequency->peakNow[i - 1] = true;
    }
    else if (reduce <= this->_pass_frequency->peak[i - 1])
    {
      this->_pass_frequency->peak[i - 1] -= reduce;
      this->_pass_frequency->peakNow[i - 1] = false;
    }
    else
    {
      this->_pass_frequency->peak[i - 1] = 0;
      this->_pass_frequency->peakNow[i - 1] = false;
    }

    temploudness += this->_pass_frequency->freq[i - 1];
    i++;
  }
  this->_pass_frequency->loudness_old = this->_pass_frequency->loudness_computed;
  this->_pass_frequency->loudness = temploudness / this->_pass_frequency->resolution;
  if (this->_pass_frequency->max_loudness < this->_pass_frequency->loudness)
  {
    this->_pass_frequency->max_loudness = this->_pass_frequency->loudness;
  }
  else this->_pass_frequency->max_loudness = (this->_pass_frequency->max_loudness + (this->_pass_frequency->loudness > min_max_loudness ? this->_pass_frequency->loudness : min_max_loudness)) / 2;
  this->_pass_frequency->loudness = map(this->_pass_frequency->loudness, 0, this->_pass_frequency->max_loudness, 0, 255);
  if (this->_pass_frequency->loudness_computed > 2 || this->_pass_frequency->loudness != 0)
  {
    this->_pass_frequency->loudness_computed = (this->_pass_frequency->loudness_computed + this->_pass_frequency->loudness) / 2;
  }
  else
  {
    this->_pass_frequency->loudness_computed = 0;
  }
}



uint8_t MusicAnalyzer::setResolution()
{
  uint8_t res = 2;
  while ((this->_samples >> res) != 0)
  {
    res++;
  }
  res -= 2;
  return res;
}


void MusicAnalyzer::beatRecognize()
{
  uint16_t tempBeat = 0;
  uint8_t i = 0;
  for (i; i < (this->_pass_frequency->resolution > 7 ? this->_pass_frequency->resolution - 7 : 1); i++)  
  {
    tempBeat += this->_pass_frequency->freq[i];
  }
  tempBeat /= i;
  if (tempBeat > 30)
  {
    this->_pass_frequency->beat = true;
  }
  else
  {
    this->_pass_frequency->beat = false;
  }
}
//400HZMAXBASS
