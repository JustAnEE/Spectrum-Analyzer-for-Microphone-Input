#ifndef MVCTASK_HPP
#define MVCTASK_HPP

#include "../TaskBase.hpp"
#include "../MessageController.hpp"
#include "../Packets/Hpp/SpectrumOutputPacket.hpp"

class MVCTask : public TaskBase
{
public:
   MVCTask(Mutex& refMutex_, CondVar& refCondVar_, TaskQueue& refTaskQueue_);

public:
   void InitializeTask() {}
   void Main();

private:
   MessageController<std::shared_ptr<SpectrumOutputCL>,  MVCTask>* pclMVCController;
};
#endif 