#include <stdio.h>
#include <string.h>
#include "znakmanager.h"

void inputData(ZNAK *mass, int size) {
    int i;
    
    printf("Enter information for %d people:\n", size);
    
    for (i = 0; i < size; i++) {
        printf("\nPerson %d:\n", i + 1);
        
        printf("Enter name and surname: ");
        scanf(" %[^\n]", mass[i].Name);
        
        printf("Enter zodiac sign: ");
        scanf(" %[^\n]", mass[i].ZOD);
        
        printf("Enter date of birth (year month day): ");
        scanf("%d %d %d", &mass[i].DATE[0], &mass[i].DATE[1], &mass[i].DATE[2]);
    }
    
    // Sort the array after input
    sortByBirthDate(mass, size);
}

int compareDates(const int *date1, const int *date2) {
    // Compare years
    if (date1[0] != date2[0]) {
        return date1[0] - date2[0];
    }
    
    // Years are equal, compare months
    if (date1[1] != date2[1]) {
        return date1[1] - date2[1];
    }
    
    // Years and months are equal, compare days
    return date1[2] - date2[2];
}

void sortByBirthDate(ZNAK *mass, int size) {
    int i, j;
    ZNAK temp;
    
    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size - i - 1; j++) {
            if (compareDates(mass[j].DATE, mass[j + 1].DATE) > 0) {
                // Swap elements
                temp = mass[j];
                mass[j] = mass[j + 1];
                mass[j + 1] = temp;
            }
        }
    }
}

int findByZodiac(ZNAK *mass, int size, const char *zodiac, ZNAK *result, int resultSize) {
    int i;
    int count = 0;
    
    for (i = 0; i < size && count < resultSize; i++) {
        if (strcmp(mass[i].ZOD, zodiac) == 0) {
            result[count++] = mass[i];
        }
    }
    
    return count;
}

void displayPerson(const ZNAK *person) {
    printf("Name: %s\n", person->Name);
    printf("Zodiac sign: %s\n", person->ZOD);
    printf("Date of birth: %d-%02d-%02d\n", 
           person->DATE[0], person->DATE[1], person->DATE[2]);
}
