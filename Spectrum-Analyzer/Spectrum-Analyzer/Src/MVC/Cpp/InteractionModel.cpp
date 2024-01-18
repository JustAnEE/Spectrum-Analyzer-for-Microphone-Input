#include "../Hpp/InteractionModel.hpp"

InteractionModel::InteractionModel()
{
    currentListMenuOption = 0;
    currentListMenuID = "Method";
}



std::string InteractionModel::getCurrentListMenuID()
{
    return currentListMenuID;
}

void InteractionModel::setCurrentListMenuID(std::string MenuID)
{
    currentListMenuID = MenuID;
    notifySubscribers();
}

int InteractionModel::getCurrentListMenuOption()
{
    return currentListMenuOption;
}

void InteractionModel::setCurrentListMenuOption(int _option)
{
    currentListMenuOption = _option;
    notifySubscribers();
}

Plot* InteractionModel::getSelectedPlot()
{
    return selectedPlot;
}

void InteractionModel::setSelectedPlot(Plot* _plot)
{
    selectedPlot = _plot;
    notifySubscribers();
}

void InteractionModel::detectClickWidget(double xpos, double ypos)
{
    for (InteractionModelSubscriber* sub : subscribers)
    {
        sub->detectClick(xpos, ypos);
    }
}

void InteractionModel::detectScrollWidget(double xpos, double ypos, int direction)
{
    for (InteractionModelSubscriber* sub : subscribers)
    {
        sub->detectScroll(xpos, ypos, direction);
    }
}

void InteractionModel::addSubscriber(InteractionModelSubscriber* newSub)
{
    subscribers.push_back(newSub);
}

void InteractionModel::notifySubscribers()
{
    for (InteractionModelSubscriber* sub : subscribers)
    {
        sub->IModelChanged();
    }
}
