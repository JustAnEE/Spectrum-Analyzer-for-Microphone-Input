#ifndef LISTMENU_H
#define LISTMENU_H

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "TextLabel.h"

class ListMenu{
public:
	ListMenu(glm::vec3 _location, float _width, float _height, std::string _title, std::vector<std::string> _options);
	~ListMenu();

	int detectClick(double xpos, double ypos);
	void scrollOptions(int direction);

private:
	// -- only draw 6 options 1 for each slot. if there are more than 6 options, each scroll will slide
	// -- the options window, E.G 10 options, window starts at 0 -> 5:  scroll up, window slides to  1 -> 6

	// -- detectClick

	glm::vec3 location;
	float width, height;
	float bottom, top, left, right;
	std::vector<std::string> optionStrings;
	std::vector<TextLabel*> optionLabels;
	TextLabel* title;
	
	int windowStart, optionsShown;

	float* vertexBuffer;
	
	void fillVertexBuffer();
	void fillOptionText();

};




#endif // !LISTMENU_H


