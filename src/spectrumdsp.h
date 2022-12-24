#pragma once 

#include <fftw3.h>
#include <math.h> 
#include <iostream>
#include <glad/glad.h>
#include "SpectrumTypes.hpp"
#include "SpectrumInitPacket.hpp"
#include "SpectrumPacket.hpp"

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
   void ApplyFilter();
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
   // New stuff
   void GenWindow();

// Private members 
private:

   int iMySampleRate, iMyBufferSize;

   GLfloat* pafMyFrequencyArray;
   
   // New stuff

   SpectrumPacket* pclMySpectrumPacket;

   SpectrumTypeEnum eMySpectrumType;
   WindowTypeEnum   eMyWindowType; 
   FilterTypeEnum   eMyFilterType; 

   GLfloat* pafMyWindow; 
   //! This is the local copy of the sample buffer read in from the microphone.
   GLfloat* pafMyLocalSampleBuffer;
   //! This is an array which holds the spectrum data output (y axis). 
   GLfloat* pafMySpectrumData; 
   //! This is an array which holds the interleaved x and y axis for the spectrum plot. 
   GLfloat* pafMySpectrumPlotData; 

};
