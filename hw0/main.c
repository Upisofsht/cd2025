#include <stdio.h>
#include <string.h>

int main(void) {
    char *str = malloc(100 * sizeof(char));
    if (str == NULL) {                     // 檢查是否配置成功
        perror("Failed to allocate memory");
        return 1;
    }
    // FILE *fptr;
    // fptr = fopen("main.c", "r");
    // fscanf(fptr, "%s", *str);

    // fclose(fptr);
    // printf("%s", *str);
    return 0;
}
