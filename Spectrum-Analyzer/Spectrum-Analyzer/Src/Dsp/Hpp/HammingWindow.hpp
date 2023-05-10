#ifndef HAMMINGWINDOW_HPP
#define HAMMINGWINDOW_HPP

#include "Window.hpp"

class HammingWindow : public Window
{
   
public:
   HammingWindow();

private:
   void GenerateWindow();
};

#endif 
