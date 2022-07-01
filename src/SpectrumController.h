
#ifndef SPECCONT_H
#define SPECCONT_H

#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include"SpectrumModel.h"
#include"InteractionModel.h"

using namespace std;

class SpectrumController {
public:
	SpectrumModel* model;
	InteractionModel* IModel;

	SpectrumController();
	~SpectrumController();

	void setDModel(SpectrumModel* _model);
	void setIModel(InteractionModel* _IModel);
	
	void jobStartEvent();

	// -- General handlerMethods
	void handleKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);
	void handleMouseClick(GLFWwindow* window, int button, int action, int mods);
	void handleMouseScroll(GLFWwindow* window, double xoffset, double yoffset);

	// -- widget handlerMethods.
	void handleListButton(std::string buttonID, int type);
	void handleBooleanButton(std::string buttonID, int type);
	void handleListMenu(std::string ListID, int optionNum);

private:
	enum STATE { READY, READING, PROCESSING, HALT };
	STATE currentState;
};





#endif
