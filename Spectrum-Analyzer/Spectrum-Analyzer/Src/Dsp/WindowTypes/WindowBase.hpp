#ifndef WINDOW_BASE_HPP
#define WINDOW_BASE_HPP

class WindowBase
{

   public:
      WindowBase(int iBufferSize_);
      virtual ~WindowBase() = default; 
      
   public: 
      void ApplyWindow(float* pafData_);

   protected:
      virtual void InstantiateWindow() = 0; 

   protected:
      float* pafMyWindow; 
      int iMyBufferSize;

};

#endif 
