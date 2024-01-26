#ifndef SPECTRUM_INIT_PACKET_HPP
#define SPECTRUM_INIT_PACKET_HPP

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
} DSPInitStruct;

typedef struct DSPFilterInitStruct
{
    float fCutOffHz_LPF;
    float fCutOffHz_HPF;
    float fLeftEdge_BPF;
    float fRightEdge_BPF;
    float fLeftEdge_BSF;
    float fRightEdge_BSF; 
} DSPFilterInitStruct;

class SpectrumInitPacket
{ 
   //Constructors/Destructors
   public:

      SpectrumInitPacket();

   // Public Methods 
   public:
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

#endif 