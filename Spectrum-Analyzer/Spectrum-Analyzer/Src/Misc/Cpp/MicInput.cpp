#include "../Hpp/MicInput.hpp"

MicInput::MicInput(int channels, long sampleRate, long bitsPerSample)
{
    // Defining the audio format.
    formatMono44khz.wFormatTag = WAVE_FORMAT_PCM;
    formatMono44khz.nChannels = channels;
    formatMono44khz.nSamplesPerSec = sampleRate;
    formatMono44khz.wBitsPerSample = bitsPerSample;
    formatMono44khz.nAvgBytesPerSec = sampleRate * channels * bitsPerSample / 8;
    formatMono44khz.nBlockAlign = channels * bitsPerSample / 8;
    formatMono44khz.cbSize = 0;
}

int MicInput::getBlockAlign()
{
    return formatMono44khz.nBlockAlign;
}

void MicInput::readMicInput(char* inputBuffer, int bufSize)
{
    // Device handle pointer.
    HWAVEIN hWaveIn;

    // Creation of the buffer header
    WAVEHDR bufH;                           /* MUST SET ITEMS BELOW PREPARE! */
    bufH.lpData = inputBuffer;              // pointer to the data buffer.     
    bufH.dwBufferLength = bufSize;          // buffer size in Bytes.           
    bufH.dwFlags = WHDR_BEGINLOOP;          // Flag, indicating buffer status. 
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

    // Open Device, WAVE_MAPPER automagically finds the mic
    auto openResult = waveInOpen(&hWaveIn, WAVE_MAPPER, &formatMono44khz, 0, 0, CALLBACK_NULL);
    if (openResult != 0)
    {
        std::cout << "ERROR CODE waveInOpen: " << openResult << "\n";
        exit(EXIT_FAILURE);
    }

    // prepare the Header
    auto prepareResult = waveInPrepareHeader(hWaveIn, &bufH, sizeof(bufH));
    if (prepareResult != 0)
    {
        std::cout << "ERROR CODE waveInPrepareHeader: " << prepareResult << "\n";
        exit(EXIT_FAILURE);
    }

    // create buffer, dwFlag set to WHDR_DONE when done.
    auto addBufResult = waveInAddBuffer(hWaveIn, &bufH, sizeof(bufH));
    if (addBufResult != 0)
    {
        std::cout << "ERROR CODE waveInAddBuffer: " << addBufResult << "\n";
        exit(EXIT_FAILURE);
    }

    // start recording
    auto startResult = waveInStart(hWaveIn);
    if (startResult != 0)
    {
        std::cout << "ERROR CODE waveInStart: " << startResult << "\n";
        exit(EXIT_FAILURE);
    }

    // Busy wait while device driver reads data.
    while (!(bufH.dwFlags & WHDR_DONE)) {}

    // stop recording.
    auto stopResult = waveInStop(hWaveIn);

    // unprepare buffer.
    auto unPrepareBuf = waveInUnprepareHeader(hWaveIn, &bufH, sizeof(bufH));

    // close device.
    auto closeResult = waveInClose(hWaveIn);
}

