#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include "../Hpp/SpectrumTypes.hpp"


class WindowBase
{
   // Non Copyable 
   WindowBase(const WindowBase& clRHS_) = delete;
   WindowBase& operator=(const WindowBase& clRHS_) = delete; 

   public:
      WindowBase();
      virtual ~WindowBase();

   public:
      void CreateEmptyWindow(const unsigned int ulWindowLength_);
      void ApplyWindow(GLfloat* pafDataToWindow_);
      virtual void InitWindow() = 0;

      bool IsWindowReady();

   protected:
      unsigned int ulMyWindowLength; 
      bool bMyIsWindowConfigured;
      GLfloat* pafMyWindow; 
};


class HammingWindow : public WindowBase
{
   public:
      HammingWindow() = default;
      ~HammingWindow() = default;

   public:
      void InitWindow();

};


class BlackmanWindow : public WindowBase
{
   public:
      BlackmanWindow() = default;
      ~BlackmanWindow() = default;

   public:
      void InitWindow();
};


class BarletteWindow : public WindowBase
{
   public:
      BarletteWindow() = default;
      ~BarletteWindow() = default;

   public:
      void InitWindow();
};


class WindowInstantiator
{
   WindowInstantiator() = delete;

   public:
      static WindowBase* InstantiateWindow(WindowTypeEnum eWindowType_);
};

#endif //Window_HPP