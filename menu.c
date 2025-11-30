#include <stdio.h>
#include "menu.h"
#include "notice.h"
#include "subscription.h"

void showMenu() {
    int userSelection;

    do {
        printf("\n");
        printf("=========================================\n");
        printf("      CAMPUS DIGITAL NOTICEBOARD         \n");
        printf("=========================================\n");
        printf(" [ MANAGE NOTICES ]\n");
        printf("  1. Post New Notice\n");
        printf("  2. Edit Existing Notice\n");
        printf("  3. Delete Notice\n");
        printf("\n");
        printf(" [ BROWSE & SEARCH ]\n");
        printf("  4. View All Notices (Table View)\n");
        printf("  5. Search by Title\n");
        printf("  6. Filter by Department\n");
        printf("  7. Filter by Date\n");
        printf("\n");
        printf(" [ STUDENT ZONE ]\n");
        printf("  8. Subscribe to Updates\n");
        printf("  9. View My Feed\n");
        printf("\n");
        printf("  0. Exit System\n");
        printf("=========================================\n");
        printf("[?] Enter your choice: ");
        
        scanf("%d", &userSelection);
        getchar(); // clear buffer

        switch(userSelection) {
            case 1: postNotice(); break;
            case 2: editNotice(); break;
            case 3: deleteNotice(); break;
            case 4: listNotices(); break;
            case 5: searchNotices(); break;
            case 6: filterByDeptSem(); break;
            case 7: filterByDateRange(); break;
            case 8: subscribeStudent(); break;
            case 9: showNotifications(); break;
            case 0: printf("\n[!] Shutting down... Goodbye!\n"); break;
            default: printf("\n[!] Invalid option. Please try again.\n");
        }

    } while(userSelection != 0);
}