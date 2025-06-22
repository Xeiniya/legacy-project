#include "client.h"
#include <stdio.h>

void display_data(CarOwner owners[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%s %s %s %s %s %s %s %s\n",
               owners[i].surname, owners[i].name, owners[i].patronymic, owners[i].phone,
               owners[i].address, owners[i].car_brand, owners[i].car_number, owners[i].tech_passport);
    }
}
