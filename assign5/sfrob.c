#include <stdlib.h>
#include <stdio.h>

int frobcmp(char const *a, char const *b);
int frobcmp_qsort(const void *a, const void *b);

int main(int argc, char *argv[])
{   
    int ch = getchar();
    
    int lines_size = 16;
    int lines_pos = -1;
    int str_size = 16;
    int str_pos = 0;
    char **lines = malloc(lines_size * sizeof(char*));
 
    int end_of_file = 0;
    
    while (ch != EOF) {
        
        if (str_pos == 0) {
            lines_pos++;
            //reallocate with larger size if lines is out of space
            if (lines_pos == lines_size) {
                lines_size *= 2;
                char **realloc_lines = realloc(lines, lines_size * sizeof(char*));
                
                //out of memory if realloc failed, exit with error
                if (realloc_lines == NULL) {
                    fprintf(stderr, "Out of memory to reallocate lines\n");
                    exit(1);
                }
                
                lines = realloc_lines;
            }
            lines[lines_pos] = malloc(str_size * sizeof(char));
        }
        
        //expand string if out of space
        if (str_pos == str_size) {
            str_size *= 2;
            char *realloc_str = realloc(lines[lines_pos], str_size * sizeof(char));
            
            //check if out of memory
            if (realloc_str == NULL) {
                fprintf(stderr, "Out of memory to reallocate string\n");
                exit(1);
            }
            lines[lines_pos] = realloc_str;
        }
        
        lines[lines_pos][str_pos] = ch;
        str_pos++;
        
        //new line when space found
        if (ch == ' ') {
            str_pos = 0;
            str_size = 16;
        }
        
        ch = getchar();
    }
    
    //add space if input is missing one at the end
    if (str_pos != 0) {
        //expand string if out of space
        if (str_pos == str_size) {
            char *realloc_str = realloc(lines[lines_pos], (str_pos+1) * sizeof(char));
            //check if out of memory
            if (realloc_str == NULL) {
                fprintf(stderr, "Out of memory to reallocate string\n");
                exit(1);
            }
        }
        lines[lines_pos][str_pos] = ' ';
    }
    
    //check if EOF was caused by a read error instead of actual EOF
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input from stdin\n");
        exit(1);
    }
    
    qsort(lines, lines_pos + 1, sizeof(char*), frobcmp_qsort);
    
    for (int i = 0; i <= lines_pos; i++) {
        for (int j = 0; lines[i][j] != ' '; j++) {
            
            if (putchar(lines[i][j]) == EOF) { //check for write error
                fprintf(stderr, "Error writing output to stdout");
                exit(1);
            }
        }
        if (putchar(' ') == EOF) { //check for write error
            fprintf(stderr, "Error writing output to stdout");
            exit(1);
        }
    }
    
    //free memory used by array
    for (int i = 0; i <= lines_pos; i++) {
        free(lines[i]);
    }
    free(lines);
    
    exit(0);
}

int frobcmp(char const *a, char const *b)
{    
    int i;
    for (i = 0; a[i] != ' ' && b[i] != ' '; i++) {
        char defrob_a = a[i] ^ 42;
        char defrob_b = b[i] ^ 42;
        
        int diff = defrob_a - defrob_b;
        if (diff != 0)
            return diff;
    }
    
    //b is a prefix of a
    if (a[i] != ' ')
        return 1;
    //a is a prefix of b
    if (b[i] != ' ')
        return -1;
    
    return 0; //both strings identical
}

//helper function so that frobcmp is properly used by qsort
int frobcmp_qsort(const void *a, const void *b) {
    return frobcmp(*(char const **)a, *(char const **)b);
}