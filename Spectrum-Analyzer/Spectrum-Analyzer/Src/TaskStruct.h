#ifndef TASKSTRUCT_H
#define TASKSTRUCT_H

// 1000 bytes for now 
#define MAX_QUEUE_ENTRY_SIZE (1000)
// 30000 bytes for now 
#define MAX_QUEUE_LENGTH (30000)
// Only three tasks at the moment, SpectrumDSP, MVC, MicData
#define NUM_TASKS (3)

#define DSP_TASK_HANDLE (0xFFFF)
#define MVC_TASK_HANDLE (0xFFFC)
#define MICDATA_TASK_HANDLE (0xFFFA)

typedef enum TaskIDEnum
{
   SPECTRUMDSPTASK = 0,
   MVCTASK         = 1,
   MICINPUTTASK    = 2
}TaskIDEnum;

typedef int QueueHandle;

typedef struct TaskQueueData
{
   TaskIDEnum  eTaskID;
   unsigned char aucTaskData[MAX_QUEUE_ENTRY_SIZE];

}TaskQueueData;

typedef struct TheTaskStruct
{
   TaskQueueData astTaskQueueEntries[NUM_TASKS];
}TheTaskStruct;

#endif 

