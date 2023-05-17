#include "../Hpp/SampleBufferPacket.hpp"
#include <cstring>

SampleBufferCL::SampleBufferCL()
{
   // Default constructor, memset to zero 
   memset(afMySampleBufferArray, 0.0, SAMPLE_BUFFER_SIZE);
   memset(acMyRawData, 0, NUM_SAMPLES);
   bMyMicDataRead = false; 
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

char* SampleBufferCL::GetpacRawMicData()
{
   return acMyRawData;
}

void SampleBufferCL::ConvertRawDataToFloat(int iBlockAlign)
{
    int count = 0;
    // -- Loop through each raw sample's byte data and create a 4 byte int.
    for (int i = 0; i < NUM_SAMPLES; i += iBlockAlign) {
        int value = 0;
        char intBytes[4] = {};

        for (int j = 0; j < 4; j++) {
            intBytes[j] = (j < iBlockAlign) ? acMyRawData[i + j] : 0x00;
        }

        // -- Cast that 4 byte int to GLfloat.
        memcpy(&value, &intBytes, 4);
        afMySampleBufferArray[count] = ((GLfloat)value) - 128.0f;
        count++;
    }
}

GLfloat* SampleBufferCL::GetSampleBufferArray()
{
   return afMySampleBufferArray;
}

void SampleBufferCL::SetMicData(GLfloat* aufSampleBuffer_)
{
   memcpy(afMySampleBufferArray, aufSampleBuffer_, sizeof(aufSampleBuffer_));
}