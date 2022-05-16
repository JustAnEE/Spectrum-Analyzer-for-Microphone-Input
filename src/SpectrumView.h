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
#include"SpectrumModel.h"
#include"SpectrumController.h"
#include FT_FREETYPE_H 

using namespace std;


class SpectrumView : public SpectrumModelSubscriber {
public:
	struct Character {
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};
	std::map<char, Character> Characters;

	SpectrumModel* model;

	SpectrumView();
	~SpectrumView();
	void draw();
	void setModel(SpectrumModel* givenModel);
	void setController(SpectrumController* givenController);
	GLFWwindow* getWindow();
	void modelChanged();

private:
	GLuint VAO, VBO, TVAO, TVBO; //EBO;
	GLuint vertexShader, fragmentShader, shaderProgram;
	GLuint textureVertexShader, textureFragmentShader, textureShaderProgram;
	GLFWwindow* window;

	void drawPlot(Plot* plot);
	void drawText(string text, float xNormalized, float yNormalized, float scale);
	
};

#endif // !SPECTRUMVIEW_H

