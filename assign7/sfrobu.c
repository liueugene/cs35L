#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int comparisons = 0;

int frobcmp(char const *a, char const *b);
int frobcmp_qsort(const void *a, const void *b);

int main()
{
    struct stat buffer;
    char *bytes;
    char **lines;
    int alloc_size;
    int file_size = 0;
    int pos = 0;
    int bytes_read = 0;
    
    if (fstat(0, &buffer) == -1) {
        fprintf(stderr, "Error reading file info\n");
        exit(1);
    }
    
    if (buffer.st_size == 0)
        alloc_size = 16;
    else
        alloc_size = buffer.st_size;
    
    bytes = malloc(alloc_size);
    
    if (bytes == NULL) {
        fprintf(stderr, "Out of memory to allocate\n");
        exit(1);
    }
    
    while ((bytes_read = read(0, bytes + pos, alloc_size - pos)) > 0) {
        file_size += bytes_read;
        pos = alloc_size;
        
        fstat(0, &buffer);
        //reallocate if file has grown while reading
        if (buffer.st_size != alloc_size) {
            if (buffer.st_size == 0)
                alloc_size *= 2;
            else
                alloc_size = buffer.st_size;
            
            char *realloc_bytes = realloc(bytes, alloc_size);
            
            if (realloc_bytes == NULL) {
                fprintf(stderr, "Out of memory to reallocate\n");
                exit(1);
            }
            
            bytes = realloc_bytes;
        }
    }    

    if (bytes_read == -1) {
        fprintf(stderr, "Error reading file\n");
        exit(1);
    }
    
    //add space at end of file if needed
    if (file_size > 0 && bytes[file_size - 1] != ' ') {
        bytes = realloc(bytes, file_size + 1);
        bytes[file_size] = ' ';
        file_size++;
    }
    
    int lines_pos = -1;
    int lines_size = 16;
    lines = malloc(lines_size * sizeof(char*));
    
    if (lines == NULL) {
        fprintf(stderr, "Out of memory to allocate\n");
        exit(1);
    }
    
    int prev_is_space = 1;

    //create the lines array (pointers to first character of each line)
    for (int i = 0; i < file_size - 1; i++) {
        if (bytes[i] == ' ') {
            prev_is_space = 1;
            continue;
        }
        
        if (prev_is_space) {
            lines_pos++;
            //allocate more space if needed
            if (lines_pos == lines_size) {
                lines_size *= 2;
                char **realloc_lines = realloc(lines, lines_size * sizeof(char*));
                
                if (realloc_lines == NULL) {
                    fprintf(stderr, "Out of memory to reallocate\n");
                    exit(1);
                }
                
                lines = realloc_lines;
            }
            
            lines[lines_pos] = bytes + i;
            prev_is_space = 0;
        }
    }
    
    qsort(lines, lines_pos + 1, sizeof(char*), frobcmp_qsort);
    
    for (int i = 0; i <= lines_pos; i++) {
        int length = 0;
        for (; lines[i][length] != ' '; length++);
        length++;
        
        write(1, lines[i], length);
    }
    
    free(bytes);
    free(lines);
    
    fprintf(stderr, "Comparisons: %d\n", comparisons);
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
    comparisons++;
    return frobcmp(*(char const **)a, *(char const **)b);
}