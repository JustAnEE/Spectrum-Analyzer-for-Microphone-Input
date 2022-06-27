#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include <string>

#include <glm/glm.hpp>

#include "TextLabel.h"

class Button{
public:
	Button(glm::vec3 _location, glm::vec3 _color, float _width, float _height, std::string _text);
	~Button();


	bool detectClick(double xpos, double ypos, int type);

	glm::vec3 getDrawLocation();
	TextLabel* getLabelText();
	float getWidth();
	float getHeight();

	void setPressedFlag(bool status);


private:
	bool PRESSED;

	float* vertexBuffer;

	glm::vec3 location, color;
	float width, height;
	TextLabel* text;
	std::string textString;

	void fillVertexBuffer();
	void fillTextLabel();




};

#endif //! BUTTON_H
