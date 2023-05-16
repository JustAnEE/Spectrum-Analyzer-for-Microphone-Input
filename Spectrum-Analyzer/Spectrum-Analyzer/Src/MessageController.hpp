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

public:
   MessageController(TaskType pclTask);

public:

   void RegisterProvider(TaskIDEnum eTaskID_);
   void RegisterConsumer(TaskIDEnum eCurrentTaskID_, TaskIDEnum eProviderTask_);

   MessageStatusEnum PublishMessage(PacketType pclPacket);

   void GetMemoryFromFriends(TaskType pclTask);

   void RequestPacket(PacketType pclEmptyPacket);

private:
   TaskQueueData stMyQueueData;

   Mutex* pMutexMyTaskMemory;
   CondVar* pcvMyTaskConditionVariable;
   TaskQueue* pqMyTaskQueue;
};


template<typename PacketType, typename TaskType>
inline MessageController<PacketType, TaskType>::MessageController(TaskType pclTask)
{
   //! TODO: Pass in the pointers, stop being chump :) 

   GetMemoryFromFriends(pclTask);
}

template<typename PacketType, typename TaskType>
inline void MessageController<PacketType, TaskType>::RegisterProvider(TaskIDEnum eTaskID_)
{
   //! TODO: Make sure that the task registering for the packet is allowed to consume the packet. 
   stMyQueueData.eTaskID = eTaskID_;

}

template<typename PacketType, typename TaskType>
inline void MessageController<PacketType, TaskType>::RegisterConsumer(TaskIDEnum eCurrentTaskID_, TaskIDEnum eProviderTask_)
{
   if (astTaskTable[eCurrentTaskID_].eConsumerTaskID = eCurrentTaskID_)
   {

   }

   _ASSERT_EXPR(true, "Cannot register for this packet \n");
}

template<typename PacketType, typename TaskType>
inline MessageStatusEnum MessageController<PacketType, TaskType>::PublishMessage(PacketType pclPacket)
{
   // Copy the packet into the queue data
   memcpy(stMyQueueData.aucTaskData, pclPacket);

   // Lock the mutex 
   std::unique_lock<Mutex>(*pMutexMyTaskMemory).lock();
   // Place data onto queue
   pqMyTaskQueue->push(&stMyQueueData);

   // Notify all the threads waiting for this DP. 
   pcvMyTaskConditionVariable->notify_all();
   return;
}

template<typename PacketType, typename TaskType>
inline void MessageController<PacketType, TaskType>::GetMemoryFromFriends(TaskType pclTask)
{
   //! TODO: Don't be a chump, pass in the function pointer to the method we wish to call in the 
   //! class. 

   // We can do this because we're friends :) 
   pMutexMyTaskMemory = pclTask->pMutexMyTaskMemory;
   pcvMyTaskConditionVariable = pclTask->pcvMyTaskConditionVariable;
   pqMyTaskQueue = pclTask->pqMyTaskQueue;
   return; 
}

template<typename PacketType, typename TaskType>
inline void MessageController<PacketType, TaskType>::RequestPacket(PacketType pclEmptyPacket)
{
   _ASSERT_EXPR(pclEmptyPacket == nullptr, "Cannot request a packet from a non null pointer. \n");

   bool bAwakeCondition = (pqMyTaskQueue->front().eTaskID == stMyQueueData.eTaskID);
   pcvMyTaskConditionVariable->wait(std::unique_lock<Mutex>(*pMutexMyTaskMemory), bAwakeCondition);

   // Create the packet
   pclEmptyPacket = std::make_shared<PacketType>();

   // If the thread is awake, grab the data packet from the struct. 
   memcpy(pclEmptyPacket, &stMyQueueData, sizeof(pclEmptyPacket));
   return;
}

#endif 