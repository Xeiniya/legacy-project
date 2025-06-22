#ifndef INTERFACE_H
#define INTERFACE_H

#define MAX_LEN 100

typedef struct {
    char surname[MAX_LEN];
    char name[MAX_LEN];
    char patronymic[MAX_LEN];
    char phone[MAX_LEN];
    char address[MAX_LEN];
    char car_brand[MAX_LEN];
    char car_number[MAX_LEN];
    char tech_passport[MAX_LEN];
} CarOwner;

#endif
