#ifndef SPECTRUMOUTPUTPACKET_HPP
#define SPECTRUMOUTPUTPACKET_HPP

#include "../../Utils/constants.h"
#include "../../../Libraries/include/glad/glad.h"
#include "../../Utils/types.h"

class SpectrumOutputCL
{

// POD Class 
public:

   GLfloat afSpectrumOutput[2 * NUM_SAMPLES];
   DSPInitStruct stDSPInit;

};


#endif 