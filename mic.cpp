#include<Windows.h>
#include<mmsystem.h>
#include<iostream>
#include<fstream>
#include<iostream>
using namespace std;


int main(){
// -- sanity check, returns the number of audio devices found on the system. 0 -> n-1 devices.
cout << "# of input devices: " << waveInGetNumDevs() << endl;


// -- Defining the audio format.
WAVEFORMATEX formatMono44khz;
formatMono44khz.wFormatTag = WAVE_FORMAT_PCM;
formatMono44khz.nChannels = 1;
formatMono44khz.nSamplesPerSec = 44100L;
formatMono44khz.nAvgBytesPerSec = 88200L;
formatMono44khz.nBlockAlign = 2; 
formatMono44khz.wBitsPerSample = 16;
formatMono44khz.cbSize = 0;

// -- creation waveform-audio buffer.
// -- The LPSTR type 8-bit characters, which MAY be terminated by a null character.
DWORD bufferSizeBytes = 65536; /* 2^20 - 11MB */
char buffer [65536] = { };

LPSTR bufPtr = buffer;

// -- creation of the buffer header
WAVEHDR bufH;                    /* MUST SET ITEMS BELOW PREPARE!   */
bufH.lpData = bufPtr;                   /* pointer to the data buffer.     */
bufH.dwBufferLength = bufferSizeBytes;  /* buffer size in Bytes.           */
bufH.dwFlags = WHDR_BEGINLOOP;          /* Flag, indicating buffer status. */



/*  ~~~~ ERROR FLAGS ~~~~ */
/*  MMSYSERR_BADDEVICEID  */
/*  MMSYSERR_ALLOCATED    */
/*  MMSYSERR_NOMEM        */
/*  MMSYSERR_NODRIVER     */
/*  MMSYSERR_INVALHANDLE  */
/*  WAVERR_STILLPLAYING   */


// -- Get device, WAVE_MAPPER flag is auto find mic
HWAVEIN hWaveIn;
auto openResult = waveInOpen((LPHWAVEIN)&hWaveIn, WAVE_MAPPER, &formatMono44khz, 0, 0, CALLBACK_NULL);

// --  prepare the Header
auto prepareResult = waveInPrepareHeader(hWaveIn, &bufH, sizeof(bufH));

// -- create buffer, dwFlag set to WHDR_DONE when done.
auto addBufResult = waveInAddBuffer(hWaveIn, &bufH, sizeof(bufH));

// -- start recording
auto startResult = waveInStart(hWaveIn);

// -- test read data
bool RECORDING = true;
ofstream audiofile;
audiofile.open("test.WAV");

cout << "getting data..." << endl;

// -- zero pad data to power of two.
while(RECORDING && !audiofile){
    // -- proccess audio information
    cout << "reading..." << endl;
    if(bufH.dwFlags == WHDR_DONE){
        cout << "ninjad" << endl;
        audiofile << bufH.lpData;
        break;
    }
}
cout << "stopping..." << endl;
audiofile.close();

// -- stop recording
auto stopResult = waveInStop(hWaveIn);

// -- close device
auto closeResult = waveInClose(hWaveIn);

}
