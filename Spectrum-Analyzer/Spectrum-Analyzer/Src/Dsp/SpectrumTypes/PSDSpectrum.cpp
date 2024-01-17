#include "PSDSpectrum.hpp"


PSDSpectrum::PSDSpectrum(int iBufferSize_, int iSampleRate_) 
   : SpectrumBase(iBufferSize_, iSampleRate_)
{
   return; 
}

void PSDSpectrum::CalculateSpectrum()
{
   for (int k = 0; k < iMyBufferSize; k++) 
   {
      float fAmplitude = pafMyMagnitudeData[k];
      pafMyMagnitudeData[k] = fAmplitude * fAmplitude / static_cast<float>(iMyBufferSize);
   }
   
   Interleave();
   return; 
}
