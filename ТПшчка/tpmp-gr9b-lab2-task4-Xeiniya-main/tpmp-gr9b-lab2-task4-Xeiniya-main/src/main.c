#include "filemanager.h"
#include "client.h"
#include <stdio.h>

#define MAX_OWNERS 100

int main() {
    CarOwner owners[MAX_OWNERS];
    int size;
    
    if (create_sample_data("data.txt")) {
        printf("Ошибка создания файла data.txt\n");
        return 1;
    }
    
    printf("Создан файл data.txt с тестовыми данными.\n");
    printf("Содержимое data.txt:\n");
    display_file_content("data.txt");
    
    if (read_data("data.txt", owners, &size)) {
        printf("Ошибка открытия файла data.txt\n");
        return 1;
    }
    
    printf("\nСписок всех владельцев автомобилей:\n");
    display_data(owners, size);
    
    filter_and_save("output.txt", owners, size);
    printf("\nДанные о владельцах автомобилей марки 'Ваз' сохранены в output.txt\n");
    printf("Содержимое output.txt:\n");
    display_file_content("output.txt");
    
    return 0;
}
