#include <stdio.h>
#include <limits.h>

int main()
{
    long a = INT_MAX;
    
    if ((int)a != a)
        printf("%s", "different\n");
    else
        printf("%s", "same\n");
}