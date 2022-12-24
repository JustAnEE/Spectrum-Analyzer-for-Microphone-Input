#pragma once 

#include <fftw3.h>
#include <math.h> 
#include <iostream>
#include <glad/glad.h>
#include "SpectrumTypes.hpp"
#include "SpectrumInitPacket.hpp"
#include "SpectrumPacket.hpp"
#include "Filter.h"

#define REAL 1
#define IMAG 0

class SpectrumDSP
{

// Constructors and Destructors: 
public: 

   SpectrumDSP(int _iMySampleRate, int _iMyBufferSize);
   ~SpectrumDSP();

// Public Methods 
public: 

   void ProcessSpectrumInitPacket(SpectrumInitPacket* pclSpectrumInitPacket_, GLfloat* pafSampleBuffer_);
   void GetSpectrumOutput(SpectrumPacket& clSpectrumPacket_);

// Operations. 
private:
   void fft(fftwf_complex* input, fftwf_complex* output);
   void ifft(fftwf_complex* in, fftwf_complex* out);
   
   void ApplyWindow();
   fftwf_complex* set_fft(fftwf_complex* shifted_input);
   fftwf_complex* fft_shift();
   void MagnitudeSpectrum(fftwf_complex* fft_data);
   void DBSpectrum();
   void DBmSpectrum();
   void PSDSpectrum();
   void PrepDataForPlot();
   void PhaseSpectrum(fftwf_complex* fft_data);
   void DetrendBuffer();
   void CalculateSpectrum();
   void setFreqs();

   void PopulateSpectrumPacket();
   void GenWindow();

// Private members 
private:

   //! This is a pointer to a spectrum packet. 
   SpectrumPacket* pclMySpectrumPacket;

   //! This is a pointer to a Filter 
   Filter* pclMyFilter; 

   //! This is the sample rate of the system. 
   int iMySampleRate;

   //! This is the size of the incoming sample buffer. 
   int iMyBufferSize;
   
   //! This enum is the spectrum type: Magnitude Response, DB Spectrum, PSD, etc.
   SpectrumTypeEnum eMySpectrumType;

   //! This enum is the window type: Hamming, Blackman, Rectangular, Barlette. 
   WindowTypeEnum   eMyWindowType; 

   //! This enum is the filter type: LPF, HPF, BPF, BSF. 
   FilterTypeEnum   eMyFilterType; 

   //! This is a pointer to an array containing the frequencies corresponding to FFT bins (array index). 
   GLfloat* pafMyFrequencyArray;

   //! This is a pointer to an array containing the current window function. 
   GLfloat* pafMyWindow; 

   //! This is a pointer to the local copy of the sample buffer read in from the microphone.
   GLfloat* pafMyLocalSampleBuffer;

   //! This is an array which holds the spectrum data output (y axis). 
   GLfloat* pafMySpectrumData; 

   //! This is an array which holds the interleaved x and y axis for the spectrum plot. 
   GLfloat* pafMySpectrumPlotData; 

};
