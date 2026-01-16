// Queue.cpp
#include "Queue.h"

Queue::Queue() {
    createQueue();
}

Queue::~Queue() {
    destroyQueue();
}

void Queue::createQueue() {
    head = NULL;
}

void Queue::destroyQueue() {
    Node* current = head;
    while(current != NULL) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = NULL;
}

bool Queue::isEmpty() {
    return (head == NULL);
}

// Add to Rear
void Queue::enQueue(Patient newP) {
    Node* newNode = new Node(newP);

    if (head == NULL) {
        head = newNode;
    } else {
        Node* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// Remove from Front
void Queue::deQueue() {
    if (head == NULL) return;

    Node* toDelete = head;
    head = head->next;
    delete toDelete;
}

Patient Queue::getFront() {
    if (head) return head->data;
    return Patient(); // Return empty if null
}

Patient Queue::getRear() {
    if (!head) return Patient();
    Node* temp = head;
    while(temp->next != NULL) temp = temp->next;
    return temp->data;
}