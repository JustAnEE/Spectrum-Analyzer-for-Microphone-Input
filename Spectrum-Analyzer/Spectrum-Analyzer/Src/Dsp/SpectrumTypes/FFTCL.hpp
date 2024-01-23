#ifndef FFTCL_HPP
#define FFTCL_HPP

#include <fftw3.h>

class FFTCL 
{
   // Public constructors / destructors 
   public:
      FFTCL(int iBufferSize_, int iSampleRate_);
      ~FFTCL(); 

   // Public methods 
   public:
      void SetSampleData(float* pafSampleData_);
      void FFT(); 
      void IFFT(); 
      float* GetMagnitudeData();  
      float* GetFrequencyVector() const; 

   // Private methods 
   private:
      void CreateAndRunFFTPlan(int iFFTPlanType_);
      void FFTShift();
      void CreateMagnitudeData();
      void FillFrequencyArray();
      void Normalize();

   // Private fields 
   private:
      float* pafMySampleData;
      float* pafMyFrequencyArray;
      int iMySampleDataSize;
      int iMySampleRate;
      fftwf_complex* pFFTInput;
      fftwf_complex* pFFTOutput; 
};

#endif // FFTCL_HPP
