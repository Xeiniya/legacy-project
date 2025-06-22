#include "filemanager.h"
#include <stdio.h>
#include <string.h>

int create_sample_data(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) return 1;
    
    fprintf(file, "Иванов Иван Иванович 123456789 Минск,Ленина,10 Ваз 1234AB-5 56789\n");
    fprintf(file, "Петров Петр Петрович 987654321 Гомель,Советская,15 BMW 5678CD-7 12345\n");
    fprintf(file, "Сидоров Алексей Викторович 555666777 Брест,Мира,5 Ваз 4321EF-3 67890\n");
    
    fclose(file);
    return 0;
}

int read_data(const char *filename, CarOwner owners[], int *size) {
    FILE *file = fopen(filename, "r");
    if (!file) return 1;
    
    *size = 0;
    while (fscanf(file, "%s %s %s %s %s %s %s %s", owners[*size].surname, owners[*size].name,
                  owners[*size].patronymic, owners[*size].phone, owners[*size].address,
                  owners[*size].car_brand, owners[*size].car_number, owners[*size].tech_passport) == 8) {
        (*size)++;
    }
    fclose(file);
    return 0;
}

void filter_and_save(const char *output_filename, CarOwner owners[], int size) {
    FILE *file = fopen(output_filename, "w");
    if (!file) return;
    
    for (int i = 0; i < size; i++) {
        if (strcmp(owners[i].car_brand, "Ваз") == 0) {
            fprintf(file, "%s %s %s %s %s %s %s %s\n",
                    owners[i].surname, owners[i].name, owners[i].patronymic, owners[i].phone,
                    owners[i].address, owners[i].car_brand, owners[i].car_number, owners[i].tech_passport);
        }
    }
    fclose(file);
}

void display_file_content(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка открытия файла %s\n", filename);
        return;
    }
    
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
    fclose(file);
}
