
#ifndef SPECCONT_H
#define SPECCONT_H

#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include"../Hpp/SpectrumModel.hpp"
#include"../Hpp/InteractionModel.hpp"


typedef enum ControllerStateEnum
{
	START   = 0,
	READING = 1,
	PROCESSING = 2,
	HALT = 3 
}ControllerStateEnum; 

class SpectrumController {
public:
	SpectrumModel* model;
	InteractionModel* IModel;

	SpectrumController();
	~SpectrumController() = default;

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
	ControllerStateEnum eMyControllerState;	
};





#endif
