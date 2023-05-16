#ifndef MAGNITUDESPECTRUM_HPP
#define MAGNITUDESPECTRUM_HPP

#include "SpectrumBase.hpp"

class MagnitudeSpectrum : public SpectrumBase
{
public:
   MagnitudeSpectrum(int iSampleRate, int iBufferSize);
   ~MagnitudeSpectrum();
public:

private:
   void CalculateSpectrum(); 
   void PopulateSpectrumPacket();

};

#endif 