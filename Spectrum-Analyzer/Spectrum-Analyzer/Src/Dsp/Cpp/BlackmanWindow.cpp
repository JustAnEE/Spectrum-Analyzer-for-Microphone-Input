#include "../Hpp/BlackmanWindow.hpp"

BlackmanWindow::BlackmanWindow()
{
   GenerateWindow();
}

void BlackmanWindow::GenerateWindow()
{
   for (int k = 0; k < SAMPLE_BUFFER_SIZE; k++)
   {
      aufMyWindow[k] = 0.42 - 0.5 * cosf(2.0 * 3.14159 * (GLfloat)k / ((GLfloat)(SAMPLE_BUFFER_SIZE - 1))) + 0.08 * cos(4.0 * 3.14159 * (GLfloat)k / ((GLfloat)(SAMPLE_BUFFER_SIZE - 1)));
   }
}