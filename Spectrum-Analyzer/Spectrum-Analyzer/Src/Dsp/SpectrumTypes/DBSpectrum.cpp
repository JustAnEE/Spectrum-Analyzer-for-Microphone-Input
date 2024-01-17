#include "DBSpectrum.hpp"
#include <math.h>


DBSpectrum::DBSpectrum(int iBufferSize_, int iSampleRate_) 
   : SpectrumBase(iBufferSize_, iSampleRate_)
{
   return; 
}

void DBSpectrum::CalculateSpectrum()
{
   // Need to deal with magnitude data close to/ equal to zero. 
   float fNoiseFloor = -200;

   for (int k = 0; k < iMyBufferSize; k++)
   {
      float fAmplitude = pafMyMagnitudeData[k];

      if (fAmplitude < 1.0f * pow(10, -6) || (fAmplitude == 0.0f)) 
      {
         pafMyMagnitudeData[k] = fNoiseFloor;
      }
      else 
      {
         pafMyMagnitudeData[k] = 10.0f * log10(1000.0f * fAmplitude);
      }
   }

   Interleave();
   return; 
}

