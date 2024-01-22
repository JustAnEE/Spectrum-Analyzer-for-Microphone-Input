#ifndef RECTANGULARWINDOW_HPP
#define RECTANGULARWINDOW_HPP

#include "WindowBase.hpp"


class RectangularWindow : public WindowBase 
{
   public:
      RectangularWindow(int iBufferSize_);
      ~RectangularWindow() = default;
   
   private:
      void InstantiateWindow() override;

};



#endif 
