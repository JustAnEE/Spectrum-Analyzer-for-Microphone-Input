#ifndef DBM_SPECTRUM_HPP
#define DBM_SPECTRUM_HPP


#include "SpectrumBase.hpp"

class DBmSpectrum : public SpectrumBase
{
   public:
      DBmSpectrum(int iBufferSize_, int iSampleRate_);
      ~DBmSpectrum() = default; 

   public:
      void CalculateSpectrum() override;
};

#endif 
