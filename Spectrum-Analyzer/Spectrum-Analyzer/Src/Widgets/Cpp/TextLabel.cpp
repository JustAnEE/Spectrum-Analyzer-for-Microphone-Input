#include "../Hpp/TextLabel.hpp"

TextLabel::TextLabel(glm::vec3 _origin, glm::vec3 _color, float _width, std::string _text)
{
    origin = _origin;
    color = _color;
    width = _width;
    text = _text;
}
