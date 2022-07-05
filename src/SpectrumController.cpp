#include "SpectrumController.h"

SpectrumController::SpectrumController(){
	currentState = READY;
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


	Plot* clickedPlot = model->detectClickPlot((xpos * 2 / 800) - 1, ((ypos * 2 / 800) - 1) * -1);
	IModel->detectClickWidget((xpos * 2 / 800) - 1, ((ypos * 2 / 800) - 1) * -1);
	
	switch (currentState) {
		case SpectrumController::READY:
			if (clickedPlot == nullptr) {
				// --            x      y     wid   height  XrefMin  YrefMin    XrefMax  YrefMax  rows cols     Methodflag   
 				//model->addPlot( 0.5f,  0.5f,  0.6f, 0.6f,     0.0f, -128.0f,     1.0f,   128.0f,  5,   5, SpectrumModel::NORMAL, true, true);
				model->addPlot(-0.5f,  0.5f,  0.6f, 0.6f, -1000.0f,    0.0f,  1000.0f, 10000.0f,  5,   5, SpectrumModel::MAG, true, true);
				model->addPlot(-0.5f, -0.5f,  0.6f, 0.6f,     1.0f,  -90.0f, 10000.0f,   180.0f,  5,   4, SpectrumModel::DB_MAG, false, true);
				//model->addPlot( 0.5f, -0.5f,  0.6f, 0.6f, -1000.0f,    0.0f,  1000.0f,   500.0f,  4,   4, SpectrumModel::PWR_SPECTRUM);
				currentState = READING;
			}
			break;
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
}

void SpectrumController::handleBooleanButton(std::string buttonID, int type){
	std::cout << buttonID << std::endl;
	if (buttonID == "Detrend") { model->setDetrendFlag(); }
	//else if (buttonID == "Normalize") {}
}

void SpectrumController::handleListMenu(std::string ListID, int optionNum) {
	std::cout << ListID << std::endl;
	if      (ListID == "Filter") { model->setFilterFlag(optionNum); }
	else if (ListID == "Window") { model->setWindowFlag(optionNum); }
	else if (ListID == "Method") { model->setMethodFlag(optionNum); }
}



