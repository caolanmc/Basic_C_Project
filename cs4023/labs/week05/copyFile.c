#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int main(int argCopy, char* argPaste[])
{
  char c;
  FILE *from;
  FILE *from2;
  int flip=(strcmp(argPaste[1], "-f")==0);

	    if(flip==0)
	      {
		from=fopen(argPaste[2], "r");
		from2=fopen(argPaste[2], "w+");
	      }
	    else
	      {
		from=fopen(argPaste[2],"r");
		from2=fopen(argPaste[3],"w+");
	      }
	    if(from==NULL)
	      {
		perror("file.txt");
		exit(1);
	      }

	    while((c=getc(from))!=EOF)
	      {
		if(flip)
		  putc(flipChar(c),from2);
		else
		  putc(c,from2);
	      }
	    fclose(from2);
	    fclose(from);

	    exit(0);
}
