#ifndef TEXT_H
#define TEXT_H

#include <iostream>
#include <glm/glm.hpp>


class TextLabel{
public:

	TextLabel(glm::vec3 _origin, glm::vec3 _color, float _width, std::string _text);

	glm::vec3 origin, color;
	float width;
	std::string text;

};
#endif //!TEXT_H
