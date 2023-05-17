#ifndef MESSAGECONTROLLER_HPP
#define MESSAGECONTROLLER_HPP

#include "TaskStruct.h"
#include "Utils/enums.h"
#include <queue>
#include <mutex>
#include <memory>
#include <cstring>

typedef std::mutex Mutex;
typedef std::condition_variable CondVar;
typedef std::queue<TaskQueueData> TaskQueue;

// Forward declarations 
class SpectrumDSPTask;
class MVCTask;
class MicInputTask;


// The message controller handles the queue and converts published messages
// into the format required for the queue.  
template<typename PacketType, typename TaskType>
class MessageController
{

// Constructor takes (*this) from the task which is hosting it. 
public:
   MessageController(TaskType pclTask);

// Public operations 
public:

   void RegisterProvider(TaskIDEnum eTaskID_);
   void RegisterConsumer(TaskIDEnum eCurrentConsumerTaskID_);

   MessageStatusEnum PublishMessage(PacketType pclPacket);
   void GetMemoryFromFriends(TaskType pclTask);
   void RequestPacket(PacketType pclEmptyPacket);

// Private operations 
private:
   ConsumerStatusEnum CheckOnOtherConsumers();
   void UpdateConsumerStatus(TaskIDEnum eConsumerTask);
   void ResetConsumersStatus();

// Private attributes
private:

   //! This struct contains the raw memory and size of an element to be added/extracted 
   //! to the queue. 
   TaskQueueData stMyQueueData;

   //! This is a mutex for locking access to the memory pool. 
   Mutex* pMutexMyTaskMemory;

   //! This condition variable allows for automagic sleep/wake up 
   CondVar* pcvMyTaskConditionVariable;

   //! This is a pointer to the shared queue memory pool between tasks. 
   TaskQueue* pqMyTaskQueue;
};


template<typename PacketType, typename TaskType>
inline MessageController<PacketType, TaskType>::MessageController(TaskType pclTask)
{
   GetMemoryFromFriends(pclTask);
}

template<typename PacketType, typename TaskType>
inline void MessageController<PacketType, TaskType>::RegisterProvider(TaskIDEnum eTaskID_)
{
   // Check to make sure the task can actually provide the packet. 
   if (astTaskTable[eTaskID_].stProvideConsumeRelationships.astProviders[eTaskID_].eProviderTask == eTaskID_)
      return; 

   _ASSERT_EXPR(true, "Cannot register as a provider for this packet \n");
}

template<typename PacketType, typename TaskType>
inline void MessageController<PacketType, TaskType>::RegisterConsumer(TaskIDEnum eCurrentConsumerTaskID_)
{
   if (astTaskTable[eCurrentConsumerTaskID_].stProvideConsumeRelationships.astConsumers[eCurrentConsumerTaskID_] = eCurrentConsumerTaskID_)
   {
      stMyQueueData.eConsumerTask = eCurrentConsumerTaskID_;
      return; 
   }

   _ASSERT_EXPR(true, "Cannot register as a consumer for this packet \n");
}

template<typename PacketType, typename TaskType>
inline MessageStatusEnum MessageController<PacketType, TaskType>::PublishMessage(PacketType pclPacket)
{
   // Copy the packet into the queue data
   stMyQueueData.iActualDataSize = sizeof(pclPacket.get());
   memcpy(stMyQueueData.aucTaskData, pclPacket.get(), stMyQueueData.iActualDataSize);

   // Lock the mutex 
   std::unique_lock<Mutex>(*pMutexMyTaskMemory).lock();
   // Place data onto queue
   pqMyTaskQueue->push(stMyQueueData);

   // Notify all the threads waiting for this data 
   pcvMyTaskConditionVariable->notify_all();
   return MESSAGE_SUCCESS;
}

template<typename PacketType, typename TaskType>
inline void MessageController<PacketType, TaskType>::GetMemoryFromFriends(TaskType pclTask)
{
   //! TODO: Don't be a chump, pass in the function pointer to the method we wish to call in the 
   //! class so we don't potentially dereference a random type. 
   pMutexMyTaskMemory = pclTask->pMutexMyTaskMemory;
   pcvMyTaskConditionVariable = pclTask->pcvMyTaskConditionVariable;
   pqMyTaskQueue = pclTask->pqMyTaskQueue;
   return;
}

template<typename PacketType, typename TaskType>
inline void MessageController<PacketType, TaskType>::RequestPacket(PacketType pclEmptyPacket)
{
   _ASSERT_EXPR(pclEmptyPacket != nullptr, "Cannot request a packet from a non null pointer. \n");

   // If the thread randomly awakes, check the front of the queue for the relevant task ID. 
   bool bAwakeCondition = (pqMyTaskQueue->front().eTaskID == stMyQueueData.eTaskID);
   pcvMyTaskConditionVariable->wait(std::unique_lock<Mutex>(*pMutexMyTaskMemory), bAwakeCondition);

   // Create the packet
   pclEmptyPacket = std::make_shared<PacketType>();

   // If the thread is awake, grab the data packet from the struct. 
   TaskQueueData stTempQueueData;
   stTempQueueData = pqMyTaskQueue->front();
   memcpy(pclEmptyPacket.get(), stTempQueueData.aucTaskData, stTempQueueData.iActualDataSize);

   UpdateConsumerStatus(stMyQueueData.eConsumerTask);

   // Check on other consumers, if the other consumers have already read, we can pop. 
   if (CheckOnOtherConsumers() == ALL_OTHER_CONSUMERS_USED_PACKET)
   {
      pqMyTaskQueue->pop();
      ResetConsumersStatus();
   }

   return;
}

template<typename PacketType, typename TaskType>
inline ConsumerStatusEnum MessageController<PacketType, TaskType>::CheckOnOtherConsumers()
{
   for (int iEntry = 0; iEntry < NUM_HISTORY_ENTRIES; iEntry++)
   {
      if (astConsumerHistoryTable[iEntry].eConsumerID == stMyQueueData.eConsumerTask)
      {
         if (astConsumerHistoryTable[iEntry].eConsumerStatus == CONSUMER_STILL_WAITING_FOR_DATA)
            return SOME_CONSUMERS_NEED_PACKET;
      }
   }
   return ALL_OTHER_CONSUMERS_USED_PACKET;
}

template<typename PacketType, typename TaskType>
inline void MessageController<PacketType, TaskType>::UpdateConsumerStatus(TaskIDEnum eConsumerTask_)
{
   for (int iEntry = 0; iEntry < NUM_TASKS; iEntry++)
   {
      for (int iConsumers = 0; iConsumers < NUM_TASKS; iConsumers++)
      {
         if (astTaskTable[iEntry].stProvideConsumeRelationships.astConsumers[iConsumers].eConsumerTask == eConsumerTask_)
         {
            astConsumerHistoryTable[iConsumers].eConsumerStatus = PACKET_CONSUMED_FOR_THIS_CONSUMER;
            return; 
         }
      }
   }
}

template<typename PacketType, typename TaskType>
inline void MessageController<PacketType, TaskType>::ResetConsumersStatus()
{
   for (int iConsumers = 0; iConsumers < NUM_TASKS; iConsumers++)
   {
      astConsumerHistoryTable[iConsumers].eConsumerStatus = CONSUMER_STILL_WAITING_FOR_DATA;
   }
}

#endif 