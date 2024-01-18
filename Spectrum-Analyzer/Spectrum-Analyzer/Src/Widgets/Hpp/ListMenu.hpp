#ifndef LISTMENU_H
#define LISTMENU_H

#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "../Hpp/Widget.hpp"

class ListMenu : public Widget
{
public:
    ListMenu(glm::vec3 _location, float _width, float _height, std::string _title, std::vector<std::string> _options, std::string _ID);
    ~ListMenu();

    void setText(std::string _title, std::vector<std::string> _options);
    void setSelectedOption(int _value);

    int detectClick(double xpos, double ypos);
    void scrollOptions(double xpos, double ypos, int direction);

private:
    // only draw 6 options 1 for each slot. if there are more than 6 options, each scroll will slide
    // the options window, E.G 10 options, window starts at 0 -> 5:  scroll up, window slides to  1 -> 6

    float bottom, top, left, right;
    int selectedOption;

    std::vector<std::string> optionStrings;
    std::string title;

    int windowStart, optionsShown;

    void fillVertexBuffer();
    void fillOptionText();
};
#endif // !LISTMENU_H
