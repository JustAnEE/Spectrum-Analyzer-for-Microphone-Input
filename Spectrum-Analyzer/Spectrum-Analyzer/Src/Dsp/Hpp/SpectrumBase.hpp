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
#include "SpectrumDSPConfig.hpp"
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

   // After the DSP is configed, this method will produce the output spectrum 
   void CalculateSpectralData();
   
   // This method returns the pointer to the spectrum output packet. 
   virtual SpectrumPacket* GetSpectrumOutput();

   // This method returns the pointer to the spectrum config class. 
   virtual SpectrumDSPConfig* GetpclSpectrumConfig();

   // Awkward method which reformats the input data into output time series data
   // for the option where only the time series is displaced. Moved into the 
   // DSP since that's probably the least awkward spot at the moment. 
   GLfloat* FormatTimeSeries(GLfloat* pafMicData_);

// Protected methods
protected:
   // FFT calculation, implementation never changes 
   virtual FFTData* FFT();
   // IFFT calculation, implementation never changes. 
   //virtual FFTData* IFFT(FFTData* in, FFTData* out);

   // Normalize the spectral output. 
   virtual void NormalizeSpectrum();

   // Prepare the data for plot {x,y, x,y, x,y ....}. Implementation never changes.
   virtual void PrepDataForPlot();

   // Applys a window to the sample buffer data 
   virtual void ApplyWindow();

   // Generates the frequency arrray used 
   virtual void GenerateFrequency();

   // Calculate the Spectral Magnitude of the spectrum 
   virtual void CalculateFFTMagnitude(); 

   // Calculate the spectrum, see specific spectrum type for implementation
   virtual void CalculateSpectrum() = 0; 

   // Populates the outgoing packet with the spectrum data ready for plot. 
   virtual void PopulateSpectrumPacket() = 0; 

   // Resets the DSP 
   virtual void Reset();



// Protected Attributes
protected:
   //! This is a pointer to a spectrum packet. 
   SpectrumPacket* pclMySpectrumPacket;

   //! This is a pointer to a Filter 
   Filter* pclMyFilter;

   // Array of pointers to windowing classes
   Window* apclMyWindows[WindowTypeEnum::WINDOW_COUNT]; 

   //! This is the sample rate of the system. 
   int iMySampleRate;

   //! This is the size of the incoming sample buffer. 
   int iMyBufferSize;

   //! This tells the DSP to normalize the output spectrum. 
   bool bMyNormalize;

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

   //! This is the configuration of the Spectrum DSP 
   SpectrumDSPConfig clMySpectrumDSPConfig; 

};

#endif 