#include "DBmSpectrum.hpp"
#include "SpectrumBase.hpp"
#include <math.h>

DBmSpectrum::DBmSpectrum(int iBufferSize_, int iSampleRate_)
   : SpectrumBase(iBufferSize_, iSampleRate_)
{
   return; 
}

void DBmSpectrum::CalculateSpectrum()
{
   // Need to deal with magnitude data close to/ equal to zero. 
   const float fNoiseFloor = -170.0f;

   //!TODO: If DSP ever becomes a performance bottleneck, we should implement 
   // fast logs, this would also get rid of the branch in the loop. 
   for (int k = 0; k < iMyBufferSize; k++)
   {
      float fAmplitude = pafMyMagnitudeData[k];

      if (fAmplitude < 1.0f * pow(10.0f, -6.0f) || (fAmplitude == 0.0f)) 
      {
         pafMyMagnitudeData[k] = fNoiseFloor;
      }
      else 
      {
         pafMyMagnitudeData[k] = 10.0f * log10(1000.0f * fAmplitude);
      }
   }
   return; 
}
