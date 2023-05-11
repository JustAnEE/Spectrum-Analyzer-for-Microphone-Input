#include "../Hpp/SpectrumBase.hpp"

SpectrumBase::SpectrumBase(int _iMySampleRate, int _iMyBufferSize)
{
   iMySampleRate = _iMySampleRate;
   iMyBufferSize = _iMyBufferSize;

   pafMyFrequencyArray    = new GLfloat[iMyBufferSize];
   pafMySpectrumData      = nullptr;
   pafMySpectrumPlotData  = nullptr;
   pafMyLocalSampleBuffer = nullptr;
   pclMySpectrumPacket    = new SpectrumPacket();
   pclMyFilter            = new Filter(iMySampleRate, iMyBufferSize);
   
   // Initialize windows 
   apclMyWindows[HAMMING_WINDOW]  = new HammingWindow();
   apclMyWindows[BLACKMAN_WINDOW] = new BlackmanWindow();
   apclMyWindows[BARLETTE_WINDOW] = new BarletteWindow();

   // Generate frequency array 
   GenerateFrequency();
}

SpectrumPacket* SpectrumBase::GetSpectrumOutput()
{
   return pclMySpectrumPacket;
}


void SpectrumBase::ProcessSpectrumInitPacket(SpectrumInitPacket* pclSpectrumInitPacket_, GLfloat* pafSampleBuffer_)
{

    // Grab information from the initialisation packet 
    eMySpectrumType = pclSpectrumInitPacket_->stMyDSPInitialisation.eSpectrumOutput;
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

    // Apply any windowing selected. 
    ApplyWindow();

    // Calculate the Spectrum 
    CalculateSpectrum();

    // Load the output spectrum packet.
    PopulateSpectrumPacket();
}

void SpectrumBase::GenerateFrequency()
{
   GLfloat fSampleRate = (GLfloat)iMySampleRate;
   GLfloat fNumSamples = (GLfloat)iMyBufferSize;
   for (int k = 0; k < iMyBufferSize; k++) { pafMyFrequencyArray[k] = -fSampleRate / 2 + ((GLfloat)k) * fSampleRate / fNumSamples; }
}

void SpectrumBase::CalculateFFTMagnitude()
{
    // Calculate the FFT data from the local sample buffer
    FFTData* ptrFFTData = FFT();

    for (int k = 0; k < iMyBufferSize; k++)
    {
        GLfloat fSquareData = ptrFFTData[k][REAL] * ptrFFTData[k][REAL] + ptrFFTData[k][IMAG] * ptrFFTData[k][IMAG];
        pafMySpectrumData[k] = sqrt(fSquareData);
    }
}

FFTData* SpectrumBase::FFT()
{
   FFTData* FFTInputData  = (FFTData*)fftwf_malloc(sizeof(FFTData) * (iMyBufferSize));
   FFTData* FFTOutputData = (FFTData*)fftwf_malloc(sizeof(FFTData) * (iMyBufferSize));

   // Scale input sample buffer data so that output FFT bins are shifted. 
   for (int k = 0; k < iMyBufferSize; k++) {
      FFTInputData[k][REAL] = pafMyLocalSampleBuffer[k] * pow(-1, k);
      FFTInputData[k][IMAG] = 0;
   }

   // Calculate the FFT
   fftwf_plan plan = fftwf_plan_dft_1d(iMyBufferSize, FFTInputData, FFTOutputData, FFTW_FORWARD, FFTW_ESTIMATE);

   fftwf_execute(plan);
   fftwf_destroy_plan(plan);
   fftwf_cleanup();

   return FFTOutputData;
}

void SpectrumBase::PrepDataForPlot()
{
   int iPlotDataSize = 2 * iMyBufferSize;

   for (int k = 0; k < iPlotDataSize; k += 2) {

      int index = k / 2;

      pafMySpectrumPlotData[k] = pafMyFrequencyArray[index];
      pafMySpectrumPlotData[k + 1] = pafMySpectrumData[index];

   }
   return;
}

void SpectrumBase::ApplyWindow()
{
   if (eMyWindowType != RECTANGULAR_WINDOW)
   {
      for (int iIndex = 0; iIndex < SAMPLE_BUFFER_SIZE; iIndex++)
      {
         pafMyLocalSampleBuffer[iIndex] *= (apclMyWindows[eMyWindowType]->GetWindow())[iIndex];
      }
   }
   // If rectangular, leave the sample buffer alone as applying the window 
   // is the same as multiplying every sample by one. 
   return;
}

void SpectrumBase::NormalizeSpectrum()
{
   if (!bMyNormalize)
   {
      return;
   }

   // Calculate the sum of the sample buffer. 
   float fIntegratedSpectrum = 0.0f;

   for (int iIndex = 0; iIndex < SAMPLE_BUFFER_SIZE; iIndex++)
   {
       fIntegratedSpectrum += pafMyLocalSampleBuffer[iIndex];
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

SpectrumDSPConfig* SpectrumBase::GetpclSpectrumConfig()
{
   return &clMySpectrumDSPConfig;
}