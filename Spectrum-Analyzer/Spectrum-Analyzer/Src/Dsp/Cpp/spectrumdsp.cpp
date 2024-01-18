#include "../Hpp/spectrumdsp.hpp"
#include "../SpectrumTypes/SpectralFactory.hpp"

SpectrumDSP::SpectrumDSP(int _iMySampleRate, int _iMyBufferSize) 
{

    iMySampleRate = _iMySampleRate;
    iMyBufferSize = _iMyBufferSize;

    //!TODO: Decide once and for all, dynamic buffer sizes or nah
    pafMyFrequencyArray = new GLfloat[iMyBufferSize]; 
    pafMySpectrumData = new GLfloat[iMyBufferSize]; 
    pafMySpectrumPlotData = new GLfloat[2*iMyBufferSize];
    pafMyLocalSampleBuffer = new GLfloat[iMyBufferSize]; 
    pafMyWindow = new GLfloat[iMyBufferSize]; 
    pclMySpectrumPacket = new SpectrumPacket();
    pclMyFilter = new Filter(iMySampleRate, iMyBufferSize);
    
    //!TODO: Implement DBM and phase spectrum. Fix the SpectrumTypeEnum throughout the system 
    for (const SpectrumTypeEnum& eType : {MAGNITUDE_SPECTRUM, PSD_SPECTRUM, DB_SPECTRUM})
    {
       apclMySpectrumCalculators[static_cast<int>(eType)] = SpectralFactory::Instantiate(eType, iMyBufferSize, iMySampleRate); 
    }

    return;  
}

SpectrumDSP::~SpectrumDSP() 
{
    delete(pafMyFrequencyArray);
    delete(pclMySpectrumPacket);
    delete(pclMyFilter);
    delete[](pafMyLocalSampleBuffer); 
    delete[](pafMySpectrumData); 
    delete[](pafMySpectrumPlotData); 
    delete[](pafMyWindow); 
}

void SpectrumDSP::ProcessSpectrumInitPacket(SpectrumInitPacket* pclSpectrumInitPacket_, GLfloat* pafSampleBuffer_)
{

   // Grab information from the initialisation packet 
   eMySpectrumType = pclSpectrumInitPacket_->stMyDSPInitialisation.eSpectrumOutput;
   eMyWindowType   = pclSpectrumInitPacket_->stMyDSPInitialisation.eWindow;
   eMyFilterType   = pclSpectrumInitPacket_->stMyDSPInitialisation.eFilter; 
  // Custom buffer size / sample rate not implemented yet 
  // iMyBufferSize   = pclSpectrumInitPacket_->stMyDSPInitialisation.iBufferSize;
  // iMySampleRate   = pclSpectrumInitPacket_->stMyDSPInitialisation.iSampleRate;

   // Copy sample buffer into the class to avoid overwriting it in other functions
   memcpy(pafMyLocalSampleBuffer, pafSampleBuffer_, iMyBufferSize*sizeof(GLfloat));

   // Fill window
   GenWindow();

   // If the sample rate or buffer size is not default, need to generate new windows and frequency arrays
   // non-default configurations are not currently supported. 
   if (!pclSpectrumInitPacket_->bIsFsandBufferSizeDefault)
   {
       // Non-default settings in the works 
   }
   if (pclSpectrumInitPacket_->bDetrend)
   {
       //! TODO: Detrend adds a rediculous DC component when it should be removing it :/ 
       DetrendBuffer();
   }
   
   if (!(eMyFilterType == NO_FILTER))
   {
       // The Filter gets initialized via the SpectrumInitPacket. 
       pclMyFilter->InitializeFilter(*pclSpectrumInitPacket_);
       
       // Filter class will convolute filter indicated in the 
       // packet with the local sample buffer. 
       pclMyFilter->ApplyFilter(pafMyLocalSampleBuffer);
   }

   if (!(eMyWindowType == RECTANGULAR_WINDOW))
   {
       // Apply any windowing or filtering. 
       ApplyWindow();
   }

   // Fill the pafMySpectrumPlotDataArray with the interleaved plot data. 
   CalculateSpectrum();

   // Load the output spectrum packet.
   PopulateSpectrumPacket();

   return; 

}



void SpectrumDSP::PopulateSpectrumPacket()
{
    // Load output packet 
    pclMySpectrumPacket->eMySpectrumType = eMySpectrumType;
    memcpy(pclMySpectrumPacket->afMySpectrumArray, pafMySpectrumPlotData, 2*iMyBufferSize*sizeof(GLfloat));
}

void SpectrumDSP::GetSpectrumOutput(SpectrumPacket& clSpectrumPacket_)
{
    clSpectrumPacket_.eMySpectrumType = eMySpectrumType;
    memcpy(clSpectrumPacket_.afMySpectrumArray, pclMySpectrumPacket->afMySpectrumArray, sizeof(pclMySpectrumPacket->afMySpectrumArray)); 
}

void SpectrumDSP::GenWindow()
{
    if (eMyWindowType == RECTANGULAR_WINDOW)
    {
        return; 
    }

    switch (eMyWindowType)
    {

    case HAMMING_WINDOW:
        for (int k = 0; k < iMyBufferSize; k++)
        {
            pafMyWindow[k] = 0.54 - 0.46 * cosf(2.0 * 3.14159 * (GLfloat)k / ((GLfloat)(iMyBufferSize - 1)));
        }
        break;

    case BLACKMAN_WINDOW:
        for (int k = 0; k < iMyBufferSize; k++) 
        {
            pafMyWindow[k] = 0.42 - 0.5 * cosf(2.0 * 3.14159 * (GLfloat)k / ((GLfloat)(iMyBufferSize - 1))) + 0.08 * cos(4.0 * 3.14159 * (GLfloat)k / ((GLfloat)(iMyBufferSize - 1)));
        }
        break;

    case BARLETTE_WINDOW:
        for (int k = 0; k < iMyBufferSize; k++)
        {
            pafMyWindow[k] = (2.0 / ((GLfloat)(iMyBufferSize - 1))) * (((GLfloat)(iMyBufferSize - 1)) / 2.0 - (GLfloat)abs((GLfloat)k - (((GLfloat)iMyBufferSize - 1)) / 2.0));
        }
        break; 
    }
}


void SpectrumDSP::ApplyWindow() 
{
   if (eMyWindowType == RECTANGULAR_WINDOW)
   {
      // Do not need to do anything with a rectangular window, simply return. 
      return; 
   }
    
   switch (eMyWindowType)
   {
      case HAMMING_WINDOW:      
         for (int k = 0; k < iMyBufferSize; ++k) 
         {
            pafMyLocalSampleBuffer[k] *= pafMyWindow[k];
         }
         break;

      case BLACKMAN_WINDOW:
         for (int k = 0; k < iMyBufferSize; ++k) 
         { 
            pafMyLocalSampleBuffer[k] *= pafMyWindow[k]; 
         }

      case BARLETTE_WINDOW:
         for (int k = 0; k < iMyBufferSize; ++k) 
         { 
            pafMyLocalSampleBuffer[k] *= pafMyWindow[k]; 
         }

      default:
         // Invalid Window Type, silently fail and return
         return; 
   }

   return; 

}

void SpectrumDSP::ifft(fftwf_complex* in, fftwf_complex* out)
{
    fftwf_plan plan = fftwf_plan_dft_1d(iMyBufferSize, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftwf_execute(plan);
    fftwf_destroy_plan(plan);
    fftwf_cleanup();
    
    for (int k = 0; k <= iMyBufferSize; k++) {

        out[k][REAL] /= iMyBufferSize;
        out[k][IMAG] /= iMyBufferSize;

    }
}


void SpectrumDSP::DBmSpectrum()
{
   // Need to deal with magnitude data close to/ equal to zero. 
   GLfloat noise_floor = -200;

   for (int k = 0; k < iMyBufferSize; k++)
   {
      GLfloat fAmplitude = pafMySpectrumData[k];

      if (fAmplitude < 1 * pow(10, -6) || (fAmplitude == 0)) 
      {
         pafMySpectrumData[k] = noise_floor;
      }
      else 
      {
         pafMySpectrumData[k] = (GLfloat)10 * log10(1000 * fAmplitude);
      }
   }

   return;
}

void SpectrumDSP::PhaseSpectrum(fftwf_complex* fft_data)
{
   GLfloat fPhaseThreshold = 0;

   //!TODO: No idea what this code is even doing at the moment. 

   for (int k = 0; k < iMyBufferSize; k++) 
   {

      fPhaseThreshold = sqrt(fft_data[k][REAL] * fft_data[k][REAL] + fft_data[k][IMAG] * fft_data[k][IMAG]);

      if (fPhaseThreshold < sqrt(fft_data[k][REAL] * fft_data[k][REAL] + fft_data[k][IMAG] * fft_data[k][IMAG])) 
      {
         fPhaseThreshold = sqrt(fft_data[k][REAL] * fft_data[k][REAL] + fft_data[k][IMAG] * fft_data[k][IMAG]);
      }

   }

   for (int k = 0; k < iMyBufferSize; k++) {

      GLfloat fMagnitude = sqrt(fft_data[k][REAL] * fft_data[k][REAL] + fft_data[k][IMAG] * fft_data[k][IMAG]);
      if (fMagnitude < fPhaseThreshold) 
      {
         pafMySpectrumData[k] = 0;
      }

      else 
      {
         pafMySpectrumData[k] = (180.0f / 3.14159) * atan2f(fft_data[k][IMAG], fft_data[k][REAL]);
      }

   }

   return;
}

void SpectrumDSP::DetrendBuffer()
{
    GLfloat buffer_avg = 0;

    for (int k = 0; k < iMyBufferSize; k++) {

        buffer_avg += pafMyLocalSampleBuffer[k];

    }

    for (int k = 0; k < iMyBufferSize; k++) {

        pafMyLocalSampleBuffer[k] -= buffer_avg;

    }

}

void SpectrumDSP::CalculateSpectrum()
{
   apclMySpectrumCalculators[eMySpectrumType]->SetSampleData(pafMyLocalSampleBuffer);
   apclMySpectrumCalculators[eMySpectrumType]->FFT();
   apclMySpectrumCalculators[eMySpectrumType]->CalculateSpectrum();
   pafMySpectrumPlotData = apclMySpectrumCalculators[eMySpectrumType]->GetInterleavedData();

   return; 
}
