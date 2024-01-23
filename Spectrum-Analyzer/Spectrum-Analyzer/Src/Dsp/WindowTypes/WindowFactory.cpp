#include "WindowFactory.hpp"
#include "HammingWindow.hpp"
#include "BarletteWindow.hpp"
#include "BlackmanWindow.hpp"
#include "RectangularWindow.hpp"

WindowBase* WindowFactory::Instantiate(WindowTypeEnum eWindowType_, int iBufferSize_)
{
   switch(eWindowType_)
   {
      case BARLETTE_WINDOW:
         return new BarletteWindow(iBufferSize_); 

      case HAMMING_WINDOW:
         return new HammingWindow(iBufferSize_); 

      case BLACKMAN_WINDOW:
         return new BlackmanWindow(iBufferSize_);

      case RECTANGULAR_WINDOW:
         return new RectangularWindow(iBufferSize_);

      default:
         break;

   }

   return new RectangularWindow(iBufferSize_); 
}
