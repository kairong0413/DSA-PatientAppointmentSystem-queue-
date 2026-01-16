// Patient.h
#ifndef PATIENT_H
#define PATIENT_H

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// --- Patient Class ---
class Patient {
private:
    int patientID;
    string name;
    string arrivalTime;
    int priorityLevel;

public:
    Patient();
    Patient(int id, string n, string t, int p);

    int getPatientID() const;
    string getName() const;
    string getArrivalTime() const;
    int getPriorityLevel() const;
    void displayPatient() const;
};

// --- Node Class ---
// Defines the link between patients
class Node {
public:
    Patient data;
    Node* next;

    Node(Patient p) {
        data = p;
        next = NULL;
    }
};

#endif