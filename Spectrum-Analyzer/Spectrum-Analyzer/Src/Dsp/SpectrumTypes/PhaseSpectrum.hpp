#ifndef PHASE_SPECTRUM_HPP
#define PHASE_SPECTRUM_HPP

#include "SpectrumBase.hpp"

class PhaseSpectrum : public SpectrumBase
{
   public:
      PhaseSpectrum(int iBufferSize_, int iSampleRate_);
      ~PhaseSpectrum() = default;

   public:
      void CalculateSpectrum() override; 
};

#endif 
