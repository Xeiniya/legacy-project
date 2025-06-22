#include <stdio.h>
#include <string.h>
#include "znakmanager.h"

int main() {
    ZNAK mass[ARRAY_SIZE];
    ZNAK result[ARRAY_SIZE];
    char searchZodiac[MAX_ZOD_LENGTH];
    int foundCount;
    int i;
    
    // Input data for the array
    inputData(mass, ARRAY_SIZE);
    
    // Display the sorted array
    printf("\nSorted list of people by date of birth:\n");
    for (i = 0; i < ARRAY_SIZE; i++) {
        printf("\nPerson %d:\n", i + 1);
        displayPerson(&mass[i]);
    }
    
    // Search by zodiac sign
    printf("\nEnter zodiac sign to search: ");
    scanf(" %[^\n]", searchZodiac);
    
    foundCount = findByZodiac(mass, ARRAY_SIZE, searchZodiac, result, ARRAY_SIZE);
    
    if (foundCount > 0) {
        printf("\nFound %d people with zodiac sign '%s':\n", foundCount, searchZodiac);
        for (i = 0; i < foundCount; i++) {
            printf("\nPerson %d:\n", i + 1);
            displayPerson(&result[i]);
        }
    } else {
        printf("\nNo people with zodiac sign '%s' were found.\n", searchZodiac);
    }
    
    return 0;
}
