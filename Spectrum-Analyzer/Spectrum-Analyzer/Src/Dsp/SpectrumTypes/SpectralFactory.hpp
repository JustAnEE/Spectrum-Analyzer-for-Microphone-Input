#ifndef SPECTRAL_FACTORY_HPP
#define SPECTRAL_FACTORY_HPP

#include "SpectrumBase.hpp"
#include "../Hpp/SpectrumTypes.hpp"

class SpectralFactory 
{
   SpectralFactory() = delete;
   ~SpectralFactory() = delete; 

   public:
      static SpectrumBase* Instantiate(SpectrumTypeEnum eSpectrumType_, int iBufferSize_, int iSampleRate_);

};


#endif 

