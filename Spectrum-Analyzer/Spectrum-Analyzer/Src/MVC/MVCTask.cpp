#include "MVCTask.hpp"

MVCTask::MVCTask(Mutex& refMutex_, CondVar& refCondVar_, TaskQueue& refTaskQueue_) : 
   TaskBase(refMutex_, refCondVar_, refTaskQueue_)
{
   return; 
}
