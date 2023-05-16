#ifndef TYPES_H
#define TYPES_H
#include "enums.h"

typedef struct DSPInitStruct
{
    SpectrumTypeEnum eSpectrumOutput;
    WindowTypeEnum eWindow;
    FilterTypeEnum eFilter;
    bool bIsNormalized;
    bool bDetrend;

}DSPInitStruct;

#endif 