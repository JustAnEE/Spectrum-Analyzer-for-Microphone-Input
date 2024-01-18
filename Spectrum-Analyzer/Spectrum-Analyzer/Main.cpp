#include <iostream>

#include "Src/MVC/Hpp/InteractionModel.hpp"
#include "Src/MVC/Hpp/SpectrumController.hpp"
#include "Src/MVC/Hpp/SpectrumModel.hpp"
#include "Src/MVC/Hpp/SpectrumView.hpp"
#include <memory> 

//!TODO: Could add some sort of object which handles all the boilerplate, pull more stuff out of main
int main() {

	std::unique_ptr<SpectrumController> pclController = std::make_unique<SpectrumController>();
	std::unique_ptr<SpectrumModel> pclModel = std::make_unique<SpectrumModel>();
	std::unique_ptr<InteractionModel> pclIModel = std::make_unique<InteractionModel>();
	std::unique_ptr<SpectrumView> pclView = std::make_unique<SpectrumView>(1000, 900); 

	// -- Set MVC connections.
	pclView->setModels(pclModel.get(), pclIModel.get());
	pclView->setController(pclController.get());
	pclController->setDModel(pclModel.get());
	pclController->setIModel(pclIModel.get());

	// -- Pub Sub connections.
	pclModel->addSubscriber(pclView.get());
	pclIModel->addSubscriber(pclView.get());


	GLFWwindow* window = pclView->getWindow();

	// -- Main Event loop.
	while (!glfwWindowShouldClose(window)){
		
		// -- Event Polling.
		pclController->jobStartEvent();

		glfwPollEvents();
	}

	return 0;
}












