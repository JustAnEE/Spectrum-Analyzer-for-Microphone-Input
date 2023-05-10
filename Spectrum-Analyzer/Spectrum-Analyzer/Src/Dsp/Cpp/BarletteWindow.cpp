#include "../Hpp/BarletteWindow.hpp"

BarletteWindow::BarletteWindow()
{
   GenerateWindow();
}

void BarletteWindow::GenerateWindow()
{
   for (int k = 0; k < SAMPLE_BUFFER_SIZE; k++)
   {
      aufMyWindow[k] = (2.0 / ((GLfloat)(SAMPLE_BUFFER_SIZE - 1))) * (((GLfloat)(SAMPLE_BUFFER_SIZE - 1)) / 
         2.0 - (GLfloat)abs((GLfloat)k - (((GLfloat)SAMPLE_BUFFER_SIZE - 1)) / 2.0));
   }
}