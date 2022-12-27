#pragma once 

#include <glad/glad.h>
#include <cstring>

#include "../Hpp/SpectrumTypes.hpp"

// DSP Initializer struct, gives information to the SpectrumDSP class 
// as to what output is required, etc.
typedef struct DSPInitStruct
{
    int iBufferSize;
    int iSampleRate;
    SpectrumTypeEnum eSpectrumOutput;
    WindowTypeEnum eWindow;
    FilterTypeEnum eFilter; 
}DSPInitStruct;

typedef struct DSPFilterInitStruct
{
    GLfloat fCutOffHz_LPF;
    GLfloat fCutOffHz_HPF;
    GLfloat fLeftEdge_BPF;
    GLfloat fRightEdge_BPF;
    GLfloat fLeftEdge_BSF;
    GLfloat fRightEdge_BSF; 
}DSPFilterInitStruct;

class SpectrumInitPacket
{
public: 

//Constructors/Destructors
public:

    SpectrumInitPacket(); 
    void CopyTo (SpectrumInitPacket& clSpectrumInitPacket_);

    void SetDefaults();

// Attributes (POD - Public)
public:

   bool bIsNormalized; 
   bool bIsFsandBufferSizeDefault;
   bool bDetrend; 
   DSPInitStruct       stMyDSPInitialisation; 
   DSPFilterInitStruct stMyFilterInitialisation;

};