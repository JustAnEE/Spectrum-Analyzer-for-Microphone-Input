#include "../Hpp/HammingWindow.hpp"

HammingWindow::HammingWindow()
{
   GenerateWindow();
}

void HammingWindow::GenerateWindow()
{
   for (int k = 0; k < SAMPLE_BUFFER_SIZE; k++)
   {
      aufMyWindow[k] = 0.54 - 0.46 * cosf(2.0 * 3.14159 * (GLfloat)k / ((GLfloat)(SAMPLE_BUFFER_SIZE - 1)));
   }
}