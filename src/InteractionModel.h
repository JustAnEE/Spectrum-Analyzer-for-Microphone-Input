#ifndef INTERACTIONMODEL_H
#define INTERACTIONMODEL_H

#include<iostream>
#include<vector>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"InteractionModelSubscriber.h"


class InteractionModel{

public:
	InteractionModel();


	// -- getters and setters
	std::string getCurrentListMenuID();

	// -- Widget Methods.
	void detectHitWidget(double xpos, double ypos);
	void swapListMenu(std::string MenuID);

	// -- Pub-Sub methods.
	void addSubscriber(InteractionModelSubscriber* newSub);


private:
	std::string currentListMenuID;
	std::vector<InteractionModelSubscriber*> subscribers;

	// -- Pub-Sub methods.
	void notifySubscribers();
};



#endif // !INTERACTIONMODEL_H
