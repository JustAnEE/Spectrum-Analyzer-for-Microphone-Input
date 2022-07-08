#include "InteractionModel.h"

InteractionModel::InteractionModel(){}



std::string InteractionModel::getCurrentListMenuID() {
	return currentListMenuID;
}

Plot* InteractionModel::getSelectedPlot(){
	return selectedPlot;
}

void InteractionModel::setSelectedPlot(Plot* _plot){
	selectedPlot = _plot;
	notifySubscribers();
}

void InteractionModel::detectClickWidget(double xpos, double ypos){
	for (InteractionModelSubscriber* sub : subscribers) {
		sub->detectClick(xpos, ypos);
	}
}

void InteractionModel::detectScrollWidget(double xpos, double ypos, int direction) {
	for (InteractionModelSubscriber* sub : subscribers) {
		sub->detectScroll(xpos, ypos, direction);
	}
}


void InteractionModel::swapListMenu(std::string MenuID){
	currentListMenuID = MenuID;
	notifySubscribers();
}


void InteractionModel::addSubscriber(InteractionModelSubscriber* newSub){
	subscribers.push_back(newSub);
}

void InteractionModel::notifySubscribers(){
	for (InteractionModelSubscriber* sub : subscribers) {
		sub->IModelChanged();
	}
}
