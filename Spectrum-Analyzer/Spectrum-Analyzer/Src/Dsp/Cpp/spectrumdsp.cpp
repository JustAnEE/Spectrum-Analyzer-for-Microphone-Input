#include "../Hpp/spectrumdsp.hpp"
#include "../SpectrumTypes/SpectrumBase.hpp"
#include "../SpectrumTypes/SpectralFactory.hpp"
#include "../WindowTypes/WindowFactory.hpp"
#include "../WindowTypes/WindowBase.hpp"

SpectrumDSP::SpectrumDSP(int _iMySampleRate, int _iMyBufferSize) 
{
    //!TODO: Decide once and for all, dynamic buffer sizes or nah
   iMySampleRate = _iMySampleRate;
   iMyBufferSize = _iMyBufferSize;

   pafMySpectrumPlotData = new GLfloat[2*iMyBufferSize];
   pafMyLocalSampleBuffer = new GLfloat[iMyBufferSize]; 
   pclMySpectrumPacket = new SpectrumPacket();
   pclMyFilter = new Filter(iMySampleRate, iMyBufferSize);

   //!TODO: Should probably make an interator if any more of these are added. 
   for (const SpectrumTypeEnum& eType : {MAGNITUDE_SPECTRUM, PSD_SPECTRUM, DB_SPECTRUM, DBM_SPECTRUM, PHASE_SPECTRUM})
   {
      apclMySpectrumCalculators[static_cast<int>(eType)] = SpectralFactory::Instantiate(eType, _iMyBufferSize, _iMyBufferSize); 
   }

   for (const WindowTypeEnum& eType : {RECTANGULAR_WINDOW, HAMMING_WINDOW, BLACKMAN_WINDOW, BARLETTE_WINDOW})
   {
      apclMyWindows[static_cast<int>(eType)-1] = WindowFactory::Instantiate(eType, _iMyBufferSize);
   }

   return;  
}

SpectrumDSP::~SpectrumDSP() 
{
   delete(pclMySpectrumPacket);
   delete(pclMyFilter);
   delete[](pafMyLocalSampleBuffer); 
   delete[](pafMySpectrumPlotData);
   return; 
}

void SpectrumDSP::ProcessSpectrumInitPacket(SpectrumInitPacket* pclSpectrumInitPacket_, float* pafSampleBuffer_)
{

   // Grab information from the initialisation packet 
   eMySpectrumType = pclSpectrumInitPacket_->stMyDSPInitialisation.eSpectrumOutput;
   eMyWindowType   = pclSpectrumInitPacket_->stMyDSPInitialisation.eWindow;
   eMyFilterType   = pclSpectrumInitPacket_->stMyDSPInitialisation.eFilter; 

   // Copy sample buffer into the class to avoid overwriting it in other functions
   memcpy(pafMyLocalSampleBuffer, pafSampleBuffer_, iMyBufferSize*sizeof(float));

   if (pclSpectrumInitPacket_->bDetrend)
   {
       //! TODO: Detrend adds a rediculous DC component when it should be removing it :/ 
       DetrendBuffer();
   }
   
   if (!(eMyFilterType == NO_FILTER))
   {
       // The Filter gets initialized via the SpectrumInitPacket. 
       pclMyFilter->InitializeFilter(*pclSpectrumInitPacket_);
       
       // Filter class will convolute filter indicated in the 
       // packet with the local sample buffer. 
       pclMyFilter->ApplyFilter(pafMyLocalSampleBuffer);
   }

   if (!(eMyWindowType == RECTANGULAR_WINDOW))
   {
       // Apply any windowing or filtering. 
       ApplyWindow();
   }

   // Fill the pafMySpectrumPlotDataArray with the interleaved plot data. 
   CalculateSpectrum();

   // Load the output spectrum packet.
   PopulateSpectrumPacket();

   return; 
}


void SpectrumDSP::PopulateSpectrumPacket()
{
   // Load output packet 
   pclMySpectrumPacket->eMySpectrumType = eMySpectrumType;
   memcpy(pclMySpectrumPacket->afMySpectrumArray, pafMySpectrumPlotData, 2*iMyBufferSize*sizeof(float));
   return; 
}


void SpectrumDSP::GetSpectrumOutput(SpectrumPacket& clSpectrumPacket_)
{
   clSpectrumPacket_.eMySpectrumType = eMySpectrumType;
   memcpy(clSpectrumPacket_.afMySpectrumArray, pclMySpectrumPacket->afMySpectrumArray, sizeof(pclMySpectrumPacket->afMySpectrumArray));
   return; 
}


void SpectrumDSP::ApplyWindow() 
{
   apclMyWindows[eMyWindowType]->ApplyWindow(pafMyLocalSampleBuffer);
   return;   
}


void SpectrumDSP::DetrendBuffer()
{
   //!TODO: yuck, but less yuck than before.
   apclMySpectrumCalculators[MAGNITUDE_SPECTRUM]->Detrend(pafMyLocalSampleBuffer);
   return;
}


void SpectrumDSP::CalculateSpectrum()
{
   apclMySpectrumCalculators[eMySpectrumType]->SetSampleData(pafMyLocalSampleBuffer);
   apclMySpectrumCalculators[eMySpectrumType]->FFT();
   apclMySpectrumCalculators[eMySpectrumType]->CalculateSpectrum();
   pafMySpectrumPlotData = apclMySpectrumCalculators[eMySpectrumType]->GetInterleavedData();
   return; 
}

