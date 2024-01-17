#include "MagnitudeSpectrum.hpp" 

MagnitudeSpectrum::MagnitudeSpectrum(int iBufferSize_, int iSampleRate_) 
   : SpectrumBase(iBufferSize_, iSampleRate_)
{

   return; 
}


void MagnitudeSpectrum::CalculateSpectrum()
{
   
   // We can just interleave the data,
   // the magnitude spectrum is always calculated
   // by the FFTObj in SpectrumBase 
   Interleave();
   return; 
}

