#ifndef BLACKMAN_WINDOW_HPP
#define BLACKMAN_WINDOW_HPP

#include "WindowBase.hpp"


class BlackmanWindow : public WindowBase
{
   public:
      BlackmanWindow(int iBufferSize_);
      ~BlackmanWindow() = default;

   private:
      void InstantiateWindow() override;

};
      



#endif 
