#ifndef DB_SPECTRUM_HPP
#define DB_SPECTRUM_HPP

#include "SpectrumBase.hpp"

class DBSpectrum : public SpectrumBase
{
   public:
      DBSpectrum(int iBufferSize_, int iSampleRate_);
      ~DBSpectrum() = default; 

   public:
      void CalculateSpectrum();

};

#endif 
