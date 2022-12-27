#ifndef INTERACTIONMODEL_H
#define INTERACTIONMODEL_H

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../Widgets/Hpp/Plot.hpp"
#include "../Hpp/InteractionModelSubscriber.hpp"

class InteractionModel
{

public:
	InteractionModel();


	// -- getters and setters
	std::string getCurrentListMenuID();
	void setCurrentListMenuID(std::string MenuID);

	int getCurrentListMenuOption();
	void setCurrentListMenuOption(int _option);

	Plot* getSelectedPlot();
	void  setSelectedPlot(Plot* _plot);


	// -- Widget Methods.
	void detectClickWidget(double xpos, double ypos);
	void detectScrollWidget(double xpos, double ypos, int direction);

	// -- Pub-Sub methods.
	void addSubscriber(InteractionModelSubscriber* newSub);


private:
	std::string currentListMenuID;
	int currentListMenuOption;

	Plot* selectedPlot;

	std::vector<InteractionModelSubscriber*> subscribers;

	// -- Pub-Sub methods.
	void notifySubscribers();
};



#endif // !INTERACTIONMODEL_H
