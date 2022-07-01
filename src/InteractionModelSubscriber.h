#ifndef IMS_H
#define IMS_H
#include<iostream>

/* Interface for subscribers to get notified of model change. */
class InteractionModelSubscriber {
public:
	virtual void IModelChanged() = 0;
	virtual void detectClick(double xpos, double ypos) = 0;
};


#endif // !IMS_H

