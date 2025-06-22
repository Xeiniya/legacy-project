#ifndef INTERFACE_H
#define INTERFACE_H

#define MAX_NAME_LENGTH 50
#define MAX_ZOD_LENGTH 20
#define ARRAY_SIZE 10

// Structure definition for ZNAK
typedef struct {
    char Name[MAX_NAME_LENGTH];
    char ZOD[MAX_ZOD_LENGTH];
    int DATE[3]; // [0] - year, [1] - month, [2] - day
} ZNAK;

#endif 
