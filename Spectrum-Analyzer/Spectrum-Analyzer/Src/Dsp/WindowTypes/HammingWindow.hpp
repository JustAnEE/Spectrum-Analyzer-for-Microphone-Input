#ifndef HAMMING_WINDOW_HPP
#define HAMMING_WINDOW_HPP

#include "WindowBase.hpp"

class HammingWindow : public WindowBase
{
   public:
      HammingWindow(int iBufferSize_);
      ~HammingWindow() = default; 

   private:
      void InstantiateWindow() override;

};



#endif 
