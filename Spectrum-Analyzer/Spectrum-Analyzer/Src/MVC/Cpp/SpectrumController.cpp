#include "../Hpp/SpectrumController.hpp"

SpectrumController::SpectrumController(){
    eMyControllerState = START;
}

void SpectrumController::setDModel(SpectrumModel* _model) {
    model = _model; 
}
void SpectrumController::setIModel(InteractionModel* _IModel) {
    IModel = _IModel;
}


// -- Event handling methods.
void SpectrumController::jobStartEvent(){
    switch (eMyControllerState){
        case START:
            // -- Initial setup featuring the worlds longest function.
            model->addPlot(
                5, 5,
                SpectrumModel::TIMESERIES,
                0, 0, false, false
            );
            IModel->setSelectedPlot(model->getPlotVector()[0]);
            eMyControllerState = READING;
            break;

        case READING:
            // -- Read data from device, then swap state to PROCCESSING
            model->readMicData();
            eMyControllerState = PROCESSING;
            break;

        case PROCESSING:
            // -- process data then swap state to READING
            model->processData();
            eMyControllerState = READING;
            break;

        default:
            break;
    }
}

void SpectrumController::handleKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        if (model->getPlotVector().size() < 4) {
            model->addPlot(5, 5, SpectrumModel::TIMESERIES, 0, 0, false, false);
        }
    }
    else if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
        if (model->getPlotVector().size() > 1) {
            model->removePlot(IModel->getSelectedPlot());
            IModel->setSelectedPlot(model->getPlotVector()[0]);
        }
    }
}

void SpectrumController::handleMouseClick(GLFWwindow* window, int button, int action, int mods){
    // -- mouse position on click
    if (action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        Plot* clickedPlot = model->detectClickPlot((xpos * 2 / 1000) - 1, ((ypos * 2 / 900) - 1) * -1);
        if (clickedPlot != nullptr) {
            IModel->setSelectedPlot(clickedPlot);
            // -- The bellow code is gross.\
            IModel->setSelectedPlot(clickedPlot);
            if (IModel->getCurrentListMenuID() == "Filter") {
                IModel->setCurrentListMenuOption(clickedPlot->getFilterFlag());
            }
            else if (IModel->getCurrentListMenuID() == "Window") {
                IModel->setCurrentListMenuOption(clickedPlot->getWindowFlag());
            }
            else if (IModel->getCurrentListMenuID() == "Method") {
                IModel->setCurrentListMenuOption(clickedPlot->getMethodFlag());
            }
            else {
                std::cout << "ERROR IN SpectrumController::handleMouseClick.     IModel->getCurrentListMenuID() not a valid ID.";
                exit(-9); /* CRASH HARD. */
            }
        }

        IModel->detectClickWidget((xpos * 2 / 1000) - 1, ((ypos * 2 / 900) - 1) * -1);
    }
    
}

void SpectrumController::handleMouseScroll(GLFWwindow* window, double xoffset, double yoffset){
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int direction = (yoffset >= 0.0) ? 1 : -1;

    IModel->detectScrollWidget((xpos * 2 / 1000) - 1, ((ypos * 2 / 900) - 1) * -1, direction);
}


void SpectrumController::handleListButton(std::string buttonID, int type) {
    if (IModel->getSelectedPlot() == nullptr) { return; }

    IModel->setCurrentListMenuID(buttonID);
    int option = 0;

    if (buttonID == "Filter") { option = IModel->getSelectedPlot()->getFilterFlag(); }
    else if (buttonID == "Window") { option = IModel->getSelectedPlot()->getWindowFlag(); }
    else if (buttonID == "Method") { option = IModel->getSelectedPlot()->getMethodFlag(); }
    else {
        std::cout << "ERROR IN SpectrumController::handleListButton.     buttonID not found.";
        exit(-10); /* CRASH HARD. */
    }

    IModel->setCurrentListMenuOption(option);
}

void SpectrumController::handleBooleanButton(std::string buttonID, int type){
    if (IModel->getSelectedPlot() == nullptr) { return; }
    
    bool detrendFlag = IModel->getSelectedPlot()->getDetrendFlag();

    if (buttonID == "Detrend") { IModel->getSelectedPlot()->setDetrendFlag(!detrendFlag); }
    //else if (buttonID == "Normalize") {}
    else {
        std::cout << "ERROR IN SpectrumController::handleBooleanButton.     buttonID not found.";
        exit(-11); /* CRASH HARD. */
    }
}

void SpectrumController::handleListMenu(std::string ListID, int optionNum) {
    if (IModel->getSelectedPlot() == nullptr) { return; }
    
    if      (ListID == "Filter") { IModel->getSelectedPlot()->setFilterFlag(optionNum); }
    else if (ListID == "Window") { IModel->getSelectedPlot()->setWindowFlag(optionNum); }
    else if (ListID == "Method") { IModel->getSelectedPlot()->setMethodFlag(optionNum); }
    else { 
        std::cout << "ERROR IN SpectrumController::handleListMenu.    ListID not found.";
        exit(-12); /* CRASH HARD. */
    }
    if (optionNum != -1) {
        IModel->setCurrentListMenuOption(optionNum);
    }
}



