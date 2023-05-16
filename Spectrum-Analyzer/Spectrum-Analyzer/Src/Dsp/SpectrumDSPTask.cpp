#include "SpectrumDSPTask.hpp"

SpectrumDSPTask::SpectrumDSPTask(Mutex& refMutex_, CondVar& refCondVar_, TaskQueue& refTaskQueue_) 
   : TaskBase(refMutex_, refCondVar_, refTaskQueue_)
{
   // Init message controllers 
   pclSampleBufferController = new MessageController<std::shared_ptr<SampleBufferCL>, SpectrumDSPTask>(*this);
   pclSpectrumRequestController = new MessageController<std::shared_ptr<SpectrumRequestCL>, SpectrumDSPTask>(*this);

   pclSampleBufferController->RegisterProvider(SPECTRUMDSPTASK);
   pclSpectrumRequestController->RegisterProvider(SPECTRUMDSPTASK);

   apclSignalProcessors[MAGNITUDE_SPECTRUM] = new MagnitudeSpectrum(SAMPLE_RATE, SAMPLE_BUFFER_SIZE);
   apclSignalProcessors[PSD_SPECTRUM]       = new PSDSpectrum(SAMPLE_RATE, SAMPLE_BUFFER_SIZE);
   apclSignalProcessors[DB_SPECTRUM]        = new DBSpectrum(SAMPLE_RATE, SAMPLE_BUFFER_SIZE);
   return; 
}


void SpectrumDSPTask::Main()
{
   // Init packets received by the task to nullptr (this way the message controller can tell 
   // if the incoming pointer already has data or not)
   std::shared_ptr<SampleBufferCL> pclSampleBufferData;
   std::shared_ptr<SpectrumRequestCL> pclSpectrumRequest;

   while (true)
   {
      pclSampleBufferData = nullptr;
      pclSpectrumRequest = nullptr;

      pclSampleBufferController->RequestPacket(pclSampleBufferData);
      pclSpectrumRequestController->RequestPacket(pclSpectrumRequest);
      // If the sample buffer data is not null, the object has been created and memory has been
      // read off the queue. 
      if (pclSampleBufferData != nullptr)
      {
         HandleSampleBufferData(pclSampleBufferData);
      }

      if (pclSpectrumRequest != nullptr)
      {
         HandleSpectrumRequest(pclSpectrumRequest);
      }

   }

}

void SpectrumDSPTask::HandleSampleBufferData(std::shared_ptr<SampleBufferCL> pclSampleBufferData_)
{
   for (int iDSP = 0; iDSP < SPECTRUM_COUNT; iDSP++)
   {
      *(apclSignalProcessors[iDSP]->GetpclSpectrumConfig()->GetSampleBuffer()) = *pclSampleBufferData_;
   }
}

void SpectrumDSPTask::HandleSpectrumRequest(std::shared_ptr<SpectrumRequestCL> pclSpectrumRequest_)
{
   apclSignalProcessors[pclSpectrumRequest_->stDSPInit.eSpectrumOutput]
      ->GetpclSpectrumConfig()->ConfigDSPOnRequest(pclSpectrumRequest_->stDSPInit);

   if(apclSignalProcessors[pclSpectrumRequest_->stDSPInit.eSpectrumOutput]->GetpclSpectrumConfig()->HasDSPBeenConfigured())
   { 
      // If the DSP has been configured we can go ahead and process the data and send it to the MVC 
      std::shared_ptr<SpectrumOutputCL> pclSpectrumOutputData = std::make_shared<SpectrumOutputCL>();

      apclSignalProcessors[pclSpectrumRequest_->stDSPInit.eSpectrumOutput]->CalculateSpectralData();
      
      memcpy(pclSpectrumOutputData->afSpectrumOutput,
         apclSignalProcessors[pclSpectrumRequest_->stDSPInit.eSpectrumOutput]->GetSpectrumOutput()->afMySpectrumArray,
         2 * SAMPLE_BUFFER_SIZE);

      // Give the output message the DSPInit so that the user can tell what settings were used to generate the output data
      // if needed. 
      memcpy(&pclSpectrumOutputData->stDSPInit, &pclSpectrumRequest_->stDSPInit, sizeof(DSPInitStruct));

      pclSpectrumOutputController->PublishMessage(pclSpectrumOutputData);
   }
}