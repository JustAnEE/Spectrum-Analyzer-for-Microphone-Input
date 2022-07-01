#include "ListMenu.h"



ListMenu::ListMenu(
        glm::vec3 _location, float _width, float _height, std::string _title, 
        std::vector<std::string> _options, std::string _ID
    ) {
    ID = _ID;

    optionsShown = 6;
    windowStart = 1;

    location = _location;
    width = _width;
    height = _height;
    optionStrings = _options;
    title = _title;


    left = location.x - (width / 2);
    right = location.x + (width / 2);
    bottom = location.y - (height / 2);
    top = location.y + (height / 2);

    // -- create title and options TextLables.
    fillOptionText();

    // -- Initialize the vertex buffer
    vertexBufferSizeBytes = 10 * 6 * sizeof(float);
    vertexBuffer = (float*)calloc(10 * 6, sizeof(float));
    fillVertexBuffer();
}


ListMenu::~ListMenu() {
    free(vertexBuffer);
}



void ListMenu::setText(std::string _title, std::vector<std::string> _options) {
    title = _title;
    optionStrings = _options;
    fillOptionText();
}

// -- Virtual function implemntation.
int ListMenu::detectClick(double xpos, double ypos) {
    float range  = 0.05;
    float optionHeight = 7.0f / 8.0f * height;

    int size = (optionStrings.size() >= optionsShown) ? optionsShown : optionStrings.size();

    // -- check each option top down.
    for (int i = optionsShown; i > optionsShown - size; i--) {
        float y = i * (optionHeight / (optionsShown + 1)) + bottom;

        if (xpos > left && xpos < right && ypos > y - range && ypos < y + range) {
            cout << "selected: " << i << endl;
            return i;
        }
    }

    
    return -1;
}


void ListMenu::scrollOptions(int direction){
    int upperLimit = optionStrings.size() - optionsShown;

    // -- Only scroll if there are more options than 'optionsShown'.
    if (optionStrings.size() < optionsShown) { return; }
    if ((windowStart == 1 && direction == -1) || (windowStart == upperLimit && direction == 1)) { return; }
    windowStart += direction;
    fillOptionText();
}




// -- Private methods.
void ListMenu::fillVertexBuffer(){
    float halfWidth = width / 2;
    float halfHeight = height / 2;
    
    float titleBarY = (29.0f / 32.0f) * height + bottom;
    

    for (int i = 2; i < 10 * 6; i += 6) {
        vertexBuffer[i + 0] = 0.0f;    // -- Z position
        vertexBuffer[i + 1] = 0.0f;
        vertexBuffer[i + 2] = 1.0f;
        vertexBuffer[i + 3] = 0.0f;
    }
    // -- border locations
    vertexBuffer[0] = location.x - halfWidth;      vertexBuffer[1] = location.y - halfHeight;
    vertexBuffer[6] = location.x - halfWidth;      vertexBuffer[7] = location.y + halfHeight;

    vertexBuffer[12] = location.x - halfWidth;      vertexBuffer[13] = location.y - halfHeight;
    vertexBuffer[18] = location.x + halfWidth;      vertexBuffer[19] = location.y - halfHeight;

    vertexBuffer[24] = location.x + halfWidth;      vertexBuffer[25] = location.y + halfHeight;
    vertexBuffer[30] = location.x + halfWidth;      vertexBuffer[31] = location.y - halfHeight;

    vertexBuffer[36] = location.x + halfWidth;      vertexBuffer[37] = location.y + halfHeight;
    vertexBuffer[42] = location.x - halfWidth;      vertexBuffer[43] = location.y + halfHeight;

    // -- title bar location
    vertexBuffer[48] = location.x + halfWidth;      vertexBuffer[49] = titleBarY;
    vertexBuffer[54] = location.x - halfWidth;      vertexBuffer[55] = titleBarY;
}


void ListMenu::fillOptionText(){
    textList.clear();

    float titleHeight = (15.0f / 16.0f) * height + bottom;
    textList.push_back(new TextLabel(glm::vec3(location.x, titleHeight, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), width * 0.9f, title));

    float optionHeight = 7.0f / 8.0f * height;

    int size = (optionStrings.size() >= optionsShown) ? optionsShown : optionStrings.size();

    for (int i = windowStart; i < windowStart + size; i++) {
        float y = optionHeight - ( i * (optionHeight / (optionsShown + 1))) + bottom;
        textList.push_back(
            new TextLabel(
                glm::vec3(location.x, y, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                width * 0.9f,
                optionStrings[i - 1]
            )
        );
    }

}




