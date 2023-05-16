#include "../Hpp/MagnitudeSpectrum.hpp"

MagnitudeSpectrum::MagnitudeSpectrum(int iSampleRate, int iBufferSize) : SpectrumBase(iSampleRate, iBufferSize)
{
   // Base constructor inherited.
   return; 
}

MagnitudeSpectrum::~MagnitudeSpectrum()
{
   delete(pafMyFrequencyArray);
   delete(pclMySpectrumPacket);
   delete(pclMyFilter);
   delete[](pafMyLocalSampleBuffer);
   delete[](pafMySpectrumData);
   delete[](pafMySpectrumPlotData);
   delete[](apclMyWindows[HAMMING_WINDOW]);
   delete[](apclMyWindows[BLACKMAN_WINDOW]);
   delete[](apclMyWindows[BARLETTE_WINDOW]);
}

void MagnitudeSpectrum::CalculateSpectrum()
{
   // Calculate the spectral magnitude. 
   CalculateFFTMagnitude();
   
   // Normalize if configured to do so
   NormalizeSpectrum();

   return; 
}

void MagnitudeSpectrum::PopulateSpectrumPacket()
{
   // Load output packet 
   pclMySpectrumPacket->eMySpectrumType = SpectrumTypeEnum::MAGNITUDE_SPECTRUM;
   memcpy(pclMySpectrumPacket->afMySpectrumArray, pafMySpectrumPlotData, 2 * iMyBufferSize * sizeof(GLfloat));
}