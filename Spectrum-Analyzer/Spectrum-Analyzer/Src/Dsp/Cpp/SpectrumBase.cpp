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

void SpectrumBase::GenerateFrequency()
{
   GLfloat fSampleRate = (GLfloat)iMySampleRate;
   GLfloat fNumSamples = (GLfloat)iMyBufferSize;
   for (int k = 0; k < iMyBufferSize; k++) { pafMyFrequencyArray[k] = -fSampleRate / 2 + ((GLfloat)k) * fSampleRate / fNumSamples; }
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

void SpectrumBase::ApplyWindow(WindowTypeEnum eWindowType)
{
   if (eWindowType != RECTANGULAR_WINDOW)
   {
      for (int iIndex = 0; iIndex < SAMPLE_BUFFER_SIZE; iIndex++)
      {
         pafMyLocalSampleBuffer[iIndex] *= (apclMyWindows[eWindowType]->GetWindow())[iIndex];
      }
   }
   // If rectangular, leave the sample buffer alone as applying the window 
   // is the same as multiplying every sample by one. 
   return;
}