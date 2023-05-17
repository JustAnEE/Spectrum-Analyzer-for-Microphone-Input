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
   TaskIDEnum  eConsumerTask;
   int iActualDataSize;
   unsigned char aucTaskData[MAX_QUEUE_ENTRY_SIZE];

}TaskQueueData;

typedef struct ProviderStruct
{
   TaskIDEnum eProviderTask; 
   PacketIDEnum ePackedProvided; 
}ProviderStruct;

typedef struct ConsumerStruct
{
   TaskIDEnum eConsumerTask;
   PacketIDEnum ePacketConsumed;
};

typedef struct ProviderConsumerStruct
{
   // !TODO: A better way to determine number of providers
   // and consumers, preferably at compile time, for now 
   // hardcoded.
   ProviderStruct astProviders[NUM_TASKS];
   ConsumerStruct astConsumers[NUM_TASKS];
}ProviderConsumerStruct;

typedef struct TheTaskStruct
{  
   int iSizeOfProvidedData;
   ProviderConsumerStruct stProvideConsumeRelationships;
   
}TheTaskStruct;

typedef struct ConsumerRequestHistory
{
   TaskIDEnum eConsumerID;
   ConsumerStatusEnum eConsumerStatus; 
}ConsumerRequestHistory;

// Note this table is mutable. It is a table of most recent consumer history for all packets.
// Initialized on startup such that all consumers are waiting for data. This table will have to 
// change if any task produces more than one packet in the future. 
#define NUM_HISTORY_ENTRIES (2) // History entries num hardcoded for now 
static ConsumerRequestHistory astConsumerHistoryTable[] =
{
   {SPECTRUMDSPTASK, CONSUMER_STILL_WAITING_FOR_DATA},
   {MVCTASK,         CONSUMER_STILL_WAITING_FOR_DATA}
};

// This task table keeps track of which Tasks produce what data for consumption. If a Task attempts to request a 
// packet it is not registered for consumption for, the MessageController will assert. All tasks need to have their 
// relationships preconfigured here. 
static const TheTaskStruct astTaskTable[] = {

   /// Element Layout
   // SIZE
   //   PROVIDER, PACKETNAME
   //   CONSUMER, PACKETNAME
   // !TODO: Pretty cursed that we need the consumer to identify the packet as well,
   // !make this work properly eventually. 

   // It's possible the spectrum dsp output will go to multiple tasks 
   // in the future. 
   {
      sizeof(SpectrumOutputCL),
      { 
         {SPECTRUMDSPTASK, SPECTRUM_OUTPUT_PACKET},
         {MVCTASK, SPECTRUM_OUTPUT_PACKET}
      }
   },
   {
      sizeof(SampleBufferCL),
      {
         {MICINPUTTASK, SAMPLE_BUFFER_PACKET},
         {SPECTRUMDSPTASK, SAMPLE_BUFFER_PACKET}
      }
   }
};

#endif 

