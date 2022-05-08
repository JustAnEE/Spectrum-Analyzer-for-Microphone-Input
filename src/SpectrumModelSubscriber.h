#ifndef SMS_H
#define SMS_H
#include<iostream>

/* Interface for subscribers to get notified of model change. */
class SpectrumModelSubscriber {
public:
	virtual void modelChanged() = 0;
};


#endif // !SMS_H

