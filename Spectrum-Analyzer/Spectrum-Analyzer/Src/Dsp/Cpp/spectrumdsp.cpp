#include "../Hpp/spectrumdsp.hpp"
#include "../Hpp/window.hpp"


SpectrumDSP::SpectrumDSP(int _iMySampleRate, int _iMyBufferSize) 
{

    iMySampleRate = _iMySampleRate;
    iMyBufferSize = _iMyBufferSize;

    pafMyFrequencyArray = new GLfloat[iMyBufferSize]; 
    pafMySpectrumData = nullptr; 
    pafMySpectrumPlotData = nullptr;
    pafMyLocalSampleBuffer = nullptr; 
    pclMySpectrumPacket = new SpectrumPacket();
    pclMyFilter = new Filter(iMySampleRate, iMyBufferSize);

    setFreqs();

    // Instantiate windowers 
    for(const WindowTypeEnum& WindowType : {HAMMING_WINDOW, BLACKMAN_WINDOW, BARLETTE_WINDOW})
    {
       apclMyWindows[WindowType - WINDOW_OFFSET] = WindowInstantiator::InstantiateWindow(WindowType);
       apclMyWindows[WindowType - WINDOW_OFFSET]->CreateEmptyWindow(_iMyBufferSize);
       apclMyWindows[WindowType - WINDOW_OFFSET]->InitWindow();
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

    for (unsigned int ulIdx = 0; ulIdx < NUM_WINDOWS; ulIdx++)
    {
       delete apclMyWindows[ulIdx];
    }

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
   pafMyLocalSampleBuffer = new GLfloat[iMyBufferSize];
   memcpy(pafMyLocalSampleBuffer, pafSampleBuffer_, iMyBufferSize*sizeof(GLfloat));

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

   if (!(eMyWindowType == RECTANGULAR_WINDOW) && apclMyWindows[eMyWindowType - WINDOW_OFFSET]->IsWindowReady())
   {
       // Apply the window type onto the sample buffer 
       apclMyWindows[eMyWindowType - WINDOW_OFFSET]->ApplyWindow(pafMyLocalSampleBuffer);
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


void SpectrumDSP::setFreqs() {
    GLfloat fs = (GLfloat)iMySampleRate;
    GLfloat N = (GLfloat)iMyBufferSize;
    for (int k = 0; k < iMyBufferSize; k++) { pafMyFrequencyArray[k] = -fs / 2 + ((GLfloat)k) * fs / N; }
}

void SpectrumDSP::fft(fftwf_complex* input, fftwf_complex* output)
{

    fftwf_plan plan = fftwf_plan_dft_1d(iMyBufferSize, input, output, FFTW_FORWARD, FFTW_ESTIMATE);

    fftwf_execute(plan);
    fftwf_destroy_plan(plan);
    fftwf_cleanup();

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

fftwf_complex* SpectrumDSP::set_fft(fftwf_complex* shifted_input)
{
    fftwf_complex* y = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (iMyBufferSize));

    fft(shifted_input, y);

    return y;
}

fftwf_complex* SpectrumDSP::fft_shift() 
{
    fftwf_complex* x = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (iMyBufferSize));

        for (int k = 0; k < iMyBufferSize; k++) {
           x[k][REAL] = pafMyLocalSampleBuffer[k] * pow(-1, k);
           x[k][IMAG] = 0;
        }

    return x;
}

void SpectrumDSP::MagnitudeSpectrum(fftwf_complex* fft_data)
{

    for (int k = 0; k < iMyBufferSize; k++) 
    {
        GLfloat square = fft_data[k][REAL] * fft_data[k][REAL] + fft_data[k][IMAG] * fft_data[k][IMAG];
        pafMySpectrumData[k] = sqrt(square);
    }

    return; 
}

void SpectrumDSP::DBSpectrum()
{
   // Need to deal with magnitude data close to zero. Noise floor of -120 dB corresponds to a magnitude of 1e-6. 
   GLfloat noise_floor = -120.0f;

   for (int k = 0; k < iMyBufferSize; k++) 
   {
      GLfloat fAmplitude = pafMySpectrumData[k]; 
      if (fAmplitude < 1 * pow(10, -6) || (fAmplitude == 0)) 
      {
         pafMySpectrumData[k] = noise_floor;
      }
      else 
      {
          pafMySpectrumData[k] = (GLfloat)20 * log10(fAmplitude);
      }
   }

    return;
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

void SpectrumDSP::PSDSpectrum()
{
   for (int k = 0; k < iMyBufferSize; k++) 
   {
      GLfloat fAmplitude = pafMySpectrumData[k];
      pafMySpectrumData[k] = fAmplitude * fAmplitude / iMyBufferSize;
   }
   return;
}

void SpectrumDSP::PrepDataForPlot()
{
    int iPlotDataSize = 2 * iMyBufferSize;

    for (int k = 0; k < iPlotDataSize; k += 2) {

        int index = k / 2;

        pafMySpectrumPlotData[k] = pafMyFrequencyArray[index];
        pafMySpectrumPlotData[k + 1] = pafMySpectrumData[index];

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
 
   fftwf_complex* fftin_ptr = fft_shift();
   fftwf_complex* fft_ptr = set_fft(fftin_ptr);
   
   // Magnitude spectrum is always calculated, since the other spectra are obtained via manipulating
   // the magnitude spectrum. 
   MagnitudeSpectrum(fft_ptr);

   switch (eMySpectrumType)
   {
      case MAGNITUDE_SPECTRUM: 
         // Magnitude data already exists, interleave with frequency array immediately. 
         PrepDataForPlot();
         break;

      case DB_SPECTRUM:   
         DBSpectrum(); 
         PrepDataForPlot();
         break;

      case DBM_SPECTRUM:       
         DBmSpectrum(); 
         PrepDataForPlot();
         break;

      case PSD_SPECTRUM:  
         PSDSpectrum();
         PrepDataForPlot();
         break;

      case PHASE_SPECTRUM:  
         PhaseSpectrum(fft_ptr); 
         PrepDataForPlot();
         break;

      default:
         // If invalid spectrum enum, simply return the interleaved magnitude data. 
         PrepDataForPlot(); 
         break; 
   }

   fftwf_free(fftin_ptr);
   fftwf_free(fft_ptr);

   return; 
}