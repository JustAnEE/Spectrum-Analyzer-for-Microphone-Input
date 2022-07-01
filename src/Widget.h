#ifndef WIDGET_H
#define WIDGET_H

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include "TextLabel.h"
#include "SpectrumController.h"


class Widget {
public:

	typedef void (SpectrumController::*func_ptr)(std::string, int);

	float getWidth();
	float getHeight();

	float* getVertexBuffer();
	int getVertexBufferSizeBytes();
	std::vector<TextLabel*> getTextList();

	void setWidgetCallback(func_ptr _ptr, SpectrumController* controller);
	void action(int flag);

	virtual int detectClick(double xpos, double ypos) = 0;

protected:
	// -- Yucky pointers to controller in the widget class.
	func_ptr ptr;
	SpectrumController* controller;


	float* vertexBuffer;
	int vertexBufferSizeBytes;

	std::vector<TextLabel*> textList;
	std::string ID;

	glm::vec3 location, color;
	float width, height;
};


#endif // !WIDGET_H

