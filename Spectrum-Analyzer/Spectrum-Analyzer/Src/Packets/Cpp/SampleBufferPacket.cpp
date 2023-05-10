#include "../Hpp/SampleBufferPacket.hpp"
#include <cstring>

SampleBufferPacket::SampleBufferPacket()
{
   // Default constructor, memset to zero 
   memset(aufSampleBufferArray, 0.0, SAMPLE_BUFFER_SIZE);
   return; 
}

SampleBufferPacket::SampleBufferPacket(const SampleBufferPacket& clSampleBufferPacket_)
{
   // Private copy constructor 
   return; 
}

SampleBufferPacket& SampleBufferPacket::operator=(const SampleBufferPacket& clSampleBufferPacket_)
{
   SampleBufferPacket clSampleBuffer;
   memcpy(&clSampleBuffer, &clSampleBufferPacket_, sizeof(clSampleBuffer));
   return clSampleBuffer;
}

void SampleBufferPacket::SetMicData(GLfloat* aufSampleBuffer_)
{
   memcpy(aufSampleBufferArray, aufSampleBuffer_, sizeof(aufSampleBuffer_));
}