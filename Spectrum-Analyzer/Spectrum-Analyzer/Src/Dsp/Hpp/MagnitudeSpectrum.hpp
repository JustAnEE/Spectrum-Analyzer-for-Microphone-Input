#ifndef MAGNITUDESPECTRUM_HPP
#define MAGNITUDESPECTRUM_HPP

#include "SpectrumBase.hpp"

class MagnitudeSpectrum : public SpectrumBase
{
public:
   MagnitudeSpectrum(int iSampleRate, int iBufferSize);
   ~MagnitudeSpectrum();
public:

   void ProcessSpectrumInitPacket(SpectrumInitPacket* pclSpectrumInitPacket_, GLfloat* pafSampleBuffer_);

private:
   void CalculateSpectrum(); 
   void PopulateSpectrumPacket();

};

#endif 