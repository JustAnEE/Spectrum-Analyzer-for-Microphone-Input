#pragma once
#include <glad/glad.h>
#include <iostream>
#include "SpectrumTypes.hpp"
#include "SpectrumInitPacket.hpp"

class Filter 
{

// Constructors and Destructors
public:

   Filter(int iSampleRate_, int iBufferSize_);
   ~Filter(); 

// Public Methods 
public:

   void InitializeFilter(const SpectrumInitPacket& cpSpectrumInit_);
   void ApplyFilter(GLfloat* pafSampleBuffer_);

// Private Methods 
private:

	void Convolve(GLfloat* pafSampleBuffer_); 
	void GenLPF(); 
	void LowToHigh(); 

// Private Attributes.
private:

	//! This attribute contains information to initialize the filter 
	DSPFilterInitStruct stMyDSPInit;
	
	//! This attribute is the filter type.
	FilterTypeEnum eMyFilterType; 

	//! This is the sample rate of the system. 
	int iMySampleRate;

	//! This is the buffer size of the system.
	int iMyBufferSize; 

	//! This is the filter length
	int fMyFilterLength; 

	//! This is the pointer to the filter taps. 
	GLfloat* pafMyFilterTaps; 

};
