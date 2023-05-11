#ifndef ENUMS_H
#define ENUMS_H

// Spectrum type enum used for specifying spectrum output.
typedef enum SpectrumTypeEnum
{
   MAGNITUDE_SPECTRUM = 0,
   DB_SPECTRUM        = 1,
   PSD_SPECTRUM       = 2,
   DBM_SPECTRUM       = 3,
   TIME_SERIES        = 4,
   PHASE_SPECTRUM     = 5,
   NO_SPECTRUM        = 6,
   COUNT              = 3 //!TODO: Only Mag/DB/PSD implemented, so count = 3. 
}SpectrumTypeEnum;


typedef enum WindowTypeEnum
{
   HAMMING_WINDOW     = 0,
   BLACKMAN_WINDOW    = 1,
   BARLETTE_WINDOW    = 2,
   RECTANGULAR_WINDOW = 3, 
   NO_WINDOW = 5,
   // Number of actionable window types 
   COUNT     = 3,
}WindowTypeEnum;

typedef enum FilterTypeEnum
{
   LOWPASS = 1,
   HIGHPASS = 2,
   BANDPASS = 3,
   BANDSTOP = 4,
   NO_FILTER = 5
}FilterTypeEnum;

#endif 