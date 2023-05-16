#ifndef MVCTASK_HPP
#define MVCTASK_HPP

#include "../TaskBase.hpp"
#include "../MessageController.hpp"

class MVCTask : public TaskBase
{
public:
   MVCTask(Mutex& refMutex_, CondVar& refCondVar_, TaskQueue& refTaskQueue_);

public:
   void InitializeTask() {}
   void Main() {}

private:

};
#endif 