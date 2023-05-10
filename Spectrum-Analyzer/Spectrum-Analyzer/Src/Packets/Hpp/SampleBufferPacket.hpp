#ifndef SAMPLEBUFFERPACKET_HPP
#define SAMPLEBUFFERPACKET_HPP

#include "../../Utils/constants.h"
#include "../../../Libraries/include/glad/glad.h"

class SampleBufferPacket
{
// Constructors 
public:
   SampleBufferPacket();

// Operators 
public:
   SampleBufferPacket& operator = (const SampleBufferPacket& clSampleBufferPacket_);

// Public methods. 
public:
   void SetMicData(GLfloat* aufSampleBuffer_);

private:
   //! Array which stores the sample buffer. 
   GLfloat aufSampleBufferArray[SAMPLE_BUFFER_SIZE];

// Private copy constructor, not implemented. Use = operator to get data. 
private:
   SampleBufferPacket(const SampleBufferPacket& clSampleBufferPacket_);
};

#endif 