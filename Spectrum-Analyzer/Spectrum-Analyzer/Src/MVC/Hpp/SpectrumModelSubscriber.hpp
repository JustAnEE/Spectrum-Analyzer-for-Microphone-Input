#ifndef SMS_H
#define SMS_H
#include<iostream>

/* Interface for subscribers to get notified of model change. */
class SpectrumModelSubscriber 
{

   public:
      virtual ~SpectrumModelSubscriber() = default;

   public:
      virtual void DModelChanged() = 0;

};



#endif // !SMS_H

