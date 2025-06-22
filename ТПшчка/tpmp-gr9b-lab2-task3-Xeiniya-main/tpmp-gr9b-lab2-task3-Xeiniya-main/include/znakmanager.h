#ifndef SERVER_H
#define SERVER_H

#include "interface.h"

// Function to input data into the MASS array
void inputData(ZNAK *mass, int size);

// Function to sort the array by birth date
void sortByBirthDate(ZNAK *mass, int size);

// Function to find people by zodiac sign
int findByZodiac(ZNAK *mass, int size, const char *zodiac, ZNAK *result, int resultSize);

// Function to compare dates (for sorting)
int compareDates(const int *date1, const int *date2);

// Function to display person data
void displayPerson(const ZNAK *person);

#endif 
