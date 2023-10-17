#pragma once

// Spectrum type enum used for specifying spectrum output.
typedef enum SpectrumTypeEnum
{  
   TIME_SERIES        = 0,
   DB_SPECTRUM        = 1,
   MAGNITUDE_SPECTRUM = 2,
   PSD_SPECTRUM       = 3,
   DBM_SPECTRUM       = 4,
   PHASE_SPECTRUM     = 5,
}SpectrumTypeEnum;


// !TODO: WindowTypeEnum is tightly coupled to the 
// plotting, this results in boof. Fix :) 
typedef enum WindowTypeEnum
{
   NULL_WINDOW = 0, 
   RECTANGULAR_WINDOW = 1,
   HAMMING_WINDOW     = 2,
   BLACKMAN_WINDOW    = 3,
   BARLETTE_WINDOW    = 4,
   NUM_WINDOWS   = 3, // Only Hamming, Blackman and Barlette do anything 
   WINDOW_OFFSET = 2  // Subtrack WindowTypeEnum - WINDOW_OFFSET to get array index
                      // !TODO: very boof, has to do with coupling mentioned above. 
}WindowTypeEnum;

typedef enum FilterTypeEnum
{
   LOWPASS   = 1,
   HIGHPASS  = 2,
   BANDPASS  = 3,
   BANDSTOP  = 4,
   NO_FILTER = 5, 
}FilterTypeEnum;