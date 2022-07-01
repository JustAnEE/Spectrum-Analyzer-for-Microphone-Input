#include "Widget.h"

float Widget::getWidth(){
    return width;
}

float Widget::getHeight(){
    return height;
}

float* Widget::getVertexBuffer(){
    return vertexBuffer;
}

int Widget::getVertexBufferSizeBytes(){
    return vertexBufferSizeBytes;
}

std::vector<TextLabel*> Widget::getTextList() {
    return textList;
}


void Widget::setWidgetCallback(func_ptr _ptr, SpectrumController* _controller) {
    controller = _controller;
    ptr = _ptr;
}

void Widget::action(int flag){
    (controller->*ptr)(ID, flag);
}
