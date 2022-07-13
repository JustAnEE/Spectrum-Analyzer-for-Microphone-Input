#include "SpectrumController.h"

SpectrumController::SpectrumController(){
	currentState = START;
}

SpectrumController::~SpectrumController(){}


void SpectrumController::setDModel(SpectrumModel* _model) {
	model = _model; 
}
void SpectrumController::setIModel(InteractionModel* _IModel) {
	IModel = _IModel;
}


// -- Event handling methods.
void SpectrumController::jobStartEvent(){
	switch (currentState){
		case SpectrumController::START:
			// -- Initial setup featuring the worlds longest function.
			model->addPlot(
				-0.25f,  -0.25f,	  1.0f,		   1.0f,  
					 5,		  5, 
			    SpectrumModel::TIMESERIES, 
					 0,		  0,	  false,	  false
			);
			IModel->setSelectedPlot(model->getPlotVector()[0]);
			currentState = READING;
			break;

		case SpectrumController::READING:
			// -- Read data from device, then swap state to PROCCESSING
			model->readMicData();
			currentState = PROCESSING;
			break;

		case SpectrumController::PROCESSING:
			// -- process data then swap state to READING
			model->processData();
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


	//Plot* clickedPlot = model->detectClickPlot((xpos * 2 / 800) - 1, ((ypos * 2 / 800) - 1) * -1);
	//IModel->setSelectedPlot(clickedPlot);
	IModel->detectClickWidget((xpos * 2 / 800) - 1, ((ypos * 2 / 800) - 1) * -1);
	
	switch (currentState) {
		default:
			break;
	}
}

void SpectrumController::handleMouseScroll(GLFWwindow* window, double xoffset, double yoffset){
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	int direction = (yoffset >= 0.0) ? 1 : -1;

	IModel->detectScrollWidget((xpos * 2 / 800) - 1, ((ypos * 2 / 800) - 1) * -1, direction);
}


void SpectrumController::handleListButton(std::string buttonID, int type) {
	IModel->swapListMenu(buttonID);
	// -- change selection box
}

void SpectrumController::handleBooleanButton(std::string buttonID, int type){
	bool detrendFlag = IModel->getSelectedPlot()->getDetrendFlag();

	if (buttonID == "Detrend") { IModel->getSelectedPlot()->setDetrendFlag(!detrendFlag); }
	//else if (buttonID == "Normalize") {}
}

void SpectrumController::handleListMenu(std::string ListID, int optionNum) {
	if      (ListID == "Filter") { IModel->getSelectedPlot()->setFilterFlag(optionNum); }
	else if (ListID == "Window") { IModel->getSelectedPlot()->setWindowFlag(optionNum); }
	else if (ListID == "Method") { IModel->getSelectedPlot()->setMethodFlag(optionNum); }
	cout << endl;
	cout << "DETRED FLAG: " << IModel->getSelectedPlot()->getDetrendFlag() << endl;
	cout << "FILTER FLAG: " << IModel->getSelectedPlot()->getFilterFlag() << endl;
	cout << "WINDOW FLAG: " << IModel->getSelectedPlot()->getWindowFlag() << endl;
	cout << "METHOD FLAG: " << IModel->getSelectedPlot()->getMethodFlag() << endl;
	cout << endl;
}



