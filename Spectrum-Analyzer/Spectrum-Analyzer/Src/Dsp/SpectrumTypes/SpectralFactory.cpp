#include "SpectralFactory.hpp"
#include "MagnitudeSpectrum.hpp"
#include "PSDSpectrum.hpp"
#include "DBSpectrum.hpp"
#include "DBmSpectrum.hpp"
#include "PhaseSpectrum.hpp"

SpectrumBase* SpectralFactory::Instantiate(SpectrumTypeEnum eSpectrumType_, int iBufferSize_, int iSampleRate_)
{
   switch(eSpectrumType_)
   {
      case MAGNITUDE_SPECTRUM:
         return new MagnitudeSpectrum(iBufferSize_, iSampleRate_);

      case PSD_SPECTRUM:
         return new PSDSpectrum(iBufferSize_, iSampleRate_);

      case DB_SPECTRUM:
         return new DBSpectrum(iBufferSize_, iSampleRate_);

      case DBM_SPECTRUM:
         return new DBmSpectrum(iBufferSize_, iSampleRate_);
      
      case PHASE_SPECTRUM:
         return new PhaseSpectrum(iBufferSize_, iSampleRate_);

      default:
         break; 

   }
   
   // Soft fail, return a magnitude spectrum calculator.
   return new MagnitudeSpectrum(iBufferSize_, iSampleRate_);
}
