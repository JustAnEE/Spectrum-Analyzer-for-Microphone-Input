#include "MicInputTask.hpp"

MicInputTask::MicInputTask(Mutex& refMutex_, CondVar& refCondVar_, TaskQueue& refTaskQueue_) :
   TaskBase(refMutex_, refCondVar_, refTaskQueue_)
{
   pclMyMicInput = new MicInput(NUM_CHANNELS, SAMPLE_RATE, BITS_PER_SAMPLE);
   pclMessageController = new MessageController<std::shared_ptr<SampleBufferCL>, MicInputTask>(*this);
   InitializeTask();

   return; 
}

void MicInputTask::InitializeTask()
{
   bMyFirstRun = TRUE; 

}

void MicInputTask::Main()
{

   while (true)
   {
      GenerateMicData();
   }

   

}

void MicInputTask::GenerateMicData()
{

   // Instantiate a SP packet for mic data 
   std::shared_ptr<SampleBufferCL> pclSampleBufferSP = std::make_shared<SampleBufferCL>();;


   // Initialize
   pclMyMicInput->readMicInput(pclSampleBufferSP->GetpacRawMicData(), pclMyMicInput->getBlockAlign());
   pclSampleBufferSP->ConvertRawDataToFloat(pclMyMicInput->getBlockAlign());

   // Publish the packet
   PublishSampleBufferPacket(pclSampleBufferSP);
}

void MicInputTask::PublishSampleBufferPacket(std::shared_ptr<SampleBufferCL> pclSampleBufferSP_)
{
   pclMessageController->PublishMessage(pclSampleBufferSP_);
}