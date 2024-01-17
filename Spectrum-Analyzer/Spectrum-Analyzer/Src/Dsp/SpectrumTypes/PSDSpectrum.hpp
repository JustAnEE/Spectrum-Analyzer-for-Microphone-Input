#ifndef PSD_SPECTRUM_HPP
#define PSD_SPECTRUM_HPP

#include "SpectrumBase.hpp"

class PSDSpectrum : public SpectrumBase 
{
   public:
      PSDSpectrum(int iBufferSize_, int iSampleRate_);
      ~PSDSpectrum() = default;

   public:
      void CalculateSpectrum(); 

};

#endif 
