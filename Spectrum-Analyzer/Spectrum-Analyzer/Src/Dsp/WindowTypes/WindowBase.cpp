#include "WindowBase.hpp"


WindowBase::WindowBase(int iBufferSize_)
{
   pafMyWindow = new float[iBufferSize_];
   iMyBufferSize = iBufferSize_;
   return; 
}

void WindowBase::ApplyWindow(float* pafData_)
{
   for (int k = 0; k < iMyBufferSize; k++)
   {
      pafData_[k] *= pafMyWindow[k];
   }

   return; 
}
