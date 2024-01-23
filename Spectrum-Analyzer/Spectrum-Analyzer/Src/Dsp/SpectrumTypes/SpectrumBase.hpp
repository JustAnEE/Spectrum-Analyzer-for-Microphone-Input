#ifndef SPECTRUM_BASE_HPP
#define SPECTRUM_BASE_HPP

// Forward declarations 
class FFTCL; 

class SpectrumBase 
{
   // Public constructors / destructors 
   public:
      SpectrumBase(int iBufferSize_, int iSampleRate_); 
      virtual ~SpectrumBase();

   // Public methods 
   public:
      virtual void CalculateSpectrum() = 0;
      void SetSampleData(float* pafSampleData_);
      void Detrend(float* pafSampleData_);
      void FFT();
      void IFFT();
      float* GetInterleavedData(); 

   // Protected methods 
   protected:
      // Do not let any inheriters overwrite, the interleaving
      // process is the same across types 
      virtual void Interleave() final;

   // Protected fields 
   protected:
      float* pafMyMagnitudeData;
      float* pafMyInterleavedData;
      int iMyBufferSize;
      int iMySampleRate;

   // Private fields 
   private:
      FFTCL* pclMyFFTObj;

   

};

#endif 
