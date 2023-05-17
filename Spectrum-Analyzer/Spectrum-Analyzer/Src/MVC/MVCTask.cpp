#include "MVCTask.hpp"

MVCTask::MVCTask(Mutex& refMutex_, CondVar& refCondVar_, TaskQueue& refTaskQueue_) : 
   TaskBase(refMutex_, refCondVar_, refTaskQueue_)
{
   return; 
}

void MVCTask::Main()
{
   std::shared_ptr<SpectrumOutputCL> pclSpectrumOutput = nullptr;

   while (true)
   {
      pclMVCController->RequestPacket(pclSpectrumOutput);
      if (pclSpectrumOutput != nullptr)
      {

      }
   }
}