#ifndef MIC_H
#define MIC_H

#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#include <fstream>


class MicInput {
   public:
      /**
      * MicInput Object Constructor
      */
      MicInput();
      
      /**
      * MicInput Object Destructor 
      * @param void 
      * return, void 
      * Deallocates heap memory the object owns 
      */
      ~MicInput(); 

      /**
      * MicInput Object Constructor
      *
      * @param channels,		 An Integer representing the number of channels.
      * @param sampleRate,	 An Integer representing the number of sampleRate.
      * @param bitsPerSample,	 An Integer representing the number of bitsPerSample.
      * @return,				 MicInput object.
      */
      void readMicInput();

      float* GetMicData();

   private:
      /**
      * Packs bytes into 4 byte values
      * 
      * @return,   void.  
      */
      void PackBytes();

   private:
      WAVEFORMATEX formatMono44khz;
      char* pacMyRawBytesData;
      float* pafMyMicData;
      int iMyRawBufferSize;
};


#endif // !MIC_H