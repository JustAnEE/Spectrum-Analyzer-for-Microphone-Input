#include "../Hpp/PSDSpectrum.hpp"

PSDSpectrum::PSDSpectrum(int iSampleRate_, int iBufferSize_) : SpectrumBase(iSampleRate_, iBufferSize_)
{
   return;
}

PSDSpectrum::~PSDSpectrum()
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

void PSDSpectrum::CalculateSpectrum()
{
   // First calculate the magnitude spectrum 
   CalculateFFTMagnitude();

   // For PSD we square the samples and divide by the number of samples 
   for (int iIndex = 0; iIndex < SAMPLE_BUFFER_SIZE; iIndex++)
   {
       pafMyLocalSampleBuffer[iIndex] *= pafMyLocalSampleBuffer[iIndex];
       pafMyLocalSampleBuffer[iIndex] /= SAMPLE_BUFFER_SIZE;
   }

   // Normalize if requested 
   NormalizeSpectrum();

   return; 
}