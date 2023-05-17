#ifndef ENUMS_H
#define ENUMS_H

// Spectrum type enum used for specifying spectrum output.
typedef enum SpectrumTypeEnum
{
   MAGNITUDE_SPECTRUM = 0,
   DB_SPECTRUM        = 1,
   PSD_SPECTRUM       = 2,
   DBM_SPECTRUM       = 3,
   TIME_SERIES        = 4,
   PHASE_SPECTRUM     = 5,
   NO_SPECTRUM        = 6,
   SPECTRUM_COUNT     = 5  //!TODO: Only Mag/DB/PSD implemented, so count = 3. 
}SpectrumTypeEnum;


typedef enum WindowTypeEnum
{
   HAMMING_WINDOW     = 0,
   BLACKMAN_WINDOW    = 1,
   BARLETTE_WINDOW    = 2,
   RECTANGULAR_WINDOW = 3, 
   NO_WINDOW          = 5,
   WINDOW_COUNT       = 3, // Number of actionable window types 
}WindowTypeEnum;

typedef enum FilterTypeEnum
{
   LOWPASS = 1,
   HIGHPASS = 2,
   BANDPASS = 3,
   BANDSTOP = 4,
   NO_FILTER = 5
}FilterTypeEnum;


typedef enum TaskIDEnum
{
   SPECTRUMDSPTASK = 0,
   MVCTASK = 1,
   MICINPUTTASK = 2
}TaskIDEnum;

typedef enum PacketIDEnum
{
   SAMPLE_BUFFER_PACKET   = 0,
   SPECTRUM_OUTPUT_PACKET = 1
}PacketIDEnum;

typedef enum MessageStatusEnum
{
   MESSAGE_SUCCESS = 0,
   MESSAGE_FAILED  = 1,
   MESSAGE_HELD    = 2
}MessageStatusEnum;


typedef enum ConsumerStatusEnum
{
   SOME_CONSUMERS_NEED_PACKET        = 0,
   ALL_OTHER_CONSUMERS_USED_PACKET   = 1,
   PACKET_CONSUMED_FOR_THIS_CONSUMER = 2,
   CONSUMER_STILL_WAITING_FOR_DATA   = 3
}ConsumerStatusEnum;

#endif // enums.h