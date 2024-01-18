#include "../Hpp/Widget.hpp"

float Widget::getWidth()
{
    return width;
}

float Widget::getHeight()
{
    return height;
}

float* Widget::getVertexBuffer()
{
    return vertexBuffer;
}

int Widget::getVertexBufferSizeBytes()
{
    return vertexBufferSizeBytes;
}

std::vector<TextLabel*> Widget::getTextList()
{
    return textList;
}

std::string Widget::getID()
{
    return ID;
}

void Widget::setController(SpectrumController* _controller)
{
    controller = _controller;
}

void Widget::setClickCallback(func_ptr _ptr)
{
    clickCallbackFunction = _ptr;
}

void Widget::clickAction(int flag)
{
    (controller->*clickCallbackFunction)(ID, flag);
}

void Widget::clearTextList()
{
    for (TextLabel* t : textList)
    {
        delete t;
    }

    textList.clear();
}
