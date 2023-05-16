#ifndef MICINPUTTASK_HPP
#define MICINPUTTASK_HPP

#include "../TaskBase.hpp"
#include "Hpp/MicInput.hpp"

class MicInputTask : public TaskBase
{
public:
   void InitializeTask();
   void Main();

private:
   void GenerateMicData();
   void PublishMicData(); 

private:
   MicInput clMicInput;
};
#endif // MICINPUTTASK_HPP