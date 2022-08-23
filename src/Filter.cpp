#include "Filter.h"


Filter::Filter(int _sampleRate, int _bufferSize, float _cutOffHz, float _TBW) {
	
	sampleRate = _sampleRate;
	bufferSize = _bufferSize;
	cutOff = _cutOffHz / (float) _sampleRate;
    TBW = _TBW/ (float) _sampleRate;
    filterLength = (int) (4 / TBW);
    if (filterLength % 2 == 0) {filterLength += 1;}
    filter = (GLfloat*)calloc(filterLength, sizeof(GLfloat));

    genLPF(); 
   
}

Filter::~Filter() { free(filter); }

void Filter::genLPF() {

    GLfloat filterNorm = 0;
    float eps = 0.0001;

    for (int k = 0; k < filterLength; ++k) {
        GLfloat sincArg = 2.0f * 3.14159f * cutOff * ((GLfloat)k - (((GLfloat)filterLength - 1.0f) / 2.0f));
        GLfloat cosArg = 2.0f * 3.14159f * (GLfloat)k / ((GLfloat)filterLength - 1.0f);
        if (abs(sincArg) < eps) { filter[k] = 0.42f - 0.5f * cosf(cosArg) + 0.08f * cosf(2 * cosArg); }
        else { filter[k] = (sinf(sincArg) / (sincArg)) * (0.42f - 0.5f * cosf(cosArg) + 0.08f * cosf(2 * cosArg)); }
    }
    for (int k = 0; k < filterLength; ++k) { filterNorm += filter[k]; }
    for (int k = 0; k < filterLength; ++k) { filter[k] /= filterNorm; }
}

void Filter::lowToHigh() {
    for (int k = 0; k < filterLength; ++k) { filter[k] *= -1; }
    filter[(filterLength - 1) / 2] += 1;
}

GLfloat* Filter::convolve(GLfloat* in) {

    GLfloat* convOutput = (GLfloat*)calloc(bufferSize + filterLength - 1, sizeof(GLfloat));

    for (int k = 0; k < bufferSize + filterLength - 1; ++k) {

        int kmn = (k >= bufferSize - 1) ? k - (bufferSize - 1) : 0;
        int kmx = (k < filterLength - 1) ? k : filterLength - 1;

        for (int j = kmn; j < kmx; ++j) {
            convOutput[k] += filter[j] * in[k - j];
        }
    }

    return convOutput;
}