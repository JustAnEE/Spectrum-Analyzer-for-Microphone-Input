#ifndef TASKSTRUCT_H
#define TASKSTRUCT_H

// Forward Declarations
#include "Packets/Hpp/SampleBufferPacket.hpp"
#include "Packets/Hpp/SpectrumOutputPacket.hpp"

// 1000 bytes for now 
#define MAX_QUEUE_ENTRY_SIZE (1000)
// 30000 bytes for now 
#define MAX_QUEUE_LENGTH (30000)
// Only three tasks at the moment, SpectrumDSP, MVC, MicData
#define NUM_TASKS (3)

#define DSP_TASK_HANDLE (0xFFFF)
#define MVC_TASK_HANDLE (0xFFFC)
#define MICDATA_TASK_HANDLE (0xFFFA)


typedef int QueueHandle;

typedef struct TaskQueueData
{
   TaskIDEnum  eProviderTaskID;
   int iActualDataSize;
   unsigned char aucTaskData[MAX_QUEUE_ENTRY_SIZE];

}TaskQueueData;


typedef struct TheTaskStruct
{
   TaskIDEnum eConsumerTaskID; 
   TaskQueueData astTaskQueueEntries[NUM_TASKS];
}TheTaskStruct;


static const TheTaskStruct astTaskTable[] = {

   // Consumer of packet
   {SPECTRUMDSPTASK,
      {
         // Provider of packet
         MICINPUTTASK,
         // Size of packet 
         sizeof(SampleBufferCL)
      }
   },
   // Consumer of packet
   {MVCTASK,
      {
         // Provider of packet 
         SPECTRUMDSPTASK,
         // Size of packet 
         sizeof(SpectrumOutputCL)
      }
   }
};

#endif 

