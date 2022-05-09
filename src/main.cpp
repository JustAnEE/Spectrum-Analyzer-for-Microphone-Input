#include <iostream>
#include "SpectrumModel.h"
#include "SpectrumView.h"
#include "SpectrumController.h"

int main() {
	SpectrumController* controller = new SpectrumController();
	SpectrumModel* model = new SpectrumModel();
	SpectrumView* view = new SpectrumView();

	// -- Set MVC connections.
	view->setModel(model);
	view->setController(controller);
	controller->setModel(model);
	model->addSubscriber(view);

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












