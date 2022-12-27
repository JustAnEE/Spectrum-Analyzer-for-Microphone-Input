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
	*
	* @param channels,		 An Integer representing the number of channels.
	* @param sampleRate,	 An Integer representing the number of sampleRate.
	* @param bitsPerSample,	 An Integer representing the number of bitsPerSample.
	* @return,				 MicInput object.
	*/
	MicInput(int channels, long sampleRate, long bitsPerSample);


	/**
	* MicInput Object Constructor
	*
	* @param channels,		 An Integer representing the number of channels.
	* @param sampleRate,	 An Integer representing the number of sampleRate.
	* @param bitsPerSample,	 An Integer representing the number of bitsPerSample.
	* @return,				 MicInput object.
	*/
	void readMicInput(char* buffer, int size);


	/**
	* Gets the Block align from the Windows.h WAVEFORMATEX object.
	*
	* @return,	An Integer.
	*/
	int getBlockAlign();

private:
	WAVEFORMATEX formatMono44khz;
};


#endif // !MIC_H