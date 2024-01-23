#include "RectangularWindow.hpp"

RectangularWindow::RectangularWindow(int iBufferSize_)
   : WindowBase(iBufferSize_)
{
   InstantiateWindow();
   return;
}


void RectangularWindow::InstantiateWindow()
{
   for (int k = 0; k < iMyBufferSize; k++)
   {
      pafMyWindow[k] = 1; 
   }
   return; 
}
