#include "HammingWindow.hpp"
#include <math.h>

HammingWindow::HammingWindow(int iBufferSize_) 
   : WindowBase(iBufferSize_)
{
   InstantiateWindow();
   return; 
}

void HammingWindow::InstantiateWindow()
{
   for (int k = 0; k < iMyBufferSize; k++)
   {
      pafMyWindow[k] = 0.54 - 0.46 * cosf(2.0 * 3.14159 * (float)k / ((float)(iMyBufferSize - 1)));
   }
   
   return; 
}
