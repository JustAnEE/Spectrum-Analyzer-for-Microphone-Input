
#ifndef SPECCONT_H
#define SPECCONT_H

#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"SpectrumModel.h"


using namespace std;

class SpectrumController {
public:
	SpectrumModel* model;

	SpectrumController();
	~SpectrumController();
	void setModel(SpectrumModel* model);
	void jobStartEvent();
	void handleKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);
	void handleMouseClick(GLFWwindow* window, int button, int action, int mods);

private:
	enum STATE { READY, READING, PROCESSING, HALT };
	STATE currentState;
};





#endif
