#include "Patient.h"

#define RESET   "\033[0m"
#define WHITE   "\033[37m"

Patient::Patient() {}

Patient::Patient(int id, string n, string t, int p)
    : patientID(id), name(n), arrivalTime(t), priorityLevel(p) {}

int Patient::getPatientID() const { return patientID; }
string Patient::getName() const { return name; }
string Patient::getArrivalTime() const { return arrivalTime; }
int Patient::getPriorityLevel() const { return priorityLevel; }

void Patient::displayPatient() const {
    cout << WHITE << left << setw(10) << patientID 
         << setw(20) << name 
         << setw(15) << arrivalTime 
         << setw(8) << priorityLevel << RESET << endl;
}