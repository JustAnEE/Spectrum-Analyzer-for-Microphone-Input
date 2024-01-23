#ifndef WINDOW_FACTORY_HPP
#define WINDOW_FACTORY_HPP

#include "WindowBase.hpp"
#include "../Hpp/SpectrumTypes.hpp"

class WindowFactory
{
   WindowFactory() = delete; 
   WindowFactory(const WindowFactory&) = delete; 
   WindowFactory& operator = (const WindowFactory&) = delete; 

   public:
      static WindowBase* Instantiate(WindowTypeEnum eWindowType_, int iBufferSize_);

};


#endif 
