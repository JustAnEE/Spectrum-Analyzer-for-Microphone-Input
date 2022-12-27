#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include <string>

#include <glm/glm.hpp>

#include "../Hpp/Widget.hpp"
#include "../Hpp/TextLabel.hpp"

class Button : public Widget{
public:
	Button(glm::vec3 _location, glm::vec3 _color, float _width, float _height, std::string _text, std::string _ID);
	~Button();

	// -- virtual function implemntation
	int detectClick(double xpos, double ypos);

	glm::vec3 getDrawLocation();
	TextLabel* getLabelText();


private:
	bool PRESSED;
	float bottom, top, left, right;

	std::string textString;

	void fillVertexBuffer();
	void fillTextLabel();




};

#endif //! BUTTON_H
