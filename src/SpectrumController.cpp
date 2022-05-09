#include "SpectrumController.h"

SpectrumController::SpectrumController(){
	currentState = READY;
}

SpectrumController::~SpectrumController(){
}




void SpectrumController::jobStartEvent(){
	switch (currentState){
		case SpectrumController::READING:
			model->readMicData();
			// -- multiple processing states for different types of data
			currentState = PROCCESSING;
			break;
		case SpectrumController::PROCCESSING:
			model->processData();
			// -- multiple processing states for different types of data
			currentState = READING;
			break;
		default:
			break;
	}
}

void SpectrumController::handleKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods){
	switch (currentState){
		default:
			break;
	}
}

void SpectrumController::handleMouseClick(GLFWwindow* window, int button, int action, int mods){
	// -- mouse position on click
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	Plot* clickedPlot = model->detectHit((xpos * 2 / 800) - 1, ((ypos * 2 / 800) - 1) * -1);
	
	switch (currentState) {
		case SpectrumController::READY:
			if (clickedPlot == nullptr) {
				model->addPlot(-0.5f, 0.0f, 1.0f, 1.0f, 1, 1, 0);
				model->addPlot( 0.5f, 0.0f, 1.0f, 1.0f, 4, 4, 1);
				currentState = READING;
			}
			break;
		default:
			if (clickedPlot != nullptr) {
				//model->movePlot(clickedPlot, (xpos * 2 / 800) - 1, ((ypos * 2 / 800) - 1) * -1);
				model->scalePlot(clickedPlot, 0.0f, 0.02f);
				//model->changePlotRef(clickedPlot, xpos, ypos);
				currentState = READING;
			}
			break;
	}
}

void SpectrumController::setModel(SpectrumModel* newModel) {
	model = newModel;
}