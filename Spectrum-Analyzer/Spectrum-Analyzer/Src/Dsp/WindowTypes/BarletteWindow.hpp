#ifndef BARLETTEWINDOW_HPP
#define BARLETTEWINDOW_HPP

#include "WindowBase.hpp"

class BarletteWindow : public WindowBase
{
   public:
      BarletteWindow(int iBufferSize_);
      ~BarletteWindow() = default;

   private:
      void InstantiateWindow() override;
};


#endif 
