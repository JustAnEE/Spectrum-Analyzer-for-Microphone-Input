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
			currentState = PROCESSING;
			break;
		case SpectrumController::PROCESSING:
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
				// --            x      y     wid   height  XrefMin  YrefMin   XrefMax  YrefMax  rows cols     Methodflag   
 				model->addPlot( 0.5f,  0.5f,  0.6f, 0.6f,     0.0f, -128.0f,    1.0f,   128.0f,  5,   5, SpectrumModel::NORMAL);
				model->addPlot(-0.5f,  0.5f,  0.6f, 0.6f, -1000.0f,    0.0f, 1000.0f, 10000.0f,  5,   5, SpectrumModel::MAG);
				model->addPlot(-0.5f, -0.5f,  0.6f, 0.6f, -1000.0f,  -90.0f, 1000.0f,   180.0f,  5,   5, SpectrumModel::DB_MAG);
				model->addPlot( 0.5f, -0.5f,  0.6f, 0.6f, -1000.0f,    0.0f, 1000.0f,   500.0f,  4,   4, SpectrumModel::PWR_SPECTRUM);
				currentState = READING;
			}
			break;
		default:
			if (clickedPlot != nullptr) {
				// -- TESTING PLOT functionality
				//model->movePlot(clickedPlot, (xpos * 2 / 800) - 1, ((ypos * 2 / 800) - 1) * -1);
				//model->scalePlot(clickedPlot, 0.0f, 0.02f);
				//model->changePlotRef(clickedPlot, xpos, ypos);
				//model->removePlot(clickedPlot);
				currentState = READING;
			}
			break;
	}
}

void SpectrumController::setModel(SpectrumModel* newModel) {
	model = newModel;
}