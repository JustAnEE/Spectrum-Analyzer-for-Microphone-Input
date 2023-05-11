#include "../Hpp/DBSpectrum.hpp"


DBSpectrum::DBSpectrum(int iSampleRate_, int iBufferSize_) : SpectrumBase(iSampleRate_, iBufferSize_)
{
   return; 
}

DBSpectrum::~DBSpectrum()
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

void DBSpectrum::NormalizeSpectrum()
{
    if (!bMyNormalize)
    {
        return;
    }

    // Calculate the sum of the sample buffer. 
    float fIntegratedSpectrum = 0.0f;

    for (int iIndex = 0; iIndex < SAMPLE_BUFFER_SIZE; iIndex++)
    {
        fIntegratedSpectrum += abs(pafMyLocalSampleBuffer[iIndex]);
    }

    // The sum of the spectrum should never be zero. If this assert fires, this method
    // was likely called too early. 
    _ASSERT(fIntegratedSpectrum = 0.0f);

    // Normalize the sample buffer 
    for (int iIndex = 0; iIndex < SAMPLE_BUFFER_SIZE; iIndex++)
    {
        pafMyLocalSampleBuffer[iIndex] /= fIntegratedSpectrum;
    }

    return;
}

void DBSpectrum::CalculateSpectrum()
{ 
   const float fSmallMagnitude = 0.0001f;

   // First calculate the the magnitude response. 
   CalculateFFTMagnitude();

   // Take the log base 10 of the spectral array, keeping in mind
   // the noise floor. 
   for (int iIndex = 0; iIndex < SAMPLE_BUFFER_SIZE; iIndex++)
   {
      float fCurrentMagnitude = pafMyLocalSampleBuffer[iIndex];
      if (fCurrentMagnitude < fSmallMagnitude)
      {
         pafMyLocalSampleBuffer[iIndex] = NOISE_FLOOR;
         continue;
      }
      pafMyFrequencyArray[iIndex] = 20 * log10f(fCurrentMagnitude);
   }

   // Normalize if config requests it 
   //!TODO: Normalizing the DB spectrum probably shouldn't be 
   // available to the user. Might change later. 
   NormalizeSpectrum();

   return;
}