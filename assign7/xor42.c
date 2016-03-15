#include <stdlib.h>
#include <stdio.h>

int main()
{
    for (int i = 0; i < 256; i++) {
        if (i != 0 && i % 30 == 0) {
            printf("\n");
        }
        printf("\\%03o", i);
    }
    
    printf("\n\n\n");
    
    for (int i = 0; i < 256; i++) {
        if (i != 0 && i % 30 == 0) {
            printf("\n");
        }
        printf("\\%03o", i ^ 42);
    }
    
        printf("\n\n\n");
}