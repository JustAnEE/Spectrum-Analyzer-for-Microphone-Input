#include<Windows.h>
#include<mmsystem.h>
#include<iostream>
#include<fstream>
using namespace std;

// -- custom header for testing
struct WAVEFILE {
    /* "RIFF" Chunk descriptor */
    char ChunkID[4]{ 'R', 'I', 'F', 'F' }; // 0x52494646
    int  ChunkSize;
    char Format[4]{ 'W', 'A', 'V', 'E' }; // 0x57415645

    /* "fmt" Sub-Chunk*/
    char Subchunk1ID[4]{ 'f', 'm', 't', ' ' }; // 0x666D7420
    int Subchunk1Size;
    short AudioFormat;      // -- no compression
    short NumChannels;      // -- Mono = 1, Stereo = 2, etc.
    int SampleRate;
    int ByteRate;           // -- SampleRate * NumChannels * BitsPerSample/8
    short BlockAlign;       // -- NumChannels * BitsPerSample/8
    short BitsPerSample;

    /* "data" Sub-Chunk */
    char Subchunk2ID[4]{ 'd', 'a', 't', 'a' };
    int Subchunk2Size;      // -- number of bytes in data
    char* Data;
} wavefile;


int main() {
    // -- sanity check, returns the number of audio devices found on the system. 0 -> n-1 devices.
    cout << "# of input devices: " << waveInGetNumDevs() << endl;

    // -- Waveform
    wavefile.ChunkSize = 36 + 65536;    // -- 36 + subchunk2size
    wavefile.Subchunk1Size = 16;
    wavefile.AudioFormat = 1;
    wavefile.NumChannels = 1;
    wavefile.SampleRate = 44100;
    wavefile.ByteRate = 44100;
    wavefile.BlockAlign = 1;
    wavefile.BitsPerSample = 8;
    wavefile.Subchunk2Size = 65536;

    // -- manual header definition is purly for testing.
    char header[44];

    // -- add data to the header
    header[0] = wavefile.ChunkID[0]; header[1] = wavefile.ChunkID[1];
    header[2] = wavefile.ChunkID[2]; header[3] = wavefile.ChunkID[3];

    // -- 0 1 0 36   -->  byte[4] = 0, byte[3] = 1 => 65536, byte[2] = 0, byte[1] = 36.
    // -- bit shift to last 8 bits, bit-wise and with 11111111
    // -- -- also gives big-endian bits
    header[7] = (wavefile.ChunkSize >> 24) & 0xFF;
    header[6] = (wavefile.ChunkSize >> 16) & 0xFF;
    header[5] = (wavefile.ChunkSize >> 8) & 0xFF;
    header[4] = wavefile.ChunkSize & 0xFF;

    header[8] = wavefile.Format[0]; header[9] = wavefile.Format[1];
    header[10] = wavefile.Format[2]; header[11] = wavefile.Format[3];

    header[12] = wavefile.Subchunk1ID[0]; header[13] = wavefile.Subchunk1ID[1];
    header[14] = wavefile.Subchunk1ID[2]; header[15] = wavefile.Subchunk1ID[3];

    // -- 0 0 0 16 
    header[19] = (wavefile.Subchunk1Size >> 24) & 0xFF;
    header[18] = (wavefile.Subchunk1Size >> 16) & 0xFF;
    header[17] = (wavefile.Subchunk1Size >> 8) & 0xFF;
    header[16] = wavefile.Subchunk1Size & 0xFF;

    // -- 0 1 
    header[21] = (wavefile.AudioFormat >> 8) & 0xFF;
    header[20] = wavefile.AudioFormat & 0xFF;

    // -- 0 1 
    header[23] = (wavefile.NumChannels >> 8) & 0xFF;
    header[22] = wavefile.NumChannels & 0xFF;

    // -- 0 0 172 68 
    header[27] = (wavefile.SampleRate >> 24) & 0xFF;
    header[26] = (wavefile.SampleRate >> 16) & 0xFF;
    header[25] = (wavefile.SampleRate >> 8) & 0xFF;
    header[24] = wavefile.SampleRate & 0xFF;

    // -- 0 0 172 68 
    header[31] = (wavefile.ByteRate >> 24) & 0xFF;
    header[30] = (wavefile.ByteRate >> 16) & 0xFF;
    header[29] = (wavefile.ByteRate >> 8) & 0xFF;
    header[28] = wavefile.ByteRate & 0xFF;

    // -- 0 1 
    header[33] = (wavefile.BlockAlign >> 8) & 0xFF;
    header[32] = wavefile.BlockAlign & 0xFF;

    // -- 0 8 
    header[35] = (wavefile.BitsPerSample >> 8) & 0xFF;
    header[34] = wavefile.BitsPerSample & 0xFF;

    header[36] = wavefile.Subchunk2ID[0]; header[37] = wavefile.Subchunk2ID[1];
    header[38] = wavefile.Subchunk2ID[2]; header[39] = wavefile.Subchunk2ID[3];

    // -- 0 0 1 0 
    header[43] = (wavefile.Subchunk2Size >> 24) & 0xFF;
    header[42] = (wavefile.Subchunk2Size >> 16) & 0xFF;
    header[41] = (wavefile.Subchunk2Size >> 8) & 0xFF;
    header[40] = wavefile.Subchunk2Size & 0xFF;

    // -- device handle pointer.
    HWAVEIN hWaveIn;

    // -- Defining the audio format.
    WAVEFORMATEX formatMono44khz;
    formatMono44khz.wFormatTag = WAVE_FORMAT_PCM;
    formatMono44khz.nChannels = 1;
    formatMono44khz.nSamplesPerSec = 44100L;
    formatMono44khz.nAvgBytesPerSec = 44100L;
    formatMono44khz.nBlockAlign = 1;
    formatMono44khz.wBitsPerSample = 8;
    formatMono44khz.cbSize = 0;

    // -- creation waveform-audio buffer.
    DWORD bufSize = 65536; /* 1.4 seconds */
    char buffer[65536];

    // -- creation of the buffer header
    WAVEHDR bufH;                           /* MUST SET ITEMS BELOW PREPARE! */
    bufH.lpData = (LPSTR)buffer;            // -- pointer to the data buffer.     
    bufH.dwBufferLength = bufSize;          // -- buffer size in Bytes.           
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


    // -- Open Device, WAVE_MAPPER automagically finds the mic
    auto openResult = waveInOpen(&hWaveIn, WAVE_MAPPER, &formatMono44khz, 0, 0, CALLBACK_NULL);
    if (openResult != 0) {
        cout << "ERROR CODE waveInOpen: " << openResult << endl;
        return -1;
    }

    // -- prepare the Header
    auto prepareResult = waveInPrepareHeader(hWaveIn, &bufH, sizeof(bufH));
    if (prepareResult != 0) {
        cout << "ERROR CODE waveInPrepareHeader: " << prepareResult << endl;
        return -1;
    }

    // -- create buffer, dwFlag set to WHDR_DONE when done.
    auto addBufResult = waveInAddBuffer(hWaveIn, &bufH, sizeof(bufH));
    if (addBufResult != 0) {
        cout << "ERROR CODE waveInAddBuffer: " << addBufResult << endl;
        return -1;
    }

    // -- start recording
    auto startResult = waveInStart(hWaveIn);
    if (startResult != 0) {
        cout << "ERROR CODE waveInStart: " << startResult << endl;
        return -1;
    }

    // -- Busy wait while device driver reads data.
    while (!(bufH.dwFlags & WHDR_DONE)) {
        cout << "reading..." << bufH.dwFlags << endl;
    }
    cout << "bytes recorded:  " << bufH.dwBytesRecorded << endl;


    // -- stop recording.
    auto stopResult = waveInStop(hWaveIn);

    // -- unprepare buffer.
    auto unPrepareBuf = waveInUnprepareHeader(hWaveIn, &bufH, sizeof(bufH));

    // -- close device.
    auto closeResult = waveInClose(hWaveIn);


    // -- write data
    ofstream audioFile;
    audioFile.open("test2.WAV", ios::binary | ios::out);

    // -- write header info
    audioFile.write(header, 44);

    // -- write data
    audioFile.write(buffer, 65536);
}
