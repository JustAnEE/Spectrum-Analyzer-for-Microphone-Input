#include "BlackmanWindow.hpp"
#include "WindowBase.hpp"
#include <math.h>

BlackmanWindow::BlackmanWindow(int iBufferSize_)
   : WindowBase(iBufferSize_)
{
   InstantiateWindow();
   return; 
}


void BlackmanWindow::InstantiateWindow()
{
   for (int k = 0; k < iMyBufferSize; k++)
   {
      pafMyWindow[k] = 0.42f - 0.5f * cosf(2.0f * 3.14159f * (float)k / ((float)(iMyBufferSize - 1)))
         + 0.08f * cos(4.0 * 3.14159 * (float)k / ((float)(iMyBufferSize - 1)));
   }
   return; 
}
