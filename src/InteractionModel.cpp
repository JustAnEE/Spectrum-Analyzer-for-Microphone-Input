#include "InteractionModel.h"

InteractionModel::InteractionModel(){
}



std::string InteractionModel::getCurrentListMenuID() {
	return currentListMenuID;
}

void InteractionModel::detectHitWidget(double xpos, double ypos){
	for (InteractionModelSubscriber* sub : subscribers) {
		sub->detectClick(xpos, ypos);
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
