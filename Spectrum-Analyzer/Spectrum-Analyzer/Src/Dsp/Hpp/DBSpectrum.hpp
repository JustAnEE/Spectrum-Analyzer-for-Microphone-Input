#ifndef DBSPECTRUM_HPP
#define DBSPECTRUM_HPP

#include "SpectrumBase.hpp"

#define NOISE_FLOOR ((float) (-120))

class DBSpectrum : public SpectrumBase
{
public:
    DBSpectrum(int iSampleRate_, int iBufferSize_);
    ~DBSpectrum(); 
private:
   
    // Calculate the spectrum, see specific spectrum type for implementation
    void CalculateSpectrum();

    // DB Spectrum needs to implement its own normalize method. 
    void NormalizeSpectrum();

    // Populates the outgoing packet with the spectrum data ready for plot. 
    void PopulateSpectrumPacket();

private:

};

#endif 
