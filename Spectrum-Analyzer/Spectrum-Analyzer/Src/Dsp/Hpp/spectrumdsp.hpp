#ifndef SPECTRUM_DSP_HPP
#define SPECTRUM_DSP_HPP

#include <fftw3.h>
#include <math.h> 
#include <iostream>

#include "../Hpp/Filter.hpp"
#include "../Hpp/SpectrumInitPacket.hpp"
#include "../Hpp/SpectrumPacket.hpp"
#include "../Hpp/SpectrumTypes.hpp"


// Forward declarations 
class WindowBase; 
class SpectrumBase; 

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
      void ApplyWindow();
      void DetrendBuffer();
      void CalculateSpectrum();
      void PopulateSpectrumPacket();

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

      //! This is a pointer to the local copy of the sample buffer read in from the microphone.
      float* pafMyLocalSampleBuffer;

      //! This is an array which holds the interleaved x and y axis for the spectrum plot. 
      float* pafMySpectrumPlotData; 

      //! This is an array which holds the windowing objects. 
      WindowBase* apclMyWindows[NUM_WINDOWS]; 

      //! This is an array which holds the spectral calculator objects 
      SpectrumBase* apclMySpectrumCalculators[NUM_SPECTRUM];

};

#endif 