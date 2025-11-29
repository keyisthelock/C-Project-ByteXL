#ifndef NOTICE_H
#define NOTICE_H

#include <stdio.h>

#define MAX_DEPT 50
#define DATE_LEN 20

typedef struct {
    // using fixed size arrays instead of pointers so we don't have to use malloc
    char title[100];
    char body[500];
    char department[MAX_DEPT];
    int semester;
    char poster[50];
    char date[DATE_LEN];
} Notice;

extern Notice *notices;
extern int totalNotices;
extern int maxNotices;

void postNotice();
void editNotice();
void deleteNotice();
void listNotices();
void searchNotices();
void filterByDeptSem();
void filterByDateRange();
void ensureCapacity();

#endif