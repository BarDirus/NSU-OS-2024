#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Для getuid(), geteuid(), setuid()
#include <sys/types.h>
#include <errno.h>

void print_uids() {
    printf("Real UID: %d, Effective UID: %d\n", getuid(), geteuid());
}

int main() {
    FILE *file;
    const char *filename = "datafile.txt";  // Имя файла, созданного с правами 600

    // Шаг 1: Печать реального и эффективного идентификаторов пользователя
    printf("Before setuid:\n");
    print_uids();

    // Шаг 2: Открытие файла
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
    } else {
        printf("File opened successfully\n");
        fclose(file);
    }

    // Шаг 3: Сделать так, чтобы реальный и эффективный идентификаторы совпадали
    if (setuid(getuid()) == -1) {
        perror("Error setting UID");
        exit(EXIT_FAILURE);
    }

    // Шаг 4: Печать реального и эффективного идентификаторов после setuid
    printf("After setuid:\n");
    print_uids();

    // Шаг 5: Повторное открытие файла
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file after setuid");
    } else {
        printf("File opened successfully after setuid\n");
        fclose(file);
    }

    return 0;
}