#ifndef SPECTRUMVIEW_H
#define SPECTRUMVIEW_H

#include <iostream>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"../../Misc/Hpp/FontSystem.hpp"
#include"../../MVC/Hpp/InteractionModel.hpp"
#include"../../MVC/Hpp/SpectrumController.hpp"
#include"../../MVC/Hpp/SpectrumModel.hpp"
#include"../../Widgets/Hpp/Button.hpp"
#include"../../Widgets/Hpp/ListMenu.hpp"
#include"../Hpp/SpectrumModelSubscriber.hpp"
#include"../Hpp/InteractionModelSubscriber.hpp"

#include FT_FREETYPE_H 

class SpectrumView : public SpectrumModelSubscriber, public InteractionModelSubscriber {
public:

	SpectrumView(GLuint windowWidth, GLuint windowHeight);
	~SpectrumView();

	GLFWwindow* getWindow();

	// -- MVC connection functions for main
	void setModels(SpectrumModel* _model, InteractionModel* _IModel);
	void setController(SpectrumController* _controller);

	// -- InteractionModelSub functions
	void detectClick(double xpos, double ypos);
	void detectScroll(double xpos, double ypos, int direction);

	void DModelChanged();
	void IModelChanged();


private:
	GLuint VAO, VBO, TVAO, TVBO;
	GLuint shaderProgram, textureShaderProgram;

	GLFWwindow* window;
	GLuint windowWidth, windowHeight;
	
	FontSystem* fSys;
	GLuint atlasID;

	std::vector<Widget*> widgets;
	std::vector<std::string> methods, filters, windows;

	SpectrumModel* model;
	InteractionModel* IModel;

	void draw();
	void drawWidget(Widget* widget);
	void drawPlot(Plot* plot);
	void drawText(TextLabel* textLabel);

	void swapListMenu(std::string NewMenuID);


	std::string readShaderCode(const char* filename); 

};

#endif // !SPECTRUMVIEW_H
