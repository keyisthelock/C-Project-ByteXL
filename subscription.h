#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H

#define MAX_STUDENTS 1000
#define MAX_DEPT 50
#define MAX_SUBS_PER_STUDENT 5 // Each student can follow up to 5 depts
#define ID_LEN 20              // Added: Max length for Alphanumeric ID

// 3D Array: [Student_Index][Slot_Number][Dept_Name]
extern char subscriptions[MAX_STUDENTS][MAX_SUBS_PER_STUDENT][MAX_DEPT];
extern int subCount[MAX_STUDENTS];

// New: Parallel array to map "String ID" to "Student Index"
extern char studentIDs[MAX_STUDENTS][ID_LEN];
extern int totalStudents;

void subscribeStudent();
void showNotifications();

#endif