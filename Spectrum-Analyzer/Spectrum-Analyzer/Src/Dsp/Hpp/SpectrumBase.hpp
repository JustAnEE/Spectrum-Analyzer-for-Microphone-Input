#ifndef SPECTRUMBASE_HPP
#define SPECTRUMBASE_HPP

#include <fftw3.h>
#include <math.h> 
#include <iostream>
#include <glad/glad.h>

#include "../Hpp/Filter.hpp"
#include "../Hpp/SpectrumInitPacket.hpp"
#include "../Hpp/SpectrumPacket.hpp"
#include "../../Utils/enums.h"
#include "Window.hpp"
#include "BlackmanWindow.hpp"
#include "BarletteWindow.hpp"
#include "HammingWindow.hpp"

#define REAL 1
#define IMAG 0

typedef fftwf_complex FFTData;

class SpectrumBase
{

  // friend class Window;

public:
   SpectrumBase(int _iMySampleRate, int _iMyBufferSize);
   virtual ~SpectrumBase() {}

// Public methods 
public:
   virtual void ProcessSpectrumInitPacket(SpectrumInitPacket* pclSpectrumInitPacket_, GLfloat* pafSampleBuffer_) = 0;
   virtual SpectrumPacket* GetSpectrumOutput();

// Protected methods
protected:
   // FFT calculation, implementation never changes 
   virtual FFTData* FFT();
   // IFFT calculation, implementation never changes. 
   //virtual FFTData* IFFT(FFTData* in, FFTData* out);

   // Prepare the data for plot {x,y, x,y, x,y ....}. Implementation never changes.
   virtual void PrepDataForPlot();

   // Applys a window to the sample buffer data 
   virtual void ApplyWindow(WindowTypeEnum eWindowType);

   // Generates the frequency arrray used 
   virtual void GenerateFrequency();

   virtual void CalculateSpectrum() = 0; 
   virtual void PopulateSpectrumPacket() = 0; 

// Protected Attributes
protected:
   //! This is a pointer to a spectrum packet. 
   SpectrumPacket* pclMySpectrumPacket;

   //! This is a pointer to a Filter 
   Filter* pclMyFilter;

   // Array of pointers to windowing classes
   Window* apclMyWindows[5]; 

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

   //! This is a pointer to the local copy of the sample buffer read in from the microphone.
   GLfloat* pafMyLocalSampleBuffer;

   //! This is an array which holds the spectrum data output (y axis). 
   GLfloat* pafMySpectrumData;

   //! This is an array which holds the interleaved x and y axis for the spectrum plot. 
   GLfloat* pafMySpectrumPlotData;

};

#endif 