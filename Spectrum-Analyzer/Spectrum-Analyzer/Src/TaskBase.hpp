#ifndef TASKBASE_HPP
#define TASKBASE_HPP

#include "MessageController.hpp"

class TaskBase
{
public:

   TaskBase(Mutex& refMutex_, CondVar& refCondVar_, TaskQueue& refTaskQueue_);

   // All tasks must initialize themselves before their main is called. 
   virtual void InitializeTask() = 0;

   // All tasks must have a main function. 
   virtual void Main() = 0;

private:

protected:
   
   Mutex* pMutexMyTaskMemory;
   CondVar* pcvMyTaskConditionVariable;
   TaskQueue* pqMyTaskQueue;
};


#endif // TASKBASE_HPP