#include <iostream>

#include "Src/MVC/Hpp/InteractionModel.hpp"
#include "Src/MVC/Hpp/SpectrumController.hpp"
#include "Src/MVC/Hpp/SpectrumModel.hpp"
#include "Src/MVC/Hpp/SpectrumView.hpp"
#include "Src/Dsp/SpectrumDSPTask.hpp"
#include "Src/MVC/MVCTask.hpp"
#include "Src/Misc/MicInputTask.hpp"


// Method which starts the task when passing the method into threads 
void TaskStarter(TaskBase* pclTask_)
{
   pclTask_->Main();
   return;
}


int main() {
   SpectrumController* controller = new SpectrumController();
   SpectrumModel* model = new SpectrumModel();
   InteractionModel* IModel = new InteractionModel();
   SpectrumView* view = new SpectrumView(1000,900);

   // -- Set MVC connections.
   view->setModels(model, IModel);
   view->setController(controller);
   controller->setDModel(model);
   controller->setIModel(IModel);

   // -- Pub Sub connections.
   model->addSubscriber(view);
   IModel->addSubscriber(view);


   GLFWwindow* window = view->getWindow();

   // -- Main Event loop.
   while (!glfwWindowShouldClose(window)){
      
      // -- Event Polling.
      controller->jobStartEvent();

      glfwPollEvents();
   }

   delete view;
   delete model;
   delete controller;

   // Multi threaded implementation. 

   // Create a Queue
   std::queue<TaskQueueData> qDataQueue;

   // Create Mutex and condition variable for waiting threads 
   Mutex mutSharedMutex;
   CondVar cvSharedConditionVariable; 

   // Instantiate tasks.
   std::vector<std::unique_ptr<TaskBase>> vTaskVector;
   vTaskVector.emplace_back(std::make_unique<SpectrumDSPTask>(mutSharedMutex, cvSharedConditionVariable, qDataQueue));
   vTaskVector.emplace_back(std::make_unique<MVCTask>        (mutSharedMutex, cvSharedConditionVariable, qDataQueue));
   vTaskVector.emplace_back(std::make_unique<MicInputTask>   (mutSharedMutex, cvSharedConditionVariable, qDataQueue));

   // A vector for threads. 
   std::vector<std::thread> vTaskThreads;
   for (auto& Task : vTaskVector)
   {
      vTaskThreads.emplace_back(TaskStarter, Task.get());
   }

   // Spawn and start the threads 
   for (std::thread& Task : vTaskThreads)
   {
      Task.join();
   }


   return 0;
}












