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
   // Calculate the FFT data from the local sample buffer
   FFTData* ptrFFTData = FFT();

   for (int k = 0; k < iMyBufferSize; k++)
   {
      GLfloat fSquareData = ptrFFTData[k][REAL] * ptrFFTData[k][REAL] + ptrFFTData[k][IMAG] * ptrFFTData[k][IMAG];
      pafMySpectrumData[k] = sqrt(fSquareData);
   }
   return; 
}

void MagnitudeSpectrum::ProcessSpectrumInitPacket(SpectrumInitPacket* pclSpectrumInitPacket_,
   GLfloat* pafSampleBuffer_)
{
   // Grab information from the initialisation packet 
   eMyWindowType = pclSpectrumInitPacket_->stMyDSPInitialisation.eWindow;
   eMyFilterType = pclSpectrumInitPacket_->stMyDSPInitialisation.eFilter;
   // Custom buffer size / sample rate not implemented yet 
   // iMyBufferSize   = pclSpectrumInitPacket_->stMyDSPInitialisation.iBufferSize;
   // iMySampleRate   = pclSpectrumInitPacket_->stMyDSPInitialisation.iSampleRate;

    // Copy sample buffer into the class to avoid overwriting it in other functions
   pafMyLocalSampleBuffer = new GLfloat[iMyBufferSize];
   memcpy(pafMyLocalSampleBuffer, pafSampleBuffer_, iMyBufferSize * sizeof(GLfloat));

   // Initialize local arrays. 
   pafMySpectrumData = new GLfloat[iMyBufferSize];
   pafMySpectrumPlotData = new GLfloat[2 * iMyBufferSize];

   // If the sample rate or buffer size is not default, need to generate new windows and frequency arrays
   // non-default configurations are not currently supported. 
   if (!pclSpectrumInitPacket_->bIsFsandBufferSizeDefault)
   {
      // Non-default settings in the works 
   }
   if (pclSpectrumInitPacket_->bDetrend)
   {
      //! TODO: Detrend adds a rediculous DC component when it should be removing it :/ 
     // DetrendBuffer();
   }

   if (!(eMyFilterType == NO_FILTER))
   {
      // The Filter gets initialized via the SpectrumInitPacket. 
      pclMyFilter->InitializeFilter(*pclSpectrumInitPacket_);

      // Filter class will convolute filter indicated in the 
      // packet with the local sample buffer. 
      pclMyFilter->ApplyFilter(pafMyLocalSampleBuffer);
   }

   ApplyWindow(eMyWindowType);

   // Fill the pafMySpectrumPlotDataArray with the interleaved plot data. 
   CalculateSpectrum();

   // Load the output spectrum packet.
   PopulateSpectrumPacket();
}

void MagnitudeSpectrum::PopulateSpectrumPacket()
{
   // Load output packet 
   pclMySpectrumPacket->eMySpectrumType = SpectrumTypeEnum::MAGNITUDE_SPECTRUM;
   memcpy(pclMySpectrumPacket->afMySpectrumArray, pafMySpectrumPlotData, 2 * iMyBufferSize * sizeof(GLfloat));
}