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

   char* GetpacRawMicData(); 

   GLfloat* GetSampleBufferArray();

   void ConvertRawDataToFloat(int iBlockAlign);

private:
   //! Array which stores the sample buffer. 
    GLfloat afMySampleBufferArray[SAMPLE_BUFFER_SIZE];

   //! Raw Data Buffer which stores the samples directly read from microphone 
   char acMyRawData[NUM_SAMPLES];

   //! Boolean which indicates whether or not microphone has been read
   bool bMyMicDataRead; 

// Private copy constructor, not implemented. Use = operator to get data. 
private:
   SampleBufferCL(const SampleBufferCL& clSampleBuffer_);
};

#endif 