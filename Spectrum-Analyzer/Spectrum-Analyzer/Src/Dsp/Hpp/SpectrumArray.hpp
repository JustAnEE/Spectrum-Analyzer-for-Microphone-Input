#ifndef SPECTRUMARRAY_HPP
#define SPECTRUMARRAY_HPP

#include <cstring>
#include <math.h>
#include <crtdbg.h>

#define NUM_SAMPLES (1024 * 4)
/////////////////////////////////////////////////////////////////////////////
//
// !filename SpectrumArray.hpp
// !Brief
//  This template class implements a static array with overloaded arithmetic 
//  operators. 
//  1) + corresponds to element wise addition
//  2) - corresponds to element wise subtraction
//  3) * corresponds to element wise multiplication of two SpectrumArrays
//  4) += behaves like += but element wise 
//  5) -= behaves like -= but element wise 
//  6) ^ element wise exponentiation by an integer 
//  7) [] allows you to index into the array directly from the object
// 
//  //! TODO: Implement +, -, +=, -=, *, *= 
//   
//  //! Possible future TODO: Specialize the templates to handle cases of 
//  // operations of different types. Float * Int, etc. 
// 
/////////////////////////////////////////////////////////////////////////////
template<typename ArrayType>
class SpectrumArrayCL
{
    // Constructors 
public:

    //! This constructor initializes the array to zero
    SpectrumArrayCL();
    //! This constructor will initialize the array from a passed in array 
    SpectrumArrayCL(ArrayType* paSpectrumData);

    // Operators 
public:

    ArrayType& operator [] (const int iIndex_);

    // Overloaded plus operator. 
    template<typename ArrayType>
    friend SpectrumArrayCL<ArrayType> operator + (const SpectrumArrayCL<ArrayType>& clSpectrumArrayLeft_,
        const SpectrumArrayCL<ArrayType>& clSpectrumArrayRight_);

    // Overloaded subtraction operator 
    template<typename ArrayType>
    friend SpectrumArrayCL<ArrayType> operator - (const SpectrumArrayCL<ArrayType>& clSpectrumArrayLeft_,
        const SpectrumArrayCL<ArrayType>& clSpectrumArrayRight_);

    // Overloaded multiply operator 
    template<typename ArrayType>
    friend SpectrumArrayCL<ArrayType> operator * (const SpectrumArrayCL<ArrayType>& clSpectrumArrayLeft_,
        const SpectrumArrayCL<ArrayType>& clSpectrumArrayRight_);

    // Overloaded ^ operator (element wise power) 
    template<typename ArrayType>
    friend SpectrumArrayCL<ArrayType> operator ^ (const SpectrumArrayCL<ArrayType>& clSpectrumArrayLeft_, ArrayType iPower);

private:
    ArrayType aMySpectrumData[NUM_SAMPLES];

};

template<typename ArrayType>
inline ArrayType& SpectrumArrayCL<ArrayType>::operator [] (const int iIndex_)
{
    _ASSERT_EXPR(iIndex_ < 0, "Accessing memory not belonging to the array.");
    _ASSERT_EXPR(iIndex_ >= NUM_SAMPLES, "Accessing memory not belonging to the array.");

    return aMySpectrumData[iIndex_];
}

template<typename ArrayType>
SpectrumArrayCL<ArrayType>::SpectrumArrayCL()
{
    memset(aMySpectrumData, 0, NUM_SAMPLES * sizeof(ArrayType));
}

template<typename ArrayType>
SpectrumArrayCL<ArrayType>::SpectrumArrayCL(ArrayType* paSpectrumData)
{
    memcpy(aMySpectrumData, paSpectrumData, NUM_SAMPLES * sizeof(ArrayType));
}

template<typename ArrayType>
SpectrumArrayCL<ArrayType> operator+(const SpectrumArrayCL<ArrayType>& clSpectrumArrayLeft_,
    const SpectrumArrayCL<ArrayType>& clSpectrumArrayRight_)
{
    ArrayType aDataArrayTemp[NUM_SAMPLES] = {};

    for (int iIndex = 0; iIndex < NUM_SAMPLES; iIndex++)
    {
        aDataArrayTemp[iIndex] = clSpectrumArrayLeft_.aMySpectrumData[iIndex] + clSpectrumArrayRight_.aMySpectrumData[iIndex];
    }
    return aDataArrayTemp;
}

template<typename ArrayType>
SpectrumArrayCL<ArrayType> operator-(const SpectrumArrayCL<ArrayType>& clSpectrumArrayLeft_,
    const SpectrumArrayCL<ArrayType>& clSpectrumArrayRight_)
{
    ArrayType aDataArrayTemp[NUM_SAMPLES] = {};

    for (int iIndex = 0; iIndex < NUM_SAMPLES; iIndex++)
    {
        aDataArrayTemp[iIndex] = clSpectrumArrayLeft_.aMySpectrumData[iIndex] - clSpectrumArrayRight_.aMySpectrumData[iIndex];
    }
    return aDataArrayTemp;
}

template<typename ArrayType>
SpectrumArrayCL<ArrayType> operator*(const SpectrumArrayCL<ArrayType>& clSpectrumArrayLeft_,
    const SpectrumArrayCL<ArrayType>& clSpectrumArrayRight_)
{
    ArrayType aDataArrayTemp[NUM_SAMPLES] = {};

    for (int iIndex = 0; iIndex < NUM_SAMPLES; iIndex++)
    {
        aDataArrayTemp[iIndex] = clSpectrumArrayLeft_.aMySpectrumData[iIndex] * clSpectrumArrayRight_.aMySpectrumData[iIndex];
    }
    return aDataArrayTemp;
}

template<typename ArrayType>
SpectrumArrayCL<ArrayType> operator^(const SpectrumArrayCL<ArrayType>& clSpectrumArrayLeft_, ArrayType aPower)
{
    ArrayType aDataArrayTemp[NUM_SAMPLES] = {};

    for (int iIndex = 0; iIndex < NUM_SAMPLES; iIndex++)
    {
        aDataArrayTemp[iIndex] = powf(clSpectrumArrayLeft_.aMySpectrumData[iIndex], aPower);
    }
    return aDataArrayTemp;
}

#endif 
