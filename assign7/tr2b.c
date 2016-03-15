#include <stdlib.h>
#include <stdio.h>

int check_operands(char *from, char *to);

int main(int argc, char *argv[])
{
  if (argc != 3) {
    fprintf(stderr, "Invalid number of arguments\n");
    exit(1);
  }
  
  char *from = argv[1];
  char *to = argv[2];
  
  if (!check_operands(from, to)) {
    fprintf(stderr, "from must have no duplicate bytes and be the same length as to\n");
    exit(1);
  }
  
  int ch;
  
  while ((ch = getchar()) != EOF) {
    
    for (int i = 0; from[i] != 0; i++) {  
      if (ch == from[i]) {
	ch = to[i];
	break;
      }
    }
    
    if (putchar(ch) == EOF) {
      fprintf(stderr, "error writing character\n");
      exit(1);
    }
  }
  
  if (ferror(stdin)) {
    fprintf(stderr, "error reading character\n");
    exit(1);
  }
  
  exit(0);
}

int check_operands(char *from, char *to)
{
  //check for duplicates in from
  for (int i = 0; from[i] != 0; i++) {
    for (int j = i + 1; from[j] != 0; j++) {
      if (from[i] == from[j])
	return 0;
    }
  }
  
  //check if both from and to are the same length
  int i;
  for (i = 0; from[i] != 0 && to[i] != 0; i++);
  if (from[i] != 0 || to[i] != 0)
    return 0;
  
  return 1;
}