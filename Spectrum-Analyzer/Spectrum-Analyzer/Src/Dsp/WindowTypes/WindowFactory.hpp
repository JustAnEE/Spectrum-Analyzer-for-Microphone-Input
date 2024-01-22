#ifndef WINDOW_FACTORY_HPP
#define WINDOW_FACTORY_HPP

#include "WindowBase.hpp"
#include "../Hpp/SpectrumTypes.hpp"

// Forward declarations
class RectangularWindow;
class HammingWindow;
class BlackmanWindow; 
class BarletteWindow; 

class WindowFactory
{
   WindowFactory() = delete; 
   WindowFactory(const WindowFactory&) = delete; 
   WindowFactory& operator = (const WindowFactory&) = delete; 

   public:
      static WindowBase* Instantiate(WindowTypeEnum eWindowType_, int iBufferSize_);

};


#endif 
