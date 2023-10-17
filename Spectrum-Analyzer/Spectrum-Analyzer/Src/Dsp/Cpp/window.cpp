#include "../Hpp/window.hpp"
#include <math.h>
#include <new>

WindowBase::WindowBase()
{
   bMyIsWindowConfigured = false; 
   ulMyWindowLength = 0;
   pafMyWindow = nullptr; 

   return; 
}

WindowBase::~WindowBase()
{
   if (pafMyWindow != nullptr)
   {
      delete pafMyWindow;
   }
}

void WindowBase::CreateEmptyWindow(const unsigned int ulWindowLength_)
{
   ulMyWindowLength = ulWindowLength_;
   pafMyWindow = new GLfloat[ulMyWindowLength];
   return; 
}

//!TODO: Innacurate window, if the window length is selected to be less than the input data
//! the rest of the input data should be zeroed. Current implementation is that the window is 
//! always the same length of the data, fix later. 
void WindowBase::ApplyWindow(GLfloat* pafDataToWindow_)
{

   // Silent fail 
   if (!bMyIsWindowConfigured)
   {
      return; 
   }

   for (unsigned int ulIdx = 0; ulIdx < ulMyWindowLength; ulIdx++)
   {
      //pafDataToWindow_[ulIdx] *= pafMyWindow[ulIdx];
   }

   return; 
}

bool WindowBase::IsWindowReady()
{
   return bMyIsWindowConfigured;
}


// -------------------- HAMMING WINDOW -------------------- //

void HammingWindow::InitWindow()
{
   // Silent fail if window is less than 0 or window is not allocated 
   if (ulMyWindowLength <= 0 || pafMyWindow == nullptr)
   {
      return; 
   }

   for (unsigned int ulIdx = 0; ulIdx < ulMyWindowLength; ulIdx++)
   {
      pafMyWindow[ulIdx] = 0.54 - 0.46 * cosf(2.0 * 3.14159 * (GLfloat)ulIdx / ((GLfloat)(ulMyWindowLength - 1)));
   }

   bMyIsWindowConfigured = true; 
   return; 
}

// -------------------- BLACKMAN WINDOW -------------------- //

void BlackmanWindow::InitWindow()
{
   if (ulMyWindowLength <= 0 || pafMyWindow == nullptr)
   {
      return; 
   }

   for (unsigned int ulIdx = 0; ulIdx < ulMyWindowLength; ulIdx++)
   {
      pafMyWindow[ulIdx] = 0.42 - 0.5 * cosf(2.0 * 3.14159 * (GLfloat)ulIdx / ((GLfloat)(ulMyWindowLength - 1)))
         + 0.08 * cos(4.0 * 3.14159 * (GLfloat)ulIdx / ((GLfloat)(ulMyWindowLength - 1)));
   }
   return; 
}

// -------------------- BARLETTE WINDOW -------------------- //

void BarletteWindow::InitWindow()
{
   if (ulMyWindowLength <= 0 || pafMyWindow == nullptr)
   {
      return; 
   }

   for (unsigned int ulIdx = 0; ulIdx < ulMyWindowLength; ulIdx++)
   {
      pafMyWindow[ulIdx] = (2.0 / ((GLfloat)(ulMyWindowLength - 1))) * (((GLfloat)(ulMyWindowLength - 1))
         / 2.0 - (GLfloat)abs((GLfloat)ulIdx - (((GLfloat)ulMyWindowLength - 1)) / 2.0));
   }
   return; 
}

// Factory 
WindowBase* WindowInstantiator::InstantiateWindow(WindowTypeEnum eWindowType_)
{
   switch (eWindowType_)
   {
      //!TODO: Fix this boof 
      case RECTANGULAR_WINDOW:
         return nullptr;

      case HAMMING_WINDOW:
         return new HammingWindow();

      case BLACKMAN_WINDOW:
         return new BlackmanWindow();

      case BARLETTE_WINDOW:
         return new BarletteWindow();
      
      default:
         return nullptr;
   }
}
