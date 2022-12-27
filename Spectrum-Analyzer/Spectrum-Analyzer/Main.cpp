#include <iostream>

#include "Src/MVC/Hpp/InteractionModel.hpp"
#include "Src/MVC/Hpp/SpectrumController.hpp"
#include "Src/MVC/Hpp/SpectrumModel.hpp"
#include "Src/MVC/Hpp/SpectrumView.hpp"


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
	return 0;
}












