#include "PhaseSpectrum.hpp"
// #include <math.h> (Uncomment when implemented)

PhaseSpectrum::PhaseSpectrum(int iBufferSize_, int iSampleRate_) 
   : SpectrumBase(iBufferSize_, iSampleRate_)
{
   
   return; 
}

void PhaseSpectrum::CalculateSpectrum()
{
   //const float fPhaseThreshold = 0.0f;

   //!TODO: No idea what this code is even doing at the moment. 
   // 

   //   for (int k = 0; k < iMyBufferSize; k++) 
   //   {

   //      fPhaseThreshold = sqrt(fft_data[k][REAL] * fft_data[k][REAL] + fft_data[k][IMAG] * fft_data[k][IMAG]);

   //      if (fPhaseThreshold < sqrt(fft_data[k][REAL] * fft_data[k][REAL] + fft_data[k][IMAG] * fft_data[k][IMAG])) 
   //      {
   //         fPhaseThreshold = sqrt(fft_data[k][REAL] * fft_data[k][REAL] + fft_data[k][IMAG] * fft_data[k][IMAG]);
   //      }

   //   }

   //   for (int k = 0; k < iMyBufferSize; k++) {

   //      GLfloat fMagnitude = sqrt(fft_data[k][REAL] * fft_data[k][REAL] + fft_data[k][IMAG] * fft_data[k][IMAG]);
   //      if (fMagnitude < fPhaseThreshold) 
   //      {
   //         pafMySpectrumData[k] = 0;
   //      }

   //      else 
   //      {
   //         pafMySpectrumData[k] = (180.0f / 3.14159) * atan2f(fft_data[k][IMAG], fft_data[k][REAL]);
   //      }

   //   }
   return; 
}
