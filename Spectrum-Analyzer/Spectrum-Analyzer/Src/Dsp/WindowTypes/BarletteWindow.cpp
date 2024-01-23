#include "BarletteWindow.hpp"
#include <math.h>

BarletteWindow::BarletteWindow(int iBufferSize_)
   : WindowBase(iBufferSize_)
{
   InstantiateWindow();
   return; 
}

void BarletteWindow::InstantiateWindow()
{
   for (int k = 0; k < iMyBufferSize; k++)
   {
      pafMyWindow[k] = (2.0 / ((float)(iMyBufferSize - 1))) * (((float)(iMyBufferSize - 1)) / 2.0 
         - (float)fabs((float)k - (((float)iMyBufferSize - 1.0f)) / 2.0f));
   }

   return; 
}
