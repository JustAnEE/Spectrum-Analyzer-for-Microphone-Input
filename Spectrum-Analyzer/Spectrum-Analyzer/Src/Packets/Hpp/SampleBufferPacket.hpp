#ifndef SAMPLEBUFFERPACKET_HPP
#define SAMPLEBUFFERPACKET_HPP

#include "../../Utils/constants.h"
#include "../../../Libraries/include/glad/glad.h"

class SampleBufferCL
{
// Constructors 
public:
   SampleBufferCL();

// Operators 
public:
   SampleBufferCL& operator = (const SampleBufferCL& clSampleBuffer_);

// Public methods. 
public:
   void SetMicData(GLfloat* aufSampleBuffer_);

private:
   //! Array which stores the sample buffer. 
   GLfloat aufSampleBufferArray[SAMPLE_BUFFER_SIZE];

// Private copy constructor, not implemented. Use = operator to get data. 
private:
   SampleBufferCL(const SampleBufferCL& clSampleBuffer_);
};

#endif 