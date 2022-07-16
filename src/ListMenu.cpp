#include "ListMenu.h"



ListMenu::ListMenu(
        glm::vec3 _location, float _width, float _height, std::string _title, 
        std::vector<std::string> _options, std::string _ID
    ) {
    ID = _ID;

    optionsShown = 6;
    windowStart = 0;

    location = _location;
    width = _width;
    height = _height;
    optionStrings = _options;
    title = _title;

    selectedOption = 0;

    left = location.x - (width / 2);
    right = location.x + (width / 2);
    bottom = location.y - (height / 2);
    top = location.y + (height / 2);

    // -- create title and options TextLables.
    fillOptionText();

    // -- Initialize the vertex buffer
    vertexBufferSizeBytes = 18 * 6 * sizeof(float);
    vertexBuffer = (float*)calloc(18 * 6, sizeof(float));
    fillVertexBuffer();
}


ListMenu::~ListMenu() {
    free(vertexBuffer);
}



void ListMenu::setText(std::string _title, std::vector<std::string> _options) {
    title = _title;
    optionStrings = _options;
    ID = _title;
    windowStart = 0;
    fillOptionText();
}

void ListMenu::setSelectedOption(int value) {
    selectedOption = value;
    fillVertexBuffer();
}


int ListMenu::detectClick(double xpos, double ypos) {
    float range  = 0.05;
    float optionHeight = 7.0f / 8.0f * height;

    int size = (optionStrings.size() < optionsShown) ? optionStrings.size() : optionsShown;

    for(int i = 0; i < size; i++){
        float y = optionHeight - ( (i+1) / 7.0) + bottom;

        if (xpos > left && xpos < right && ypos > y - range && ypos < y + range) {
            return windowStart + i;
        }
    }

    
    return -1;
}


void ListMenu::scrollOptions(double xpos, double ypos, int direction){
    // -- Only scroll if mouse is over the ListMenu
    if( !(xpos > left && xpos < right && ypos > bottom && ypos < top) ) { return; }
    
    // -- Only scroll if there are more options than 'optionsShown'.
    if (optionStrings.size() < optionsShown) { return; }

    // -- Only scroll if we have room to scroll. 
    int upperLimit = optionStrings.size() - optionsShown - 1;
    if (  (windowStart == 0 && direction == -1) || (windowStart == upperLimit && direction == 1)  ) { return; }
    
    windowStart += direction;
    fillOptionText();
}




// -- Private methods.
void ListMenu::fillVertexBuffer(){
    float halfWidth = width / 2;
    float halfHeight = height / 2;

    float selectionBoxWidth  = width * 0.8f / 2.0f;
    float selectionBoxHeight = ((1.0f / 7.0f) * 0.8f) / 2.0f;
    float selectionBoxYpos = (7.0f / 8.0f * height) - ((selectedOption + 1) / 7.0f) + bottom;

    float titleBarY = (29.0f / 32.0f) * height + bottom;
    
    // -- The first 59 vertices are the Listmenu vertices
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

    // -- The last 48 are for the selection box.
    for (int i = 62; i < 18 * 6; i += 6) {
        vertexBuffer[i + 0] = 0.0f;    // -- Z position
        vertexBuffer[i + 1] = 1.0f;
        vertexBuffer[i + 2] = 0.75f;
        vertexBuffer[i + 3] = 0.0f;
    }
    // -- location based on selection button
    vertexBuffer[60] = location.x - selectionBoxWidth;      vertexBuffer[61] = selectionBoxYpos - selectionBoxHeight;
    vertexBuffer[66] = location.x - selectionBoxWidth;      vertexBuffer[67] = selectionBoxYpos + selectionBoxHeight;
    
    vertexBuffer[72] = location.x - selectionBoxWidth;      vertexBuffer[73] = selectionBoxYpos - selectionBoxHeight;
    vertexBuffer[78] = location.x + selectionBoxWidth;      vertexBuffer[79] = selectionBoxYpos - selectionBoxHeight;

    vertexBuffer[84] = location.x + selectionBoxWidth;      vertexBuffer[85] = selectionBoxYpos + selectionBoxHeight;
    vertexBuffer[90] = location.x + selectionBoxWidth;      vertexBuffer[91] = selectionBoxYpos - selectionBoxHeight;

    vertexBuffer[96] = location.x + selectionBoxWidth;      vertexBuffer[97] = selectionBoxYpos + selectionBoxHeight;
    vertexBuffer[102] = location.x - selectionBoxWidth;      vertexBuffer[103] = selectionBoxYpos + selectionBoxHeight;
}


void ListMenu::fillOptionText(){
    clearTextList();

    float titleHeight = (15.0f / 16.0f) * height + bottom;
    float optionHeight = 7.0f / 8.0f * height;
    int size = (optionStrings.size() < optionsShown) ? optionStrings.size() : optionsShown;

    textList.push_back(new TextLabel(glm::vec3(location.x, titleHeight, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), width * 0.9f, title));

    for(int i = 0; i < size; i++){
        float y = optionHeight - ((i + 1) / 7.0) + bottom;
        textList.push_back(
            new TextLabel(
                glm::vec3(location.x, y, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                width * 0.9f,
                optionStrings[windowStart + i]
            )
        );
    }

}




