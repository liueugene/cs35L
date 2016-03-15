#include <stdlib.h>
#include <stdio.h>

int main()
{
    FILE *test1 = fopen("test1.txt", "w+");
    FILE *test2 = fopen("test2.txt", "w+");
    FILE *test3 = fopen("test3.txt", "w+");
    FILE *test4 = fopen("test4.txt", "w+");
    FILE *test5 = fopen("test5.txt", "w+");
    
    for (int i = 0; i < 100; i++) {
        fprintf(test1, "%x ", i);
    }
    
    for (int i = 0; i < 1000; i++) {
        fprintf(test2, "%x ", i);
    }
    
    for (int i = 0; i < 10000; i++) {
        fprintf(test3, "%x ", i);
    }
    
    for (int i = 0; i < 1000000; i++) {
        fprintf(test4, "%x ", i);
    }
    
    for (int i = 0; i < 10000000; i++) {
        fprintf(test5, "%x ", i);
    }
    
    fclose(test1);
    fclose(test2);
    fclose(test3);
    fclose(test4);
    fclose(test5);
    
    printf("Created test files");
    exit(0);
}
