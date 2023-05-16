#include "TaskBase.hpp"

TaskBase::TaskBase(Mutex& refMutex_, CondVar& refCondVar_, TaskQueue& refTaskQueue_)
{
   pMutexMyTaskMemory = &refMutex_;
   pcvMyTaskConditionVariable = &refCondVar_;
   pqMyTaskQueue = &refTaskQueue_;
   InitializeTask();
}
