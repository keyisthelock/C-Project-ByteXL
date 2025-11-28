#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_DEPT 25 // for maximum department name length
#define MAX_STUDENTS 1000 // for maximum students
#define DATE_LEN 11  // "YYYY-MM-DD"

// Structure to represent a notice
typedef struct {
    char *title;
    char *body;
    char department[MAX_DEPT];
    int semester;
    char poster[100];
    char date[DATE_LEN];  // Format: YYYY-MM-DD
} Notice;

// Global variables
Notice *notices = NULL; // dynamic array of notices
int totalNotices = 0; // current count of notices
int maxNotices = 0; // current allocated size of notices array

// Subscription: student ID -> subscribed departments
char subscriptions[MAX_STUDENTS][MAX_DEPT];
int subCount[MAX_STUDENTS] = {0};
int totalStudents = 0;

// Function prototypes
void postNotice();// to post a new notice
void editNotice();// to edit an existing notice
void deleteNotice();// to delete a notice
void listNotices();// to list all notices
void searchNotices();// to search notices by keyword
void filterByDeptSem();// to filter by department and semester
void filterByDateRange();// to filter the range of the dates
void subscribeStudent();// to subscribe a student to departments
void showNotifications();// to show notifications for a student
char* strduplicate(char *str);// helper to duplicate string
int isValidDate(char *date);// to check if date is valid
int compareDates(char *date1, char *date2); // returns -1, 0, 1
void clearInputBuffer();// to clear input buffer
void ensureCapacity();// to ensure capacity of notices array
void cleanup();// to clean up allocated memory

// Helper: Duplicate string using malloc
char* strduplicate(char *str) {
    char *copy = (char*)malloc(strlen(str) + 1); // to allocate memory
    if (copy) strcpy(copy, str);
    return copy;
}

// Helper: Check if date is valid (YYYY-MM-DD format)
int isValidDate(char *date) {
    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3) return 0;
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    // Basic check (not leap year aware)
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return 0;
    if (month == 2 && day > 29) return 0;
    return 1;
}

// Compare two dates: -1 if date1 < date2, 0 if equal, 1 if date1 > date2
int compareDates(char *date1, char *date2) {
    return strcmp(date1, date2); // Lexicographic works for YYYY-MM-DD
}

// Clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);// discard characters until newline or End Of Function
}

// Dynamic reallocation of notices array
void ensureCapacity() {
    if (totalNotices >= maxNotices) // to check if we need more space
    {
        maxNotices = (maxNotices == 0) ? 2 : maxNotices * 2;// double the size
        notices = (Notice*)realloc(notices, maxNotices * sizeof(Notice));// reallocate memory
        if (!notices) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
    }
}

// Post a new notice
void postNotice() {
    ensureCapacity();// ensure there is enough space

    Notice *n = &notices[totalNotices];// pointer to store the new notice

    char title[200], body[1000], dept[MAX_DEPT], poster[50], date[DATE_LEN];// temporary variables
    int sem;

    printf("\n--- Post a New Notice ---\n");

    printf("Enter title: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;

    printf("Enter body: ");
    fgets(body, sizeof(body), stdin);
    body[strcspn(body, "\n")] = 0;

    printf("Enter department: ");
    fgets(dept, sizeof(dept), stdin);
    dept[strcspn(dept, "\n")] = 0;

    printf("Enter semester (1-8): ");
    scanf("%d", &sem);
    while (sem < 1 || sem > 8) {
        printf("Invalid semester. Enter 1-8: ");
        scanf("%d", &sem);
    }
    clearInputBuffer(); // clear newline from buffer

    printf("Enter name of the person who posted this notice: ");
    fgets(poster, sizeof(poster), stdin);
    poster[strcspn(poster, "\n")] = 0;

    printf("Enter date (YYYY-MM-DD): ");
    fgets(date, sizeof(date), stdin);
    date[strcspn(date, "\n")] = 0;
    while (!isValidDate(date)) {
        printf("Invalid date. Enter again (YYYY-MM-DD): ");
        fgets(date, sizeof(date), stdin);
        date[strcspn(date, "\n")] = 0;
    }

    // Allocate dynamic memory for title and body
    n->title = strduplicate(title);// duplicate title
    n->body = strduplicate(body);// duplicate body
    strcpy(n->department, dept);// copy department
    n->semester = sem;// set semester
    strcpy(n->poster, poster);// copy poster name
    strcpy(n->date, date);// copy date

    totalNotices++;// increment total notices
    printf(" Notice posted successfully!\n");
}

// Edit an existing notice
void editNotice() {
    if (totalNotices == 0) {
        printf("No notices to edit.\n");
        return;
    }

    listNotices();
    int index;
    printf("Enter notice number to edit (1-%d): ", totalNotices);
    scanf("%d", &index);
    clearInputBuffer();// clear newline from buffer

    if (index < 1 || index > totalNotices) // to validate the index
    {
        printf("Invalid notice number.\n");
        return;
    }

    Notice *n = &notices[index - 1];// pointer to the selected notice
    char input[1000];

    printf("Current title: %s\n", n->title);
    printf("Enter new title (or press Enter to skip): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; // remove newline
    if (strlen(input) > 0)
     {
        free(n->title);// free old title memory
        n->title = strduplicate(input); // duplicate new title
    }

    printf("Current body: %s\n", n->body); // display current body
    printf("Enter new body (or press Enter to skip): ");// prompt for new body
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) {
        free(n->body);
        n->body = strduplicate(input); // duplicate new body
    }

    printf(" Notice updated successfully!\n");
}

// Delete a notice
void deleteNotice() {
    if (totalNotices == 0) {
        printf("No notices to delete.\n");
        return;
    }

    listNotices();
    int index;
    printf("Enter notice number to delete (1-%d): ", totalNotices);
    scanf("%d", &index);
    clearInputBuffer();

    if (index < 1 || index > totalNotices) 
    {
        printf("Invalid notice number.\n");
        return;
    }

    Notice *n = &notices[index - 1];// pointer to the selected notice
    free(n->title);// free title memory
    free(n->body);// free body memory

    // Shift remaining notices
    for (int i = index - 1; i < totalNotices - 1; i++) // TO shift the remaining notices to fill the gap
    {
        notices[i] = notices[i + 1];
    }
    totalNotices--;

    printf(" Notice deleted successfully!\n");
}

// List all notices
void listNotices() {
    if (totalNotices == 0) {
        printf(" No notices available.\n");
        return;
    }

    printf("\n All Notices:\n");
    printf("--------------------------------------------------------------\n");
    for (int i = 0; i < totalNotices; i++) {
        printf("%d. [%s] %s\n", i + 1, notices[i].department, notices[i].title);
        printf("   Sem: %d | Date: %s | Posted by: %s\n", notices[i].semester, notices[i].date, notices[i].poster);// to display semester, date, and poster
        printf("   %s\n", notices[i].body);// to display the body of the notice
        printf("--------------------------------------------------------------\n");
    }
}

// Search notices by keyword in title or body
void searchNotices() {
    if (totalNotices == 0) {
        printf("No notices to search.\n");
        return;
    }

    char keyword[100];
    printf("Enter keyword to search: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;
    clearInputBuffer();

    int found = 0;
    printf("\n Search Results for '%s':\n", keyword);
    for (int i = 0; i < totalNotices; i++) {
        if (strstr(notices[i].title, keyword) || strstr(notices[i].body, keyword)) 
        {
            printf(" -> [%s] %s (Sem: %d, %s)\n", notices[i].department, notices[i].title, notices[i].semester, notices[i].date);
            found = 1;
        }
    }
    if (!found) printf("No matching notices found.\n");
}

// Filter by department and semester
void filterByDeptSem() {
    if (totalNotices == 0) 
    {
        printf("No notices to filter.\n");
        return;
    }

    char dept[MAX_DEPT]; // to store department name
    int sem;
    printf("Enter department to filter: ");
    fgets(dept, sizeof(dept), stdin);
    dept[strcspn(dept, "\n")] = 0;
    printf("Enter semester (0 for all): ");
    scanf("%d", &sem);
    clearInputBuffer();

    int found = 0;
    printf("\n Notices for %s, Semester %d:\n", dept, sem);
    for (int i = 0; i < totalNotices; i++)
     {
        if (strcmp(notices[i].department, dept) == 0 &&
            (sem == 0 || notices[i].semester == sem))
             {
            printf(" -> %s (%s, %s)\n", notices[i].title, notices[i].date, notices[i].poster);
            found = 1;
        }
    }
    if (!found) printf("No notices found for this filter.\n");
}

// Filter by date range
void filterByDateRange() {
    if (totalNotices == 0) {
        printf("No notices to filter.\n");
        return;
    }

    char start[DATE_LEN], end[DATE_LEN]; // to store the starting and ending dates
    printf("Enter start date (YYYY-MM-DD): ");
    fgets(start, sizeof(start), stdin);
    start[strcspn(start, "\n")] = 0;
    while (!isValidDate(start)) {
        printf("Invalid start date. Enter again: ");
        fgets(start, sizeof(start), stdin);
        start[strcspn(start, "\n")] = 0;
    }

    printf("Enter end date (YYYY-MM-DD): ");
    fgets(end, sizeof(end), stdin);
    end[strcspn(end, "\n")] = 0;
    while (!isValidDate(end)) {
        printf("Invalid end date. Enter again: ");
        fgets(end, sizeof(end), stdin);
        end[strcspn(end, "\n")] = 0;
    }

    if (compareDates(start, end) > 0) {
        printf("Start date cannot be after end date.\n");
        return;
    }

    int found = 0;
    printf("\n Notices from %s to %s:\n", start, end);
    for (int i = 0; i < totalNotices; i++) {
        if (compareDates(notices[i].date, start) >= 0 &&
            compareDates(notices[i].date, end) <= 0) {
            printf(" -> [%s] %s (Sem: %d)\n", notices[i].department, notices[i].title, notices[i].semester);
            found = 1;
        }
    }
    if (!found) printf("No notices in this date range.\n");
}

// Student subscription to departments
void subscribeStudent() {
    char dept[MAX_DEPT]; // to store the names of the departments
    int id;

    printf("Enter student ID (0-%d): ", MAX_STUDENTS - 1);
    scanf("%d", &id);
    clearInputBuffer();

    if (id < 0 || id >= MAX_STUDENTS)// for validating the student ID
     {
        printf("Invalid student ID.\n");
        return;
    }

    printf("Enter department to subscribe: ");
    fgets(dept, sizeof(dept), stdin);
    dept[strcspn(dept, "\n")] = 0; // remove newline

    strcpy(subscriptions[id] + subCount[id] * MAX_DEPT, dept); // to the add department for subscriptions
    subCount[id]++; // to increase the subscription count
    printf(" Student %d subscribed to %s.\n", id, dept);
}

// Show notifications for a student (relevant notices)
void showNotifications() {
    int id;
    printf("Enter student ID to check notifications: ");
    scanf("%d", &id);
    clearInputBuffer();

    if (id < 0 || id >= MAX_STUDENTS || subCount[id] == 0) {
        printf("No subscriptions found for student %d.\n", id);
        return;
    }

    int found = 0;
    printf("\n Notifications for Student %d:\n", id);
    for (int i = 0; i < totalNotices; i++)
     {
        for (int j = 0; j < subCount[id]; j++)
         {
            char *dept = subscriptions[id] + j * MAX_DEPT;
            if (strcmp(notices[i].department, dept) == 0) 
            {
                printf(" -> [%s] %s (Sem: %d, %s)\n", notices[i].department, notices[i].title, notices[i].semester, notices[i].date); // to display relevant notices
                found = 1;
                break;
            }
        }
    }
    if (!found) printf("No new notices for your subscribed departments.\n");
}

// For Cleaning up the allocated memory
void cleanup() {
    for (int i = 0; i < totalNotices; i++) {
        free(notices[i].title); //for free title memory
        free(notices[i].body);//for the free body memory
    }
    free(notices);
    notices = NULL;
    totalNotices = maxNotices = 0;
}

// Main Menu for the Noticeboard System
int main() {
    int choice;

    printf(" Campus Noticeboard & Notification System\n");
    printf("===========================================\n");

    do {
        printf("\n Menu:\n");
        printf("1. Post Notice\n");
        printf("2. Edit Notice\n");
        printf("3. Delete Notice\n");
        printf("4. List All Notices\n");
        printf("5. Search Notices\n");
        printf("6. Filter by Department/Semester\n");
        printf("7. Filter by Date Range\n");
        printf("8. Subscribe Student\n");
        printf("9. Show Notifications\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: postNotice(); // to post a new notice
            break;
            case 2: editNotice(); // to edit an existing notice
            break;
            case 3: deleteNotice();// to delete a notice 
            break;
            case 4: listNotices();// to list all notices 
            break;
            case 5: searchNotices(); // to search notices by keyword
            break;
            case 6: filterByDeptSem(); // to filter by department and semester
            break;
            case 7: filterByDateRange(); // to filter by date range
            break;
            case 8: subscribeStudent();// to subscribe a student to departments 
            break;
            case 9: showNotifications(); // to show notifications for a student
            break;
            case 0: printf(" Exiting system. Goodbye!\n"); 
            break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);

    cleanup();
    return 0;
}