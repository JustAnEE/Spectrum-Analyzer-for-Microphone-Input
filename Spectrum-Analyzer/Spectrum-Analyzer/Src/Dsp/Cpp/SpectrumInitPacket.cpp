#include "../Hpp/SpectrumInitPacket.hpp"
#include <cstring>

SpectrumInitPacket::SpectrumInitPacket()
{
   SetDefaults();
}


void SpectrumInitPacket::CopyTo(SpectrumInitPacket& clSpectrumInitPacket_)
{
   bIsFsandBufferSizeDefault = clSpectrumInitPacket_.bIsFsandBufferSizeDefault;
   bIsNormalized = clSpectrumInitPacket_.bIsNormalized;
   memcpy(&stMyDSPInitialisation, &clSpectrumInitPacket_.stMyDSPInitialisation, sizeof(&clSpectrumInitPacket_.stMyDSPInitialisation));
   memcpy(&stMyFilterInitialisation, &clSpectrumInitPacket_.stMyFilterInitialisation,  sizeof(&clSpectrumInitPacket_.stMyFilterInitialisation));

}

void SpectrumInitPacket::SetDefaults()
{/*
   bIsFsandBufferSizeDefault = true;
   stMyDSPInitialisation.eSpectrumOutput = MAGNITUDE_SPECTRUM;
   stMyDSPInitialisation.eWindow = RECTANGULAR_WINDOW;
   stMyDSPInitialisation.iSampleRate = 44100;
   stMyDSPInitialisation.iBufferSize = 1024 * 16;
   */
}