#include "SpectrumPacket.hpp"

SpectrumPacket::SpectrumPacket()
{

}

SpectrumPacket::SpectrumPacket(const SpectrumPacket& clSpectrumPacket_)
{
   memcpy(afMySpectrumArray, clSpectrumPacket_.afMySpectrumArray, sizeof(clSpectrumPacket_.afMySpectrumArray));
   eMySpectrumType = clSpectrumPacket_.eMySpectrumType; 
}