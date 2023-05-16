#pragma once 

#include <glad/glad.h>
#include <cstring>

#include "../../Utils/enums.h"
#include "../../Utils/types.h"

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