#ifndef SPECTRUMDSPCONFIG_HPP
#define SPECTRUMDSPCONFIG_HPP

#include "../../Utils/constants.h"
#include "../../Utils/enums.h"
#include "../../Packets/Hpp/SampleBufferPacket.hpp"
#include "../../Utils/types.h"
#include <memory>

// !TODO: glad.h is included everywhere GLfloat is needed, either remove dependency on GLfloat
// ! or work this type specifically into the tpyes / constants header 
#include <glad/glad.h>

class SpectrumDSPConfig
{
   // Let Spectrum Base directly access the private members of this config class
   // it mostly exists to slightly decouple anything interacting with the DSP directly. 
   friend class SpectrumBase; 

public:

    SpectrumDSPConfig() {}

public:
    
    // Configure the DSP settings such as Window/Filter Type, Normalization, Detrending 
    void ConfigDSP(DSPInitStruct& stDSPInit_, GLfloat* pafMicData_);

    // !TODO: Actually use this 
    void ConfigDSP(DSPInitStruct& stDSPinit_, std::shared_ptr<SampleBufferCL> pclSampleBufferSP_);

    void ConfigDSPOnRequest(DSPInitStruct& stDSPInit_);

    SampleBufferCL* GetSampleBuffer(); 

    // Optional method allows the user to select the default config and pass in only the Mic Data and Spectrum type
    // Default config: Rectangular Window, No Filter, No Normalization, No Detrending. 
    void UseDefaultConfig(SpectrumTypeEnum eSpectrumType_, GLfloat* pafMicData_);

    bool HasDSPBeenConfigured() { return bMyDSPInitialized; }

private:

   //! This struct has data which configures DSP 
   DSPInitStruct stMyDSPInitData;

   //! This is a temporary sample buffer.
   //! TODO: Remove this when SampleBufferCL is utilized. 
   GLfloat afMySampleBufferData[SAMPLE_BUFFER_SIZE];

   //! This keeps track of whether or not a DSP has been initalized.
   bool bMyDSPInitialized; 

   //! Todo: Instead of passing arround sample buffer array pointers, pass around shared pointers
   //! to the sample buffer class. 
   SampleBufferCL clMySampleBuffer;

};

#endif 
