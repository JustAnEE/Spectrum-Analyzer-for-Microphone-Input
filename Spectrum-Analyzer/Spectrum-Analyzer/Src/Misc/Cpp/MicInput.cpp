#include "../Hpp/MicInput.hpp"
#include "DefaultData.cpp"

#define NUM_CHANNELS (1)
#define SAMPLE_RATE (44100)
#define BITS_PER_SAMPLE (8)
#define CB_SIZE (0)
#define NUM_SAMPLES (1024*4)
#define PADDING (4)

// For now, we have a static configuration on start up. Right now there's no user input from the UI which could 
// indicate to this object to change any configuration, and I don't think there is a plan to have this exposed to the user.
// Therefore the model should not provide any parameters to the mic input until there's a need to.
MicInput::MicInput()
{
    bMyMicFound = true; 

    // -- Defining the audio format.
    formatMono44khz.wFormatTag = WAVE_FORMAT_PCM;
    formatMono44khz.nChannels = NUM_CHANNELS;
    formatMono44khz.nSamplesPerSec = SAMPLE_RATE;
    formatMono44khz.wBitsPerSample = BITS_PER_SAMPLE;
    formatMono44khz.nAvgBytesPerSec = SAMPLE_RATE * NUM_CHANNELS * BITS_PER_SAMPLE / 8;
    formatMono44khz.nBlockAlign = NUM_CHANNELS * BITS_PER_SAMPLE / 8;
    formatMono44khz.cbSize = CB_SIZE;
    iMyRawBufferSize = NUM_SAMPLES;

    pacMyRawBytesData = new char[NUM_SAMPLES];
    //! DO NOT DELETE THE INITIALIZER () 
    //! The float data is padded, the padded values need to be zero.
    //! Otherwise random memory will be FFT'd and dominate the spectra
    pafMyMicData = new float[NUM_SAMPLES * PADDING]();

    // Open Device, WAVE_MAPPER automagically finds the mic
    // If a device cannot be found, load in the default data and pack it.  
    auto openResult = waveInOpen(&hMyWaveIn, WAVE_MAPPER, &formatMono44khz, 0, 0, CALLBACK_NULL);
    if (openResult != 0)
    {
       bMyMicFound = false; 
       memcpy(pacMyRawBytesData, acTheDefaultData, NUM_SAMPLES);
       PackBytes();
    }

    return; 
}


MicInput::~MicInput()
{
   delete[] pafMyMicData;
   delete[] pacMyRawBytesData;

   // -- close device.
   auto closeResult = waveInClose(hMyWaveIn);
   return; 
};


void MicInput::PackBytes()
{
   int count = 0;

   // -- Loop through each raw sample's byte data and create a 4 byte int.
   for (int i = 0; i < iMyRawBufferSize; i += formatMono44khz.nBlockAlign)
   {
      int value = 0;
      char intBytes[4] = {};

      for (int j = 0; j < 4; j++)
      {
         intBytes[j] = (j < formatMono44khz.nBlockAlign) ? pacMyRawBytesData[i + j] : 0x00;
      }

      // -- Cast that 4 byte int to GLfloat.
      memcpy(&value, &intBytes, 4);
      pafMyMicData[count] = ((float)value) - 128.0f;
      count++;
   }

   return; 
}


void MicInput::readMicInput()
{
    // If the device driver could not be opened, use default data set at instantiation.
    if(!bMyMicFound)
    {
       // !TODO: Right now you have to restart the program if a mic is plugged in
       // We could handle this case at run time, however this will require a bit more design
       // in order to not impact the performance of this method. 
       // For now just fix the hard fault the user gets when a mic is not plugged in.
       return; 
    }

    // -- creation of the buffer header
    WAVEHDR bufH;                           /* MUST SET ITEMS BELOW PREPARE! */
    bufH.lpData = pacMyRawBytesData;            // -- pointer to the data buffer.     
    bufH.dwBufferLength = iMyRawBufferSize;          // -- buffer size in Bytes.           
    bufH.dwFlags = WHDR_BEGINLOOP;          // -- Flag, indicating buffer status. 
    bufH.dwLoops = 0L;


    /*  ~~~~~~~~~~~~~~~~~~~~~~~ ERROR FLAGS ~~~~~~~~~~~~~~~~~~~~~~~~ */
    /*  MMSYSERR_NOERROR        =  0, MMSYSERR_HANDLEBUSY     = 12,
        MMSYSERR_ERROR          =  1, MMSYSERR_INVALIDALIAS   = 13,
        MMSYSERR_BADDEVICEID    =  2, MMSYSERR_BADDB          = 14,
        MMSYSERR_NOTENABLED     =  3, MMSYSERR_KEYNOTFOUND    = 15,
        MMSYSERR_ALLOCATED      =  4, MMSYSERR_READERROR      = 16,
        MMSYSERR_INVALHANDLE    =  5, MMSYSERR_WRITEERROR     = 17,
        MMSYSERR_NODRIVER       =  6, MMSYSERR_DELETEERROR    = 18,
        MMSYSERR_NOMEM          =  7, MMSYSERR_VALNOTFOUND    = 19,
        MMSYSERR_NOTSUPPORTED   =  8, MMSYSERR_NODRIVERCB     = 20,
        MMSYSERR_BADERRNUM      =  9, WAVERR_BADFORMAT        = 32,
        MMSYSERR_INVALFLAG      = 10, WAVERR_STILLPLAYING     = 33,
    /*  MMSYSERR_INVALPARAM     = 11, WAVERR_UNPREPARED       = 34   */


    // -- prepare the Header
    auto prepareResult = waveInPrepareHeader(hMyWaveIn, &bufH, sizeof(bufH));
    if (prepareResult != 0)
    {
        std::cout << "ERROR CODE waveInPrepareHeader: " << prepareResult << "\n";
        exit(EXIT_FAILURE);
    }

    // -- create buffer, dwFlag set to WHDR_DONE when done.
    auto addBufResult = waveInAddBuffer(hMyWaveIn, &bufH, sizeof(bufH));
    if (addBufResult != 0)
    {
        std::cout << "ERROR CODE waveInAddBuffer: " << addBufResult << "\n";
        exit(EXIT_FAILURE);
    }

    // -- start recording
    auto startResult = waveInStart(hMyWaveIn);
    if (startResult != 0)
    {
        std::cout << "ERROR CODE waveInStart: " << startResult << "\n";
        exit(EXIT_FAILURE);
    }

    // -- Busy wait while device driver reads data.
    while (!(bufH.dwFlags & WHDR_DONE)) {}

    // -- stop recording.
    auto stopResult = waveInStop(hMyWaveIn);

    // -- unprepare buffer.
    auto unPrepareBuf = waveInUnprepareHeader(hMyWaveIn, &bufH, sizeof(bufH));

    // Pack the bytes read into 4 byte values 
    PackBytes();
    return; 
}

float* MicInput::GetMicData()
{
   return pafMyMicData;
}
