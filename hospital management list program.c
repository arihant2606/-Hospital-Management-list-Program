#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations
typedef struct Patient Patient;
typedef struct Doctor Doctor;
typedef struct Department Department;

struct Patient {
    int patientID; // First 5 digits will be DoctorID, next 3 are patient specific
    char name[100];
    char appointment[11]; // Date format: YYYY-MM-DD
    Patient* next;
};

struct Doctor {
    int doctorID; // First 2 digits will be DeptID, next 3 are doctor specific
    char name[100];
    Patient* patients;
    Doctor* next;
};

struct Department {
    int deptID;
    char name[100];
    Doctor* doctors;
    Department* next;
};

// Function Prototypes
void addDepartment(Department** head, int deptID, char* name);
void addDoctor(Department* head, int deptID, int doctorID, char* name);
void addPatient(Department* head, int doctorID, int patientID, char* name, char* appointment);
void displayDepartments(Department* head);
void displayDoctors(Department* head);
void displayPatients(Department* head);
void userInterface(Department** departmentHead);


// Main Function
int main() {
    Department* departments = NULL;
    userInterface(&departments);
    // Memory cleanup should be implemented here for departments
     // Free allocated memory
    Department* currentDept = departments;
    while (currentDept != NULL) {
        Doctor* currentDoc = currentDept->doctors;
        while (currentDoc != NULL) {
            Patient* currentPat = currentDoc->patients;
            while (currentPat != NULL) {
                Patient* tempPat = currentPat;
                currentPat = currentPat->next;
                free(tempPat);
            }
            Doctor* tempDoc = currentDoc;
            currentDoc = currentDoc->next;
            free(tempDoc);
        }
        Department* tempDept = currentDept;
        currentDept = currentDept->next;
        free(tempDept);
    }

    return 0;
}

void userInterface(Department** departmentHead) {
    int choice;
    do {
        printf("\nHospital Management System\n");
        printf("1. Add Department\n");
        printf("2. Add Doctor\n");
        printf("3. Add Patient\n");
        printf("4. Display Departments\n");
        printf("5. Display Doctors\n");
        printf("6. Display Patients\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        int deptID, doctorID, patientID;
        char name[100], appointment[11];

        switch (choice) {
            case 1:
              printf("Enter Department ID(in two digits):");
                scanf("%d", &deptID);
                 printf("Enter Department Name:");
                scanf(" %[^\n]s", name);
                
                addDepartment(departmentHead, deptID, name);
                break;
            case 2:
            displayDepartments(*departmentHead);
                printf("Enter Department ID:");
                scanf("%d",&deptID);
                printf(" Doctor ID (Department ID+three digits): ");
                scanf(" %d",&doctorID);
                printf("Doctor Name: ");
                scanf(" %[^\n]s", name);
                addDoctor(*departmentHead, deptID, doctorID, name);
                break;
            case 3:
            displayDoctors(*departmentHead);
                printf("Enter Doctor ID: ");
                scanf(" %d", &doctorID);
                 printf("Patient ID (Doctor ID+three digits: ");
                scanf(" %d",&patientID);
                 printf("Enter Name: ");
                scanf(" %[^\n]s", name);
                 printf("Appointment Date (YYYY-MM-DD): ");
                scanf(" %s",appointment);
                addPatient(*departmentHead, doctorID, patientID, name, appointment);
                break;
            case 4:
                displayDepartments(*departmentHead);
                break;
            case 5:
                displayDoctors(*departmentHead);
                break;
            case 6:
            
                displayPatients(*departmentHead);
                break;
            case 7:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);
    ;
}

// Add Department Function
void addDepartment(Department** head, int deptID, char* name) {
    Department* newDept = (Department*)malloc(sizeof(Department));
    newDept->deptID = deptID;
    strcpy(newDept->name, name);
    newDept->doctors = NULL;
    newDept->next = *head;
    *head = newDept;
}

// Add Doctor Function
void addDoctor(Department* head, int deptID, int doctorID, char* name) {
  
    // Find the department
    while (head != NULL && head->deptID != deptID) {
        head = head->next;
    }
    if (head == NULL) {
        printf("Department not found.\n");
        return;
    }
    // Add the doctor to the department
    Doctor* newDoc = (Doctor*)malloc(sizeof(Doctor));
    newDoc->doctorID = doctorID;
    strcpy(newDoc->name, name);
    newDoc->patients = NULL;
    newDoc->next = head->doctors;
    head->doctors = newDoc;
}

// Add Patient Function
void addPatient(Department* head, int doctorID, int patientID, char* name, char* appointment) {
    
   
    int deptID = doctorID / 1000; // Assuming doctorID format is DDDPP, where DDD is doctor and PP is patient
    // Find the department
    while (head != NULL && head->deptID != deptID) {
        head = head->next;
    }
    if (head == NULL) {
        printf("Department not found.\n");
        return;
    }
    // Find the doctor
    Doctor* doc = head->doctors;
    while (doc != NULL && doc->doctorID != doctorID) {
        doc = doc->next;
    }
    if (doc == NULL) {
        printf("Doctor not found in Department %d.\n", deptID);
        return;
    }
    // Add the patient to the doctor
    Patient* newPat = (Patient*)malloc(sizeof(Patient));
    newPat->patientID = patientID;
    strcpy(newPat->name, name);
    strcpy(newPat->appointment, appointment);
    newPat->next = doc->patients;
    doc->patients = newPat;
}

// Display Departments Function
void displayDepartments(Department* head) {
    printf("\nDepartments:\n");
    printf("DeptID|\t\tName\n");
    while (head != NULL) {
        printf("%d\t|\t\t%s\n", head->deptID, head->name);
        head = head->next;
    }
}

// Display Doctors Function
void displayDoctors(Department* head) {
    printf("\nDoctors:\n");
    printf("DocID\t\t|Name\t\t|Department\n");
    while (head != NULL) {
        Doctor* doc = head->doctors;
        while (doc != NULL) {
            printf("%d\t\t|%s\t\t|%s\n", doc->doctorID, doc->name, head->name);
            doc = doc->next;
        }
        head = head->next;
    }
}

// Display Patients Function
void displayPatients(Department* head) {
    printf("\nPatients:\n");
    printf("patientID |\t\tName|\t\tAppointment|\t\tDoctor\n");
    while (head != NULL) {
        Doctor* doc = head->doctors;
        while (doc != NULL) {
            Patient* pat = doc->patients;
            while (pat != NULL) {
                printf(" %d|\t\t%s|\t\t%s|\t\t%s\n", pat->patientID, pat->name, pat->appointment, doc->name);
                pat = pat->next;
            }
            doc = doc->next;
        }
        head = head->next;
    }
}
