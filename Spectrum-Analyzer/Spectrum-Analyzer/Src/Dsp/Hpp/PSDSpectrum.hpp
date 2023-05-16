#ifndef PSDSPECTRUM_HPP
#define PSDSPECTRUM_HPP

#include "SpectrumBase.hpp"

class PSDSpectrum : public SpectrumBase
{

public:
   PSDSpectrum(int iSampleRate_, int iBufferSize_);
   ~PSDSpectrum();

private:
   void CalculateSpectrum();
   void PopulateSpectrumPacket();
};

#endif 
