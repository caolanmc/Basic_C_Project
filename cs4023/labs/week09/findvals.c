#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <omp.h>
#include "utils.h"

int main(int argc, char **argv)
{
  char *refStr, *tolStr;
  int verbose = 0;

  refStr = tolStr = NULL;
  int ind;
  for (ind = 1; ind < argc; ind++)
  {
    if (strcmp(argv[ind], "-r") == 0) { refStr = argv[ind+1]; ind++; }
    if (strcmp(argv[ind], "-t") == 0) { tolStr = argv[ind+1]; ind++; }
  }

   for(int i = 0;i < argc && !verbose;i++)
     if(strcmp(argv[i], "-v") == 0)  verbose = 1;

  if (((verbose == 1) ? argc != 6 : argc !=5) || refStr == NULL || tolStr == NULL)
  {
    fprintf(stderr, "Usage: %s -r ref -t tol (-v)\n", argv[0]);
    exit(1);
  }

  struct tm *local;
  time_t start2;
  time(&start2);
  local = localtime(&start2);
  printf("# Start time and date: %s", asctime(local));

  struct timeval tim;
  gettimeofday(&tim, NULL);
  double start = tim.tv_sec+(tim.tv_usec/1000000.0);

  float ref = strtof(refStr, 0);
  float tol = strtof(tolStr, 0);
  if (tol < 0.0)
    tol *= -1;  

  int numRows, numCols;
  fscanf(stdin, "%d", &numRows);
  fscanf(stdin, "%d", &numCols);

  float** rows = (float** ) malloc(numRows * sizeof(float *));
  if (rows == 0)
  {
    fprintf(stderr, "Couldn't alocate sufficient space.\n");
    exit(1);
  }
  int i;
  for (i = 0; i < numRows; i++)
  {
    float* row = (float* ) malloc(numCols * sizeof(float));
    if (row == 0)
    {
      fprintf(stderr, "Couldn't alocate sufficient row space.\n");
      exit(1);
    }
    rows[i] = row;
  }

  int r, c;
  for (r = 0; r < numRows; r++)
    for (c = 0; c < numCols; c++)
      fscanf(stdin, "%f", &rows[r][c]);

  int count = 0;
#pragma omp parallel for private(r) private(c) shared(count)
  for (r = 0; r < numRows; r++)
    for (c = 0; c < numCols; c++)
      if (approxEqual(rows[r][c], ref, tol) == 1)
      {
      if(verbose)
        fprintf(stdout, "r=%d, cc=%d, %.6f(thread=%d)\n",r,c,rows[r][c],omp_get_thread_num());
      #pragma omp atomic update
      count++;
      }

  fprintf(stdout, "Found %d approximate matches.\n", count);
  gettimeofday(&tim, NULL);
  double end = tim.tv_sec+(tim.tv_usec/1000000.0);
  printf("Elapsed time: %.6lf(s)\n", end - start);
  
  exit(0);
}
