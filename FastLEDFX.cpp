
#include "FastLEDFX.h"
#include "FastLED.h"
FastLEDFX::FastLEDFX(frq *pass_value,CHSV *FXLED)
{
  this->_pass_value=pass_value;
  this->_FXLED=FXLED;
}

const FastLEDFX::f  FastLEDFX::Programm[]=   
    {
      &FastLEDFX::nothing,
      &FastLEDFX::FreqSimple,
      &FastLEDFX::FreqDouble,
      &FastLEDFX::FreqFixedLength,
      &FastLEDFX::PeaksSimple,
      &FastLEDFX::PeaksDouble,
      &FastLEDFX::PeaksFixedLength,
      &FastLEDFX::PeaksAsSpots,
      &FastLEDFX::BeatSpeed,
      &FastLEDFX::BeatPush,
      &FastLEDFX::DisplayLoudness,
      //&FastLEDFX::Musik_FX,
    };
    
void FastLEDFX::visualize(struct CRGB* pfirstLed, uint16_t start_led, uint16_t num_leds,bool reverse, uint8_t display_prgm,uint8_t reduce_prgm, bool loud, bool position_fixed, uint8_t reduce)
{
  (this->*(Programm[display_prgm]))(start_led, num_leds, loud, position_fixed);
  (this->*(Reduce[reduce_prgm]))(start_led, num_leds, reduce);
  for (uint16_t i = 0; i < num_leds; i++)
  {
   hsv2rgb_rainbow(this->_FXLED[(reverse?((start_led+num_leds-1)-(i)):(i+start_led))],pfirstLed[i]);
  }
}
int FastLEDFX:: nothing( uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed)
{
  return 0;
}
int FastLEDFX:: FreqSimple( uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed)
{
  uint16_t pointer = start_led;
  uint8_t bright = 255;
  for (int i = 0; i < this->_pass_value->resolution; i++)
  {
    CHSV col=ColorFromPalette(palette, i * 255/this->_pass_value->resolution, (loud ? this->_pass_value->loudness_computed : bright));
    uint16_t length_Band = map(this->_pass_value->freq[i], 0, 255, 0, num_leds / this->_pass_value->resolution);      
    uint16_t start_val = pointer; 
      for (pointer; pointer < length_Band + start_val; pointer++)
      {
        this->_FXLED[pointer] = col;
      }      
    pointer += (position_fixed ? (num_leds/this->_pass_value->resolution)-length_Band: 0);
  }
}

int FastLEDFX:: FreqDouble( uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed)
{
  uint16_t pointer =0;
  uint8_t bright = 255;
  uint16_t _endled=start_led+num_leds-1;
  for (int i = 0; i < this->_pass_value->resolution; i++)
  {
    CHSV col=ColorFromPalette(palette, i * 255/this->_pass_value->resolution, (loud ? this->_pass_value->loudness_computed : bright));
    uint16_t length_Band = map(this->_pass_value->freq[i], 0, 255, 0, ((num_leds / this->_pass_value->resolution)/2));
    uint16_t start_val = pointer;
    for (pointer; pointer < length_Band+start_val; pointer++)
    {            
      this->_FXLED[pointer] = col;
      this->_FXLED[(_endled) - pointer] = col;
    }
    pointer += (position_fixed ? (num_leds/this->_pass_value->resolution)/2-length_Band: 0);  }
}

int FastLEDFX:: FreqFixedLength( uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed)
{
  uint16_t pointer = start_led;
  uint16_t start_val = pointer;
  uint8_t bright = 255;
  for (int i = 0; i < this->_pass_value->resolution; i++)
  {  
    uint16_t length_Band = num_leds / this->_pass_value->resolution;
    start_val=pointer;
    for (pointer; pointer < length_Band+start_val; pointer++)
      {
        bright=this->_pass_value->freq[i];
        CHSV colval = ColorFromPalette(palette, i * 255/this->_pass_value->resolution, (loud ? (this->_pass_value->loudness_computed * bright / 255) : bright));
        this->_FXLED[pointer] = colval;
      }
  }
}
int FastLEDFX:: PeaksSimple( uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed)
{
  uint16_t pointer = start_led;
  uint8_t bright = 255;
  for (int i = 0; i < this->_pass_value->resolution; i++)
  {
    CHSV col=ColorFromPalette(palette, i * 255/this->_pass_value->resolution, (loud ? this->_pass_value->loudness_computed : bright));
    uint16_t length_Band = map(this->_pass_value->peak[i], 0, 255, 0, num_leds / this->_pass_value->resolution);      
    uint16_t start_val = pointer; 
      for (pointer; pointer < length_Band + start_val; pointer++)
      {
        this->_FXLED[pointer] = col;
      }      
    pointer += (position_fixed ? (num_leds/this->_pass_value->resolution)-length_Band: 0);
  }
}

int FastLEDFX:: PeaksDouble( uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed)
{
  uint16_t pointer =0;
  uint8_t bright = 255;
  uint16_t _endled=start_led+num_leds-1;
  for (int i = 0; i < this->_pass_value->resolution; i++)
  {
    CHSV col=ColorFromPalette(palette, i * 255/this->_pass_value->resolution, (loud ? this->_pass_value->loudness_computed : bright));
    uint16_t length_Band = map(this->_pass_value->peak[i], 0, 255, 0, ((num_leds / this->_pass_value->resolution)/2));
    uint16_t start_val = pointer;
    for (pointer; pointer < length_Band+start_val; pointer++)
    {            
      this->_FXLED[pointer] = col;
      this->_FXLED[(_endled) - pointer] = col;
    }
    pointer += (position_fixed ? (num_leds/this->_pass_value->resolution)/2-length_Band: 0);  }
}

int FastLEDFX:: PeaksFixedLength( uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed)
{
  uint16_t pointer = start_led;
  uint16_t start_val = pointer;
  uint8_t bright = 255;
  for (int i = 0; i < this->_pass_value->resolution; i++)
  {  
    uint16_t length_Band = num_leds / this->_pass_value->resolution;
    start_val=pointer;
    for (pointer; pointer < length_Band+start_val; pointer++)
      {
        bright=this->_pass_value->peak[i];
        CHSV colval = ColorFromPalette(palette, i * 255/this->_pass_value->resolution, (loud ? (this->_pass_value->loudness_computed * bright / 255) : bright));
        this->_FXLED[pointer] = colval;
      }
  }
}

int FastLEDFX:: DisplayLoudness( uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed)
{
  uint16_t pointer = start_led;
  uint16_t start_val = pointer;
  uint8_t bright = 255;
  uint8_t index=0;
  uint16_t loudnesslength = map(this->_pass_value->loudness_computed, 0, 255, 0, num_leds);
  for (pointer; pointer < start_val + loudnesslength; pointer++)
  {
    this->_FXLED[pointer] = ColorFromPalette(palette, map(index, 0, num_leds, 0, 255), (loud ? this->_pass_value->loudness_computed : 255));
    index++;
  }
}
int FastLEDFX:: BeatPush( uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed)
{
  uint16_t pointer = start_led;
  uint16_t start_val = pointer;
  uint8_t bright = 255;
  uint8_t index=0;
  if (this->_pass_value->beat)
  {
      pointer = pointer + num_leds - 1;
      for (pointer; pointer > start_val; pointer--)
      {
        this->_FXLED[pointer] = this->_FXLED[pointer - 1];
      }
      this->_FXLED[start_val] = ColorFromPalette(palette, this->_pass_value->loudness_computed, (loud ? this->_pass_value->loudness_computed : 255));
    }
  }
int FastLEDFX:: BeatSpeed( uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed)
{
  uint16_t pointer = start_led;
  uint16_t start_val = pointer;
  uint8_t bright = 255;
  uint8_t index = 0;
  pointer = pointer + num_leds - 1;
  for (pointer; pointer > start_val; pointer--)
  {
    this->_FXLED[pointer] = this->_FXLED[pointer - 1];
  }
  if (this->_pass_value->beat)this->_FXLED[start_val] = ColorFromPalette(palette, this->_pass_value->loudness_computed, (loud ? this->_pass_value->loudness_computed : 255));
  else this->_FXLED[start_val]=CHSV(0,0,0);
}
 int FastLEDFX::PeaksAsSpots( uint16_t start_led, uint16_t num_leds, bool loud, bool position_fixed)
{

  for (int band = 0; band < this->_pass_value->resolution; band++)
  {
    if (this->_pass_value->peakNow[band] == true)
    {
      uint16_t numb = random16(num_leds);
      for (int count = 0; count < peakLength * 2; count++)
      {
        if (numb + count < num_leds)
        {
          if (count < peakLength)
          {  
            this->_FXLED[numb + count + start_led] = ColorFromPalette(palette,band*32,map(count,0,peakLength,(loud?(this->_pass_value->loudness_computed/2) : 100),(loud?(this->_pass_value->loudness_computed) : 255)));
          }
          else
          {
            this->_FXLED[numb + count + start_led] = ColorFromPalette(palette,band*32,map((peakLength * 2) - 1 - count,0,peakLength,(loud?(this->_pass_value->loudness_computed/2) : 100),(loud?(this->_pass_value->loudness_computed) : 255)));
          }
        }
      }
    }
  }
}

const FastLEDFX::f2  FastLEDFX::Reduce[]= 
{
  &FastLEDFX::ReduceSimple,
  &FastLEDFX::ReduceComputed,
};

int FastLEDFX:: ReduceSimple( uint16_t start_led, uint16_t num_leds, uint8_t reduce)
{
  for(uint16_t pointer=start_led;pointer<start_led+num_leds;pointer++)
  {
    this->_FXLED[pointer].v-=(this->_FXLED[pointer].v<reduce?this->_FXLED[pointer].v:reduce);
  }
}
int FastLEDFX:: ReduceComputed( uint16_t start_led, uint16_t num_leds, uint8_t reduce)
{
  uint8_t malus=reduce-map(this->_pass_value->loudness_computed,0,255,0,reduce);
  for(uint16_t pointer=start_led;pointer<start_led+num_leds;pointer++)
  {
    this->_FXLED[pointer].v-=(this->_FXLED[pointer].v<malus?this->_FXLED[pointer].v:malus);
  }
}
void FastLEDFX::musicPalette(CHSVPalette16& recievePalette)
{
  palette=recievePalette;
}

CHSVPalette16 FastLEDFX::musicPalette(CRGBPalette16& recievePalette)
{
  uint8_t* p1;
  uint8_t* p2;
  p1 = (uint8_t*)recievePalette.entries;
  p2 = (uint8_t*)palette.entries;
  const uint8_t totalChannels = sizeof(CRGBPalette16);
  uint8_t i = 0;
  CHSV color;

  while (i < totalChannels)
  {
    char colorchar[30];
    color = rgb2hsv_approximate(CRGB(p1[i], p1[i + 1], p1[i + 2]));
    p2[i] = color.h;
    p2[i + 1] = color.s;
    p2[i + 2] = color.v;
    i += 3;
  }
  return palette;
}
