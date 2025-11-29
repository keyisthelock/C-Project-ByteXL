#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int isValidDate(char *date) {
    int y,m,d;
    return sscanf(date, "%d-%d-%d", &y,&m,&d) == 3;
}

int compareDates(char *d1, char *d2) {
    return strcmp(d1, d2);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}