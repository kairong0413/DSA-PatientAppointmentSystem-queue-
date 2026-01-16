#include "LLClinicQueueSystem.h"
#include <sstream> 

// Color definitions
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

LLClinicQueueSystem::LLClinicQueueSystem() : Queue() {
    // Constructor
}

int getIntegerInput() {
    string line;
    int value;
    while (true) {
        getline(cin, line);
        if (line.empty()) continue; 
        
        stringstream ss(line);
        if (ss >> value && ss.eof()) {
            return value;
        } else {
            cout << RED << "Invalid input. Please enter a valid number: " << RESET;
        }
    }
}

void waitForUser() {
    cout << "\nPress [Enter] to continue...";
    string dummy;
    getline(cin, dummy); 
    cout << endl;
}

void LLClinicQueueSystem::displayHeader(const string& title) const {
    cout << BOLD << CYAN << "\n=== " << title << " ===\n" << RESET;
    cout << BOLD << left << setw(10) << "ID" << setw(20) << "NAME" 
         << setw(15) << "ARRIVAL" << setw(8) << "PRIORITY" << RESET << endl;
    cout << string(55, '-') << endl;
}

void LLClinicQueueSystem::swapData(Node* a, Node* b) {
    Patient temp = a->data;
    a->data = b->data;
    b->data = temp;
}

void LLClinicQueueSystem::loadFromFile(const string& filename) {
    ifstream infile(filename);
    if(!infile.is_open()) return;

    string line, name, time, temp;
    int id, priority;

    while(getline(infile, line)) {
        if (line.empty()) continue; 
        stringstream ss(line);
        getline(ss, temp, ','); 
        if(temp.empty()) continue;

        try {
            id = stoi(temp);
            getline(ss, name, ',');
            getline(ss, time, ',');
            getline(ss, temp, ','); priority = stoi(temp);
        
            Patient newPatient(id, name, time, priority);
            enQueue(newPatient); 
        } catch (...) { continue; }
    }
    infile.close();
}

void LLClinicQueueSystem::saveAllToFile() const {
    ofstream outfile("patients.txt", ios::trunc); 
    if (!outfile) {
        cout << RED << "Error: Cannot save to file.\n" << RESET;
        return;
    }

    Node* temp = head;
    while (temp != NULL) {
        outfile << temp->data.getPatientID() << "," 
                << temp->data.getName() << "," 
                << temp->data.getArrivalTime() << "," 
                << temp->data.getPriorityLevel() << "\n";
        temp = temp->next;
    }
    outfile.close();
}

// --- MAIN OPERATIONS ---

void LLClinicQueueSystem::addPatient() {
    displayHeader("ADD NEW PATIENT");

    int id, priority;
    string name, time;
    
    // 1. ID INPUT
    while (true) {
        cout << "Enter ID: ";
        id = getIntegerInput(); 
        
        // Check Duplicate
        bool duplicate = false;
        Node* temp = head; 
        while(temp != NULL) {
            if (temp->data.getPatientID() == id) {
                cout << RED << "Error: ID " << id << " exists. Try again.\n" << RESET;
                duplicate = true;
                break;
            }
            temp = temp->next;
        }
        if (!duplicate) break; 
    }

    cout << "Enter Name: "; 
    getline(cin, name);
    
    cout << "Enter Arrival Time: "; 
    getline(cin, time);
    
    // 3. PRIORITY INPUT
    while (true) {
        cout << "Enter Priority (1-5): ";
        priority = getIntegerInput(); // Safe input
        if (priority >= 1 && priority <= 5) break;
        cout << RED << "Enter 1-5 only.\n" << RESET;
    }

    Patient newP(id, name, time, priority);
    enQueue(newP);
    saveAllToFile();
    
    cout << GREEN << "Patient enqueued successfully.\n" << RESET;
    waitForUser(); 
}

void LLClinicQueueSystem::deletePatient() {
    displayHeader("DELETE PATIENT");

    if (isEmpty()) {
        cout << "Queue is empty.\n";
        waitForUser();
        return;
    }

    int choice;
    while (true) {
        cout << "1. Normal Dequeue (First)\n";
        cout << "2. Delete by ID\n";
        cout << "Choice: ";
        choice = getIntegerInput();

        if (choice == 1 || choice == 2) break;
        cout << RED << "Invalid input.\n" << RESET;
    }

    bool deleted = false;

    if (choice == 1) {
        cout << GREEN << "Dequeuing: " << getFront().getName() << "\n" << RESET;
        deQueue();
        deleted = true;
    } 
    else if (choice == 2) {
        int targetID;
        cout << "Enter ID to delete: "; 
        targetID = getIntegerInput();

        // CASE A: The target is the HEAD node
        if (head->data.getPatientID() == targetID) {
            string removedName = head->data.getName();
            
            deQueue(); 
            
            cout << GREEN << "Successfully deleted ID " << targetID << " (" << removedName << ").\n" << RESET;
            deleted = true;
        } 
        else {
            // CASE B: The target is inside the list
            Node* current = head;
            while (current->next != nullptr && current->next->data.getPatientID() != targetID) {
                current = current->next;
            }

            if (current->next == nullptr) {
                cout << RED << "ID not found.\n" << RESET;
            } else {
                Node* toDelete = current->next;
                
                string removedName = toDelete->data.getName();
                
                current->next = toDelete->next;
                delete toDelete;
                
                cout << GREEN << "Successfully deleted ID " << targetID << " (" << removedName << ").\n" << RESET;
                deleted = true;
            }
        }
    }

    if (deleted) {
        saveAllToFile();
        cout << GREEN << "File updated.\n" << RESET;
    }
    waitForUser();
}

// --- SORTING ---

void LLClinicQueueSystem::displayByArrivalOrder() {
    if (!head) { cout << "Queue is empty.\n"; waitForUser(); return; }

    bool swapped;
    Node* ptr1;
    Node* lastptr = NULL;
    do {
        swapped = false;
        ptr1 = head;
        while (ptr1->next != lastptr) {
            if (ptr1->data.getArrivalTime() > ptr1->next->data.getArrivalTime()) {
                swapData(ptr1, ptr1->next);
                swapped = true;
            }
            ptr1 = ptr1->next;
        }
        lastptr = ptr1;
    } while (swapped);
    saveAllToFile();

    displayHeader("Arrival Order");
    Node* current = head;
    while (current) {
        current->data.displayPatient();
        current = current->next;
    }
    waitForUser();
}

void LLClinicQueueSystem::displayByPriority() {
    if (!head) { cout << "Queue is empty.\n"; waitForUser(); return; }

    Node* temp = head;
    while (temp) {
        Node* maxNode = temp;
        Node* curr = temp->next;
        while (curr) {
            if (curr->data.getPriorityLevel() > maxNode->data.getPriorityLevel()) {
                maxNode = curr;
            }
            curr = curr->next;
        }
        swapData(temp, maxNode);
        temp = temp->next;
    }
    saveAllToFile();

    displayHeader("Priority Order");
    Node* current = head;
    while (current) {
        current->data.displayPatient();
        current = current->next;
    }
    waitForUser();
}

void LLClinicQueueSystem::displayByIDInAscendingOrder() {
    if (!head) { cout << "Queue is empty.\n"; waitForUser(); return; }

    bool swapped;
    Node* ptr1;
    Node* lastptr = NULL;
    do {
        swapped = false;
        ptr1 = head;
        while (ptr1->next != lastptr) {
            if (ptr1->data.getPatientID() > ptr1->next->data.getPatientID()) {
                swapData(ptr1, ptr1->next);
                swapped = true;
            }
            ptr1 = ptr1->next;
        }
        lastptr = ptr1;
    } while (swapped);
    saveAllToFile();

    displayHeader("ID Order");
    Node* current = head;
    while (current) {
        current->data.displayPatient();
        current = current->next;
    }
    waitForUser();
}

void LLClinicQueueSystem::displayAlphabetically() {
    if (head == NULL || head->next == NULL) {
        displayHeader("Alphabetical Order");
        if(head) head->data.displayPatient();
        waitForUser();
        return;
    }
    Node* sorted = NULL;
    Node* curr = head;   
    while (curr != NULL) {
        Node* nextNode = curr->next;
        if (sorted == NULL || sorted->data.getName() >= curr->data.getName()) {
            curr->next = sorted;
            sorted = curr;
        } else {
            Node* temp = sorted;
            while (temp->next != NULL && temp->next->data.getName() < curr->data.getName()) {
                temp = temp->next;
            }
            curr->next = temp->next;
            temp->next = curr;
        }
        curr = nextNode;
    }
    head = sorted;
    saveAllToFile();

    displayHeader("Alphabetical Order");
    Node* current = head;
    while (current) {
        current->data.displayPatient();
        current = current->next;
    }
    waitForUser();
}

// --- SEARCHING ---

void LLClinicQueueSystem::searchByID() const {
    displayHeader("SEARCH BY ID");
    
    cout << "Enter ID to search: "; 
    int target = getIntegerInput(); // Safe input

    Node* current = head;
    while (current != NULL) {
        if (current->data.getPatientID() == target) {
            cout << "\nMatch Found:\n";
            cout << string(55, '-') << endl;
            current->data.displayPatient();
            
            cout << "\nPress [Enter] to continue...";
            string dummy; getline(cin, dummy);
            return;
        }
        current = current->next;
    }
    cout << RED << "ID not found.\n" << RESET;
    
    cout << "\nPress [Enter] to continue...";
    string dummy; getline(cin, dummy);
}

Node* getMiddle(Node* start, Node* last) {
    if (start == nullptr) return nullptr;
    Node* slow = start;
    Node* fast = start->next;
    while (fast != last) {
        fast = fast->next;
        if (fast != last) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    return slow;
}

void LLClinicQueueSystem::searchByName() {
    displayAlphabetically(); 

    displayHeader("SEARCH BY NAME");
    
    string target;
    cout << "Enter Name: "; 
    getline(cin, target);
    
    Node* start = head;
    Node* last = NULL;
    bool found = false;
    
    do {
        Node* mid = getMiddle(start, last);
        if (mid == NULL) break;
        if (mid->data.getName() == target) {
            cout << GREEN << "Found:\n" << RESET;
            mid->data.displayPatient();
            found = true;
            break;
        }
        else if (mid->data.getName() > target) last = mid;
        else start = mid->next;
    } while (last == nullptr || last != start);

    if (!found) { cout << RED << "Name not found.\n" << RESET; }
    waitForUser();
}

void LLClinicQueueSystem::viewFirstPatientInQueue() {
    displayHeader("NEXT PATIENT (FRONT)");

    if (isEmpty()) {
        cout << RED << "The queue is currently empty.\n" << RESET;
        waitForUser();
        return;
    }

    cout << GREEN << "Currently at the front of the queue:\n" << RESET;
    Patient p = getFront(); 
    p.displayPatient(); 
    
    waitForUser();
}

void LLClinicQueueSystem::viewLastPatientInQueue() {
    displayHeader("LATEST PATIENT (REAR)");

    if (isEmpty()) {
        cout << RED << "The queue is currently empty.\n" << RESET;
        waitForUser();
        return;
    }

    cout << CYAN << "Currently at the back of the queue:\n" << RESET;
    Patient p = getRear(); 
    p.displayPatient(); 
    
    waitForUser();
}

// --- MENUS ---

void LLClinicQueueSystem::adminMenu() {
    int choice = -1; 
    while(true) {
        cout << BLUE << "\n[ ADMIN MENU ]\n"
             << "1. Add Patient\n"
             << "2. Delete Patient\n"
             << "3. By Arrival\n"
             << "4. By Priority\n"
             << "5. By ID\n"
             << "0. Back\n" << RESET;
        
        cout << "Choice(0-5): ";
        choice = getIntegerInput(); // Safe Input

        if (choice == 0) break;
        
        switch(choice) {
            case 1: addPatient(); break;
            case 2: deletePatient(); break;
            case 3: displayByArrivalOrder(); break;
            case 4: displayByPriority(); break;
            case 5: displayByIDInAscendingOrder(); break;
            default: 
                cout << RED << "Invalid choice.\n" << RESET;
                waitForUser(); 
        }
    }
}

void LLClinicQueueSystem::userMenu() {
   int choice = -1;
    while(true) {
        cout << BLUE << "\n[ USER MENU ]\n"
             << "1. Search ID\n"
             << "2. Search Name\n"
             << "3. Display A-Z\n"
             << "4. View First Patient In Queue (Next)\n" 
             << "5. View Last Patient In Queue (New)\n"  
             << "0. Back\n" << RESET;
        
        cout << "Choice(0-5): ";
        choice = getIntegerInput(); 

        if (choice == 0) break;
        
        switch(choice) {
            case 1: searchByID(); break;
            case 2: searchByName(); break;
            case 3: displayAlphabetically(); break;
            case 4: viewFirstPatientInQueue(); break; 
            case 5: viewLastPatientInQueue(); break;  
            default: 
                cout << RED << "Invalid choice.\n" << RESET;
                waitForUser();
        }
    }
}

void LLClinicQueueSystem::mainMenu() {
    int choice = -1;
    while(true) {
        cout << BLUE << "\n=== PATIENT APPOINTMENT SYSTEM ===\n"
             << "1. Admin\n"
             << "2. User\n"
             << "0. Exit\n" << RESET;
        
        cout << "Choice(0-2): "; 
        choice = getIntegerInput(); 
        if (choice == 0) {
            cout << "Exiting...\n";
            break;
        }
        else if(choice == 1) adminMenu();
        else if(choice == 2) userMenu();
        else {
            cout << RED << "Invalid choice.\n" << RESET;
            waitForUser();
        }
    }
}