#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <math.h>
#include <string.h>

// File Initialization
FILE *gInputFile;
void usage (char *);

// File I/O
const int kMaxLineLength = 257;
int getNumCases (FILE *);

const int kMaxNumFragments = 288;
char gFragmentsInput [kMaxNumFragments][kMaxLineLength];

int gNumFragments = 0;
int gNumFiles = 0;
int gNumChars = 0;
int gFileLength = 0;

int getFragments (char [kMaxNumFragments][kMaxLineLength]);
int getNumFiles (int);
int getNumChars (char [kMaxNumFragments][kMaxLineLength], int);
int getFileLength (int, int);

void printFragments (char [kMaxNumFragments][kMaxLineLength], int);


int main(int argc, char *argv[]) {
  
  /*-------------------------------FILE INITIALIZATION START-----------------------------*/
  if (argc != 2) usage (argv[0]);

  if (NULL == (gInputFile = fopen(argv[1], "r"))) {
    fprintf (stderr, "inputFile: %s: %s\n", argv[1], strerror(errno));
    exit (EXIT_FAILURE);
  }
  else {
    fprintf (stderr, "%s opened for reading.\n\n" , argv[1]);
  }
  /*-------------------------------FILE INITIALIZATION END--------------------------------*/
  /*--------------------------------MAIN PROGRAM START------------------------------------*/

  int numCases = getNumCases(gInputFile);
  printf ("numCases: %d\n\n", numCases);

  for (int i = 0; i < numCases; i++) {
    gNumFragments = getFragments (gFragmentsInput);
    gNumFiles = getNumFiles (gNumFragments);
    gNumChars = getNumChars (gFragmentsInput, gNumFragments);
    gFileLength = getFileLength (gNumChars, gNumFiles);

    printf ("Case %2d\n", i);
    printf ("--------\n");
    printf ("  gNumFragments: %d\n", gNumFragments);
    printf ("  gNumFiles: %d\n", gNumFiles);
    printf ("  gNumChars: %d\n", gNumChars);
    printf ("  gFileLength: %d\n\n", gFileLength);
  }

  






  
  /*--------------------------------MAIN PROGRAM END--------------------------------------*/
  /*--------------------------------FILE CLEANUP START------------------------------------*/
  fclose (gInputFile);
  /*--------------------------------FILE CLEANUP END--------------------------------------*/
  return 0;
}

// File Initialization
void usage (char *cmd) {
  fprintf (stderr, "usage: %s inputFileName\n", cmd);
  exit (EXIT_SUCCESS);
}

// File I/O
int getNumCases (FILE *fp) {
  // Get numCases
  char line [kMaxLineLength];
  fgets (line, kMaxLineLength, fp);
  line[strcspn(line, "\n")] = '\0';
  int numCases = atoi(line);

  // Remove blank line
  char newline [1];
  fgets (newline, kMaxLineLength, fp);

  return numCases;
}

int getFragments (char fragmentArray [kMaxNumFragments][kMaxLineLength]) {
  // Get fragments
  int i = 0;
  char line [kMaxLineLength];
  while (fgets (line, kMaxLineLength, gInputFile) != NULL) {
    line[strcspn(line, "\n")] = '\0';
    strcpy (fragmentArray[i], line);
    i++;
  }
  return i;
}
void printFragments (char fragmentArray [kMaxNumFragments][kMaxLineLength], int numFragments) {
  for (int i = 0; i < numFragments; i++) {
    printf ("%s\n", fragmentArray[i]);
  }
}

int getNumFiles (int numFragments) {
  return numFragments / 2;
}

int getNumChars (char fragmentInput [kMaxNumFragments][kMaxLineLength], int numFragment) {
  int numChars = 0;
  for (int i = 0; i < numFragment; i++) {
    numChars += strlen(fragmentInput[i]);
  }
  return numChars;
}
int getFileLength (int numChars, int numFiles) {
  return numChars / numFiles;
}