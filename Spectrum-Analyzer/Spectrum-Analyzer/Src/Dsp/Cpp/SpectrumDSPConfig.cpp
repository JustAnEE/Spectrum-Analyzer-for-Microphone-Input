#include "../Hpp/SpectrumDSPConfig.hpp"
#include <cstring>

void SpectrumDSPConfig::ConfigDSP(DSPInitStruct& stDSPInit_, GLfloat* pafMicData_)
{
   memcpy(&stMyDSPInitData, &stDSPInit_, sizeof(DSPInitStruct));
   memcpy(afMySampleBufferData, pafMicData_, SAMPLE_BUFFER_SIZE);
   // Indicate that this DSP has been initialized. 
   bMyDSPInitialized = true; 
}

void SpectrumDSPConfig::ConfigDSP(DSPInitStruct& stDSPInit_, std::shared_ptr<SampleBufferCL> pclSampleBufferSP_)
{
   clMySampleBuffer = *pclSampleBufferSP_;
}

void SpectrumDSPConfig::UseDefaultConfig(SpectrumTypeEnum eSpectrumType_, GLfloat* pafMicData_)
{
   memcpy(afMySampleBufferData, pafMicData_, SAMPLE_BUFFER_SIZE);
   stMyDSPInitData.bDetrend = false;
   stMyDSPInitData.bIsNormalized = false;
   stMyDSPInitData.eSpectrumOutput = eSpectrumType_;
   stMyDSPInitData.eWindow = RECTANGULAR_WINDOW;
   stMyDSPInitData.eFilter = NO_FILTER;
   // Indicate that the DSP has been configured. 
   bMyDSPInitialized = true; 
}