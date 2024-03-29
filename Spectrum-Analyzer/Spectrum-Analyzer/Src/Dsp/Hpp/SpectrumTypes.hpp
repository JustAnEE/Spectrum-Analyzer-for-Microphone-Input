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
   NUM_SPECTRUM       = 6
}SpectrumTypeEnum;


typedef enum WindowTypeEnum
{
   RECTANGULAR_WINDOW = 1,
   HAMMING_WINDOW     = 2,
   BLACKMAN_WINDOW    = 3,
   BARLETTE_WINDOW    = 4,
   NUM_WINDOWS = 5
}WindowTypeEnum;

typedef enum FilterTypeEnum
{
   LOWPASS   = 1,
   HIGHPASS  = 2,
   BANDPASS  = 3,
   BANDSTOP  = 4,
   NO_FILTER = 5, 
}FilterTypeEnum;
