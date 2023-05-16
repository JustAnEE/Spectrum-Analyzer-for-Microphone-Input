#ifndef TASKBASE_HPP
#define TASKBASE_HPP

#include <queue>
#include <mutex>
#include <memory>
#include "TaskStruct.h"

typedef std::mutex Mutex;
typedef std::condition_variable CondVar;
typedef std::queue<TaskQueueData> TaskQueue;

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


// Method which starts the task when passing the method into threads 
void TaskStarter(TaskBase* pclTask_)
{
   pclTask_->Main();
   return;
}


#endif // TASKBASE_HPP