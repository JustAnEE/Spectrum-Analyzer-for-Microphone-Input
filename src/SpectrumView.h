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

#include"SpectrumModelSubscriber.h"
#include"InteractionModelSubscriber.h"
#include"SpectrumModel.h"
#include"InteractionModel.h"

#include"SpectrumController.h"

#include"Button.h"
#include"ListMenu.h"

#include FT_FREETYPE_H 

using namespace std;


class SpectrumView : public SpectrumModelSubscriber, public InteractionModelSubscriber {
public:
	// -- Character struct used for rendering character textures
	struct Character {
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};
	// -- Map to store each Character
	std::map<char, Character> Characters;


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


	void swapListMenu(std::string NewMenuID);


private:
	GLuint VAO, VBO, TVAO, TVBO;
	GLuint shaderProgram, textureShaderProgram;

	GLFWwindow* window;
	GLuint windowWidth, windowHeight;

	std::vector<Widget*> widgets;
	std::vector<std::string> methods, filters, windows;

	SpectrumModel* model;
	InteractionModel* IModel;

	void draw();
	void drawWidget(Widget* widget);
	void drawPlot(Plot* plot);
	void drawText(TextLabel* textLabel);
	string readShaderCode(const char* filename); 

};

#endif // !SPECTRUMVIEW_H
