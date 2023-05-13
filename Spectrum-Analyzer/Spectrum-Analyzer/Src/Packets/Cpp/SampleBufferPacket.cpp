#include "../Hpp/SampleBufferPacket.hpp"
#include <cstring>

SampleBufferCL::SampleBufferCL()
{
   // Default constructor, memset to zero 
   memset(aufSampleBufferArray, 0.0, SAMPLE_BUFFER_SIZE);
   return; 
}

SampleBufferCL::SampleBufferCL(const SampleBufferCL& clSampleBuffer_)
{
   // Private copy constructor 
   return; 
}

SampleBufferCL& SampleBufferCL::operator=(const SampleBufferCL& clSampleBuffer_)
{
   SampleBufferCL clSampleBuffer;
   memcpy(&clSampleBuffer, &clSampleBuffer_, sizeof(clSampleBuffer));
   return clSampleBuffer;
}

void SampleBufferCL::SetMicData(GLfloat* aufSampleBuffer_)
{
   memcpy(aufSampleBufferArray, aufSampleBuffer_, sizeof(aufSampleBuffer_));
}