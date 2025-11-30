#include <stdio.h>
#include <string.h>
#include "subscription.h"
#include "notice.h"
#include "utils.h"

// Original 3D Array for subscriptions
char subscriptions[MAX_STUDENTS][MAX_SUBS_PER_STUDENT][MAX_DEPT];
int subCount[MAX_STUDENTS] = {0};

// NEW: Parallel arrays to handle Alphanumeric IDs
// This maps a String ID (e.g., "A123") to an integer index (0, 1, 2...)
char studentIDs[MAX_STUDENTS][ID_LEN];
int totalStudents = 0;

// Helper function: Finds the index of a student ID string
// Returns -1 if the student is not found
int getStudentIndex(char *id) {
    for (int i = 0; i < totalStudents; i++) {
        if (strcmp(studentIDs[i], id) == 0) {
            return i;
        }
    }
    return -1;
}

void subscribeStudent() {
    char id[ID_LEN];
    char dept[MAX_DEPT];
    int index;

    printf("Student ID (e.g. A123): ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0; // Remove newline character

    // 1. Check if this student ID already exists in our database
    index = getStudentIndex(id);

    // 2. If not found, register them as a new student
    if (index == -1) {
        if (totalStudents >= MAX_STUDENTS) {
            printf("Error: Database full. Cannot register new student.\n");
            return;
        }
        
        // Assign them the next available slot
        index = totalStudents;
        strcpy(studentIDs[index], id); // Save their text ID
        subCount[index] = 0;           // Initialize their sub count
        totalStudents++;
    }

    // 3. Check subscription limit (Max 5)
    if (subCount[index] >= MAX_SUBS_PER_STUDENT) {
        printf("You can't subscribe to more departments (Max 5).\n");
        return;
    }

    printf("Department (e.g. CSE): ");
    fgets(dept, sizeof(dept), stdin);
    dept[strcspn(dept, "\n")] = 0;

    // 4. Check if already subscribed to this department
    for (int i = 0; i < subCount[index]; i++) {
        if (strcmp(subscriptions[index][i], dept) == 0) {
            printf("You are already subscribed to %s.\n", dept);
            return;
        }
    }

    // 5. Save the subscription
    // We use 'index' (the integer) to access the 3D array
    strcpy(subscriptions[index][subCount[index]], dept);
    subCount[index]++;
    printf("Subscribed!\n");
}

void showNotifications() {
    char id[ID_LEN];
    int index;

    printf("Enter ID to see your feed: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    // 1. Find the integer index for this string ID
    index = getStudentIndex(id);

    // If index is -1, the ID was never registered
    if (index == -1) {
        printf("ID not found. Subscribe to a department first.\n");
        return;
    }

    if (subCount[index] == 0) {
        printf("No subscriptions found.\n");
        return;
    }

    printf("\n--- YOUR FEED ---\n");
    int foundAny = 0;

    // 2. Check every notice against the student's subscriptions
    for (int i = 0; i < totalNotices; i++) {
        for (int j = 0; j < subCount[index]; j++) {
            // Compare notice department with student's subscribed department
            if (strcmp(subscriptions[index][j], notices[i].department) == 0) {
                printf("-> [%s] %s\n", notices[i].department, notices[i].title);
                foundAny = 1;
                break; // Break inner loop to avoid printing same notice twice
            }
        }
    }
    
    if(!foundAny) printf("No new updates.\n");
}