#include <iostream>
#include "SpectrumModel.h"
#include "SpectrumView.h"
#include "SpectrumController.h"

int main() {
	SpectrumController* controller = new SpectrumController();
	SpectrumModel* model = new SpectrumModel();
	SpectrumView* view = new SpectrumView();

	// -- set connections
	view->setModel(model);
	view->setController(controller);
	controller->setModel(model);
	model->addSubscriber(view);

	// -- needed to stop the loop.
	GLFWwindow* window = view->getWindow();

	// -- event loop
	while (!glfwWindowShouldClose(window)){
		
		// -- poll every cycle.
		controller->jobStartEvent();

		glfwPollEvents();
	}

	delete view;
	delete model;
	delete controller;
	return 0;
}












