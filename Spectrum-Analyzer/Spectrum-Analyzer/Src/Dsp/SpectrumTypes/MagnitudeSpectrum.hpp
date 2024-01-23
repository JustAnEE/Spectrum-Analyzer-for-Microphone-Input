#ifndef MAGNITUDE_SPECTRUM_HPP
#define MAGNITUDE_SPECTRUM_HPP


#include "SpectrumBase.hpp"


class MagnitudeSpectrum : public SpectrumBase
{
   public:
      MagnitudeSpectrum(int iBufferSize_, int iSampleRate_);
      ~MagnitudeSpectrum() = default; 

   public:
      void CalculateSpectrum() override;

};


#endif 
