#pragma once 

#include <fftw3.h>
#include <math.h> 
#include <iostream>
#include <glad/glad.h>

#include "../Hpp/SpectrumTypes.hpp"

#define SPECTRUM_OUTPUT_SIZE ((int) 2*1024*16)

// POD Data Packet
class SpectrumPacket
{

// Constructors/Destructors
public:
   // Default constructor 
   SpectrumPacket();

   // Copy constructor
   SpectrumPacket(const SpectrumPacket& clSpectrumPacket_);


// Methods
public:
   
// Attributes
public:

   SpectrumTypeEnum eMySpectrumType;
   GLfloat afMySpectrumArray[SPECTRUM_OUTPUT_SIZE];

};