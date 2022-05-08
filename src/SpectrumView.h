#ifndef SPECTRUMVIEW_H
#define SPECTRUMVIEW_H

#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"SpectrumModelSubscriber.h"
#include"SpectrumModel.h"
#include"SpectrumController.h"

class SpectrumView : public SpectrumModelSubscriber {
public:
	SpectrumModel* model;

	SpectrumView();
	~SpectrumView();
	void draw();
	void drawPlot(Plot* plot);

	void setModel(SpectrumModel* givenModel);
	void setController(SpectrumController* givenController);
	GLFWwindow* getWindow();
	void modelChanged();

private:
	GLuint VAO, VBO; //EBO;
	GLuint vertexShader, fragmentShader, shaderProgram;
	GLFWwindow* window;
};

#endif // !SPECTRUMVIEW_H

