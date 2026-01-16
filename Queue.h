#ifndef QUEUE_H
#define QUEUE_H

#include "Patient.h"

class Queue {
protected:
    Node* head; // Protected so LLClinicQueueSystem can access it for sorting

public:
    Queue();  
    ~Queue(); 
    
    void createQueue();
    void destroyQueue();
    bool isEmpty();
    void enQueue(Patient newP);
    void deQueue();
    Patient getFront();
    Patient getRear();
};

#endif