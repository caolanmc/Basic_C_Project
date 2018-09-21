#include <stdio.h>
#include <stdlib.h>

int main()
{
  char c;
  FILE *from;
  FILE *from2;

  from = fopen("file.txt", "r");
  if (from == NULL)
  {
    perror("file.txt");
    exit(1);
  }
  
  from2 = fopen("copy.txt", "w");
  if (from2 == NULL)
  {
    perror("copy.txt");
    exit(1);
  }
  

  /* file exists, so start reading */
  while ((c = getc(from)) != EOF)
    putc(c, from2);

  fclose(from2);

  exit(0);
}
