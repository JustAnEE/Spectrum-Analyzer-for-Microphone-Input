#ifndef MICINPUTTASK_HPP
#define MICINPUTTASK_HPP

#include "../TaskBase.hpp"
#include "Hpp/MicInput.hpp"
#include "../Packets/Hpp/SampleBufferPacket.hpp"
#include "../MessageController.hpp"

class MicInputTask : public TaskBase
{
   friend class MessageController<SampleBufferCL, MicInputTask>;

public:
   MicInputTask(Mutex& refMutex_, CondVar& refCondVar_, TaskQueue& refTaskQueue_);

public:
   void InitializeTask();
   void Main();

private:
   void GenerateMicData();
   void PublishSampleBufferPacket(std::shared_ptr<SampleBufferCL> pclSampleBufferSP_);

private:

   MessageController<std::shared_ptr<SampleBufferCL>, MicInputTask>* pclMessageController;

   MicInput* pclMyMicInput;
   bool bMyFirstRun;

};
#endif // MICINPUTTASK_HPP