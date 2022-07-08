#include "Button.h"

Button::Button(glm::vec3 _location, glm::vec3 _color, float _width, float _height, std::string _text, std::string _ID){
    ID = _ID;
    
    PRESSED = false;
    location = _location;
    color = _color;
    width = _width;
    height = _height;
    textString = _text;

    left = location.x - (width / 2);
    right = location.x + (width / 2);
    bottom = location.y - (height / 2);
    top = location.y + (height / 2);

    vertexBufferSizeBytes = 8 * 6 * sizeof(float);
    vertexBuffer = (float*)calloc(8 * 6, sizeof(float));
    
    fillVertexBuffer();
    fillTextLabel();
}

Button::~Button(){
    free(vertexBuffer);
}



int Button::detectClick(double xpos, double ypos) {
    if (xpos >= left && xpos <= right && ypos >= bottom && ypos <= top) {
        return 0;
    }
    return -1;
}


glm::vec3 Button::getDrawLocation(){ return location; }
TextLabel* Button::getLabelText(){ return  textList[0]; }



// -- private method
void Button::fillVertexBuffer(){
    float halfWidth = width / 2;
    float halfHeight = height / 2;
    for (int i = 2; i < 8 * 6; i += 6) {
        vertexBuffer[i + 0] = 0.0f;    // -- Z position
        if (PRESSED) {
            vertexBuffer[i + 1] = 1.0f;
            vertexBuffer[i + 2] = 0.7f;
            vertexBuffer[i + 3] = 0.27f;
        }
        else {
            vertexBuffer[i + 1] = color.r;
            vertexBuffer[i + 2] = color.g;
            vertexBuffer[i + 3] = color.b;
        }
    }
    // -- locations
    vertexBuffer[0]  = location.x - halfWidth;      vertexBuffer[1]  = location.y - halfHeight;
    vertexBuffer[6]  = location.x - halfWidth;      vertexBuffer[7]  = location.y + halfHeight;

    vertexBuffer[12] = location.x - halfWidth;      vertexBuffer[13] = location.y - halfHeight;
    vertexBuffer[18] = location.x + halfWidth;      vertexBuffer[19] = location.y - halfHeight;

    vertexBuffer[24] = location.x + halfWidth;      vertexBuffer[25] = location.y + halfHeight;
    vertexBuffer[30] = location.x + halfWidth;      vertexBuffer[31] = location.y - halfHeight;

    vertexBuffer[36] = location.x + halfWidth;      vertexBuffer[37] = location.y + halfHeight;
    vertexBuffer[42] = location.x - halfWidth;      vertexBuffer[43] = location.y + halfHeight;

}

void Button::fillTextLabel(){
    textList.clear();
    if (PRESSED) { textList.push_back(new TextLabel(location, glm::vec3 (1.0f, 0.7f, 0.27f), width * 0.7, textString)); }
    else { textList.push_back(new TextLabel(location, color, width * 0.7, textString)); }
}
