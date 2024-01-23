#include "FFTCL.hpp"
#include "SpectrumBase.hpp"


SpectrumBase::SpectrumBase(int iBufferSize_, int iSampleRate_)
{
   iMyBufferSize = iBufferSize_; 
   iMySampleRate = iSampleRate_;
   
   pafMyInterleavedData = new float [2*iBufferSize_];
   pafMyMagnitudeData = nullptr;

   pclMyFFTObj = new FFTCL(iBufferSize_, iSampleRate_);
   return; 
}

SpectrumBase::~SpectrumBase()
{
   delete pclMyFFTObj;
   return; 
}

void SpectrumBase::SetSampleData(float* pafSampleData_)
{
   pclMyFFTObj->SetSampleData(pafSampleData_);
   return; 
}

void SpectrumBase::FFT()
{
   pclMyFFTObj->FFT();
   pafMyMagnitudeData = pclMyFFTObj->GetMagnitudeData();
   return; 
}

void SpectrumBase::IFFT()
{
   pclMyFFTObj->IFFT();
   pafMyMagnitudeData = pclMyFFTObj->GetMagnitudeData();
   return; 
}

void SpectrumBase::Interleave()
{
   int iPlotDataSize = 2 * iMyBufferSize;

   for (int k = 0; k < iPlotDataSize; k += 2) {

      int index = k / 2;

      pafMyInterleavedData[k] = pclMyFFTObj->GetFrequencyVector()[index];
      pafMyInterleavedData[k + 1] = pafMyMagnitudeData[index];

   }
   return; 
}

//!TODO: Adds DC component when it should remove it...
void SpectrumBase::Detrend(float* pafSampleData_)
{
   float fBufferAvg = 0;

   for (int k = 0; k < iMyBufferSize; k++) 
   {
      fBufferAvg += pafSampleData_[k];
   }

   for (int k = 0; k < iMyBufferSize; k++) 
   {
      pafSampleData_[k] -= fBufferAvg;
   }
   return;   
}

float* SpectrumBase::GetInterleavedData()
{

   return pafMyInterleavedData;
}

