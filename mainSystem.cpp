#include "LLClinicQueueSystem.h"

int main() {
   
    LLClinicQueueSystem system;

    system.loadFromFile("patients.txt");

    system.mainMenu();

    return 0;
}