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

void SpectrumBase::Reset()
{
    eMySpectrumType = NO_SPECTRUM;
    eMyWindowType = NO_WINDOW;
    eMyFilterType = NO_FILTER;

}

GLfloat* SpectrumBase::FormatTimeSeries(GLfloat* pafMicData_)
{
   GLfloat* pafReformattedTimeData = (GLfloat*)calloc(NUM_SAMPLES * 2, sizeof(GLfloat));
   for (int i = 0; i < NUM_SAMPLES; i++) {
       pafReformattedTimeData[2 * i] = ((GLfloat)i) / ((GLfloat)NUM_SAMPLES);	// -- x coord
       pafReformattedTimeData[2 * i + 1] = pafMicData_[i];						// -- y cord
   }
   return pafReformattedTimeData;
}

void SpectrumBase::CalculateSpectralData()
{
    // This function should never be called if the DSP is not initialized
    if (!clMySpectrumDSPConfig.bMyDSPInitialized)
    {
        
    }
    eMySpectrumType = clMySpectrumDSPConfig.stMyDSPInitData.eSpectrumOutput;
    eMyWindowType = clMySpectrumDSPConfig.stMyDSPInitData.eWindow;
    eMyFilterType = clMySpectrumDSPConfig.stMyDSPInitData.eFilter;

     // Copy sample buffer into the class to avoid overwriting it in other functions
    pafMyLocalSampleBuffer = new GLfloat[iMyBufferSize];
    memcpy(pafMyLocalSampleBuffer, clMySpectrumDSPConfig.afMySampleBufferData, iMyBufferSize * sizeof(GLfloat));

    // Initialize local arrays. 
    pafMySpectrumData = new GLfloat[iMyBufferSize];
    pafMySpectrumPlotData = new GLfloat[2 * iMyBufferSize];

    if (clMySpectrumDSPConfig.stMyDSPInitData.bDetrend);
    {
        //! TODO: Detrend adds a rediculous DC component when it should be removing it :/ 
       // DetrendBuffer();
    }

    // Apply any windowing selected. 
    ApplyWindow();

    // Calculate the Spectrum 
    CalculateSpectrum();

    // Normalize spectrum if required. 
    NormalizeSpectrum();

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
   _ASSERTE(fIntegratedSpectrum = 0.0f);

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
