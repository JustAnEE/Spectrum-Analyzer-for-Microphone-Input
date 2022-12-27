#include "../Hpp/Filter.hpp"


Filter::Filter(int iSampleRate_, int iBufferSize_) 
{
 
    iMySampleRate = iSampleRate_;
    iMyBufferSize = iBufferSize_;

    // Set filter length to buffer size for now. 
    fMyFilterLength = iMyBufferSize;

    pafMyFilterTaps = new GLfloat[fMyFilterLength];

}

Filter::~Filter() 
{
   delete[]pafMyFilterTaps; 
}

void Filter::InitializeFilter(const SpectrumInitPacket& clSpectrumInit_)
{
    // Copy the Filter initialisation struct from the spectrum initializer class
    memcpy(&stMyDSPInit, &clSpectrumInit_.stMyFilterInitialisation, sizeof(clSpectrumInit_.stMyFilterInitialisation));
    // Set the Filter Type
    eMyFilterType = clSpectrumInit_.stMyDSPInitialisation.eFilter;
}

void Filter::ApplyFilter(GLfloat* pafSampleBuffer_)
{
    // Filtering is based on an LPF prototype,
    // so the LPF is always generated. 

    switch (eMyFilterType)
    {
    case LOWPASS:
        // Build low pass filter
        GenLPF(); 
        // Convolve sample buffer with LPF filter taps. 
        Convolve(pafSampleBuffer_);
        break;

    case HIGHPASS:
        // Build low pass filter prototype
        GenLPF();
        // Spectral inversion applied to LPF prototype to get HPF. 
        LowToHigh(); 
        // Convolve HPF filter taps with sample buffer. 
        Convolve(pafSampleBuffer_);
        break; 
        
    // !TODO: Bandpass/Bandstop not currently implemented. 
    // Right now the filter will silently return to the SpectrumDSP class. 
    case BANDPASS:
        break; 

    case BANDSTOP:
        break; 
    }
}

void Filter::GenLPF() 
{

    GLfloat fFilterNorm = 0;
    float fEps = 0.0001;
    GLfloat fNormCutOff = stMyDSPInit.fCutOffHz_LPF / ((GLfloat)iMySampleRate);

    for (int k = 0; k < (int) fMyFilterLength/5; ++k) {
        GLfloat fsincArg = 2.0f * 3.14159f * fNormCutOff * ((GLfloat)k - (((GLfloat)fMyFilterLength - 1.0f) / 2.0f));
        GLfloat fcosArg = 2.0f * 3.14159f * (GLfloat)k / ((GLfloat)fMyFilterLength - 1.0f);
        if (abs(fsincArg) < fEps) 
        { 
            pafMyFilterTaps[k] = 0.42f - 0.5f * cosf(fcosArg) + 0.08f * cosf(2 * fcosArg); 
        }
        else 
        { 
            pafMyFilterTaps[k] = (sinf(fsincArg) / (fsincArg)) * (0.42f - 0.5f * cosf(fcosArg) + 0.08f * cosf(2 * fcosArg));
        }
    }
    for (int k = 0; k < fMyFilterLength; ++k) { fFilterNorm += pafMyFilterTaps[k]; }
    for (int k = 0; k < fMyFilterLength; ++k) { pafMyFilterTaps[k] /= fFilterNorm; }
}

void Filter::LowToHigh() {
    for (int k = 0; k < fMyFilterLength; ++k) { pafMyFilterTaps[k] *= -1; }
    pafMyFilterTaps[(fMyFilterLength - 1) / 2] += 1;
}



void Filter::Convolve(GLfloat* pafSampleBuffer_) 
{

    // Only return the first BufferSize samples of convolution
    // The full convolution would be size BufferSize + FilterLength - 1 

    for (int k = 0; k < iMyBufferSize; ++k) {

        int kmn = (k >= iMyBufferSize - 1) ? k - (iMyBufferSize - 1) : 0;
        int kmx = (k < fMyFilterLength - 1) ? k : fMyFilterLength - 1;

        for (int j = kmn; j < kmx; ++j) {
            pafSampleBuffer_[k] += pafMyFilterTaps[j] * pafSampleBuffer_[k - j];
        }
    }

    return;
}