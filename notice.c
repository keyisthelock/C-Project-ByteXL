#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "notice.h"
#include "utils.h"

Notice *notices = NULL;
int totalNotices = 0;
int maxNotices = 0;

void postNotice() {
    // first check if we have space, if not make the array bigger
    ensureCapacity();
    Notice *n = &notices[totalNotices];

    char tempTitle[100], tempBody[500], dept[MAX_DEPT], poster[50], date[DATE_LEN];
    int sem;

    printf("\n--- Post a New Notice ---\n");

    // getting the title from the user
    printf("Enter title: ");
    fgets(tempTitle, sizeof(tempTitle), stdin);
    // this line just removes the enter key that fgets captures
    tempTitle[strcspn(tempTitle, "\n")] = 0;

    printf("Enter body: ");
    fgets(tempBody, sizeof(tempBody), stdin);
    tempBody[strcspn(tempBody, "\n")] = 0;

    printf("Enter department: ");
    fgets(dept, sizeof(dept), stdin);
    dept[strcspn(dept, "\n")] = 0;

    printf("Enter semester (1-8): ");
    scanf("%d", &sem);
    clearInputBuffer(); // need this to fix scanf buffer issues

    printf("Enter poster name: ");
    fgets(poster, sizeof(poster), stdin);
    poster[strcspn(poster, "\n")] = 0;

    printf("Enter date (YYYY-MM-DD): ");
    fgets(date, sizeof(date), stdin);
    date[strcspn(date, "\n")] = 0;

    // now copying all the temp variables into the main notice list
    strcpy(n->title, tempTitle);
    strcpy(n->body, tempBody);
    strcpy(n->department, dept);
    n->semester = sem;
    strcpy(n->poster, poster);
    strcpy(n->date, date);

    // increase the counter so we know how many notices we have
    totalNotices++;
    printf("Notice posted successfully!\n");
}

void editNotice() {
    if (totalNotices == 0) {
        printf("No notices to edit.\n");
        return;
    }

    listNotices();
    int index;
    printf("Enter notice number to edit: ");
    scanf("%d", &index);
    clearInputBuffer();

    if (index < 1 || index > totalNotices) return;

    Notice *n = &notices[index - 1];
    char input[1000];

    printf("New title (Enter to skip): ");
    fgets(input, sizeof(input), stdin);

    // Logic Fix: Only update if user didn't just hit Enter
    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = 0;
        strcpy(n->title, input); // Simple copy, no malloc needed
        printf("Title updated.\n");
    }

    printf("New body (Enter to skip): ");
    fgets(input, sizeof(input), stdin);

    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = 0;
        strcpy(n->body, input);
        printf("Body updated.\n");
    }
}

void deleteNotice() {
    if (totalNotices == 0) {
        printf("No notices.\n");
        return;
    }

    listNotices();
    int index;
    printf("Enter notice number to delete: ");
    scanf("%d", &index);
    clearInputBuffer();

    if (index < 1 || index > totalNotices) return;

    // Just shift everything left to cover the gap
    for (int i = index - 1; i < totalNotices - 1; i++)
        notices[i] = notices[i + 1];

    totalNotices--;
    printf("Notice deleted.\n");
}

void listNotices() {
    if (totalNotices == 0) {
        printf("No notices available.\n");
        return;
    }

    for (int i = 0; i < totalNotices; i++) {
        printf("%d. %s [%s] (%s)\n", i + 1, notices[i].title, notices[i].department, notices[i].date);
    }
}

void searchNotices() {
    char query[100];
    printf("Enter word to search: ");
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = 0;

    int found = 0;
    for(int i=0; i<totalNotices; i++) {
        // strstr checks if query is inside the title
        if(strstr(notices[i].title, query) != NULL) {
            printf("Found: %s\n", notices[i].title);
            found = 1;
        }
    }
    if(!found) printf("No match found.\n");
}

void filterByDeptSem() {
    char dept[50];
    printf("Enter Dept: ");
    fgets(dept, sizeof(dept), stdin);
    dept[strcspn(dept, "\n")] = 0;

    int found = 0;
    for(int i=0; i<totalNotices; i++) {
        if(strcmp(notices[i].department, dept) == 0) {
            printf("- %s\n", notices[i].title);
            found = 1;
        }
    }
    if(!found) printf("No notices for %s.\n", dept);
}

void filterByDateRange() {
    char date[20];
    printf("Enter Date: ");
    fgets(date, sizeof(date), stdin);
    date[strcspn(date, "\n")] = 0;

    int found = 0;
    for(int i=0; i<totalNotices; i++) {
        if(strcmp(notices[i].date, date) == 0) {
            printf("- %s\n", notices[i].title);
            found = 1;
        }
    }
    if(!found) printf("Nothing on this date.\n");
}

void ensureCapacity() {
    if (totalNotices >= maxNotices) {
        maxNotices = (maxNotices == 0 ? 2 : maxNotices * 2);
        notices = realloc(notices, sizeof(Notice) * maxNotices);
        if (!notices) {
            printf("Memory error!\n");
            exit(1);
        }
    }
}