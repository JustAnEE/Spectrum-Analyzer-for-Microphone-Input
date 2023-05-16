#ifndef SPECTRUMDSPTASK_HPP
#define SPECTRUMDSPTASK_HPP

#include "../TaskBase.hpp"
#include "../MessageController.hpp"
#include "../Packets/Hpp/SampleBufferPacket.hpp"
#include "../Packets/Hpp/SpectrumRequestPacket.hpp"
#include "../Packets/Hpp/SpectrumOutputPacket.hpp"

#include "Hpp/MagnitudeSpectrum.hpp"
#include "Hpp/PSDSpectrum.hpp"
#include "Hpp/DBSpectrum.hpp"

class SpectrumDSPTask : public TaskBase
{


public:
   SpectrumDSPTask(Mutex& refMutex_, CondVar& refCondVar_, TaskQueue& refTaskQueue_);

public:

   void InitializeTask() {}
   void Main();

private:
   void HandleSampleBufferData(std::shared_ptr<SampleBufferCL> pclSampleBufferData_);
   void HandleSpectrumRequest(std::shared_ptr<SpectrumRequestCL> pclSpectrumRequest_);
private:

   SpectrumBase* apclSignalProcessors[SPECTRUM_COUNT]; 

   MessageController<std::shared_ptr<SampleBufferCL>,    SpectrumDSPTask>* pclSampleBufferController;
   MessageController<std::shared_ptr<SpectrumRequestCL>, SpectrumDSPTask>* pclSpectrumRequestController; 
   MessageController<std::shared_ptr<SpectrumOutputCL>,  SpectrumDSPTask>* pclSpectrumOutputController; 
};
#endif // SPECTRUMDSPTASK_HPP