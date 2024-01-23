#include "FFTCL.hpp"
#include <fftw3.h>
#include <math.h>

#define REAL ((int) 1)
#define IMAG ((int) 0)

FFTCL::FFTCL(int iBufferSize_, int iSampleRate_)
{
   iMySampleDataSize = iBufferSize_;
   iMySampleRate = iSampleRate_;

   pFFTInput = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (iMySampleDataSize));
   pFFTOutput = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (iMySampleDataSize));

   pafMyFrequencyArray = new float[iMySampleDataSize];
   FillFrequencyArray(); 
   
   return; 
}

FFTCL::~FFTCL()
{
   fftwf_free(pFFTInput);
   fftwf_free(pFFTOutput);
   return; 
}

void FFTCL::FillFrequencyArray()
{
    float fs = static_cast<float>(iMySampleRate);
    float N = static_cast<float>(iMySampleDataSize);

    for (int k = 0; k < iMySampleDataSize; k++) 
    { 
        pafMyFrequencyArray[k] = -fs / 2 + (static_cast<float>(k)) * fs / N; 
    }
   
   return; 
}

void FFTCL::SetSampleData(float* pafSampleData_)
{
   pafMySampleData = pafSampleData_;
   return; 
}


void FFTCL::FFTShift()
{
   for (int k = 0; k < iMySampleDataSize; k++) 
   {
      pFFTInput[k][REAL] = pafMySampleData[k] * pow(-1, k);
      pFFTInput[k][IMAG] = 0;
   }
   return; 
}

void FFTCL::CreateAndRunFFTPlan(int iFFTPlanType_)
{
   fftwf_plan plan = fftwf_plan_dft_1d(iMySampleDataSize, pFFTInput, pFFTOutput, iFFTPlanType_, FFTW_ESTIMATE);

   fftwf_execute(plan);
   fftwf_destroy_plan(plan);
   fftwf_cleanup();

   return; 
}

void FFTCL::Normalize()
{
   for (int k = 0; k < iMySampleDataSize; k++) 
   {

      pFFTOutput[k][REAL] /= iMySampleDataSize;
      pFFTOutput[k][IMAG] /= iMySampleDataSize;

   }
   return; 
}

//! TODO: This does not work. In fact It never worked. 
// There's no callers for now until filtering is 
// revived. It's moved here to get it out of spectrumdsp.
// Do not call, will IFFT random memory most likely :) 
void FFTCL::IFFT()
{  
   FFTShift();
   CreateAndRunFFTPlan(FFTW_BACKWARD);
   Normalize();
   return; 
}

void FFTCL::CreateMagnitudeData()
{
   for (int k = 0; k < iMySampleDataSize; k++) 
   {
      float fSquare = pFFTOutput[k][REAL] * pFFTOutput[k][REAL] + pFFTOutput[k][IMAG] * pFFTOutput[k][IMAG];
      // This is going to overwrite the data in place at the scope of the SpectrumDSP class LOL 
      // !TODO: Come up with a better data passing system 
      pafMySampleData[k] = sqrt(fSquare);
   }
   return; 
}


void FFTCL::FFT()
{
   FFTShift(); 
   CreateAndRunFFTPlan(FFTW_FORWARD);
   CreateMagnitudeData();

   return; 
}

float* FFTCL::GetMagnitudeData()
{
   return pafMySampleData;
}

float* FFTCL::GetFrequencyVector() const 
{
   return pafMyFrequencyArray;
}

