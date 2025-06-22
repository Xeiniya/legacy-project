#ifndef SERVER_H
#define SERVER_H

#include "interface.h"

int create_sample_data(const char *filename);
int read_data(const char *filename, CarOwner owners[], int *size);
void filter_and_save(const char *output_filename, CarOwner owners[], int size);
void display_file_content(const char *filename);

#endif
