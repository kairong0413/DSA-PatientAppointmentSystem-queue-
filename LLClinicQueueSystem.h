// LLClinicQueueSystem.h
#ifndef LLCLINICQUEUESYSTEM_H
#define LLCLINICQUEUESYSTEM_H

#include "Queue.h" // Inherit from Queue
#include <fstream>
#include <sstream>

class LLClinicQueueSystem : public Queue {
public:
    LLClinicQueueSystem();
    // Destructor is not needed here because Queue destructor handles memory
    
    // Core Features
    void loadFromFile(const string& filename);
    void addPatient();      // Uses enQueue + File Save
    void deletePatient();   // Uses deQueue/Custom logic + File Save
    void saveAllToFile() const;

    // Sorting
    void swapData(Node* a, Node* b);
    void displayByArrivalOrder();
    void displayByPriority();
    void displayByIDInAscendingOrder();
    void displayAlphabetically();

    // Searching
    void searchByID() const;
    void searchByName();

    //queue function
    void viewFirstPatientInQueue();
    void viewLastPatientInQueue();

    // UI Helpers
    void displayHeader(const string& title) const;
    void adminMenu();
    void userMenu();
    void mainMenu();
};

#endif