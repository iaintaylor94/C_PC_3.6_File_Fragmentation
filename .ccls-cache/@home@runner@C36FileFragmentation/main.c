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
const int kMaxFileLength = 256;
const int kMaxLineLength = kMaxFileLength + 1;
int getNumCases (FILE *);

const int kMaxNumFiles = 144;
const int kMaxNumFragments = kMaxNumFiles * 2;
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


// Dictionary
struct chapter {
  char files [kMaxNumFiles][kMaxLineLength];
  int numFiles;
};
struct dictionary {
  struct chapter chapter [kMaxLineLength]; // [0] == Total combination of all files
  int numChapters;
} gDictionary;



void combineFragments (char [kMaxLineLength], char [kMaxLineLength], char [kMaxLineLength]);
bool inDictionary (char [kMaxLineLength], int);
void addToDictionary (int, char [kMaxLineLength], struct dictionary *);
void printChapter (struct dictionary*, int);
void printDictionary (struct dictionary*);

void intersection (struct chapter *, struct chapter *);
void fileIntersection (struct dictionary *);
void possibleFile (struct dictionary *);


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

    printf ("Case %2d:\n", i + 1);
    printf ("  gNumFragments: %d\n", gNumFragments);
    printf ("  gNumFiles: %d\n", gNumFiles);
    printf ("  gNumChars: %d\n", gNumChars);
    printf ("  gFileLength: %d\n", gFileLength);
    printf ("\n");


    
    for (int first = 0; first < gNumFragments; first++) {
      for (int second = 0; second < gNumFragments; second++) {
        if (strlen(gFragmentsInput[first]) + strlen(gFragmentsInput[second]) != gFileLength) {
          continue;
        }
        else if (first == second) {
          continue;
        }
        else {
          char combination [kMaxLineLength];
          int minLength = strlen(gFragmentsInput[first]) < strlen(gFragmentsInput[second]) ?
                          strlen(gFragmentsInput[first]) : strlen(gFragmentsInput[second]);
          // permutation 1
          combineFragments (combination, gFragmentsInput[first], gFragmentsInput[second]);
          addToDictionary (minLength, combination, &gDictionary);

          // Permutation 2
          combineFragments (combination, gFragmentsInput[second], gFragmentsInput[first]);
          addToDictionary (minLength, combination, &gDictionary);
        }
      }
    }

    printDictionary (&gDictionary);


    fileIntersection (&gDictionary);
    possibleFile (&gDictionary);
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


// Dictionary
void combineFragments (char combination [kMaxLineLength], char first [kMaxLineLength], char second [kMaxLineLength]) {
  strcpy (combination, first);
  strcat (combination, second);
  combination [strcspn(combination, "\n")] = '\0';
}

bool inDictionary (char file [kMaxLineLength], int c) {
  // Search Dictionary
  for (int i = 0; i < gDictionary.chapter[c].numFiles; i++) {
    if (!strcmp (file, gDictionary.chapter[c].files[i])) {
      return true;
    }
  }
  return false;
}
void addToDictionary (int minLength, char file [kMaxLineLength], struct dictionary *d) {
  // Add to Dictionary [0]
  if (!inDictionary (file, 0)) {
    strcpy (d->chapter[0].files[d->chapter[0].numFiles], file);
    d->chapter[0].numFiles++;
    // Empty if d->chapter[0].numFiles == 0
  }
  // Add to Dictionary [minLength]
  if (!inDictionary (file, minLength)) {
    strcpy (d->chapter[minLength].files[d->chapter[minLength].numFiles], file);
    d->chapter[minLength].numFiles++;
    d->numChapters = d->numChapters < minLength + 1 ? minLength + 1 : d->numChapters;
    // Empty if d->chapter[minLength].numFiles == 0
  }
}

void printChapter (struct dictionary *d, int c) {
  if (d->chapter[c].numFiles == 0) {
    return;
  }
  printf ("  chapter[%3d]: (%3d files)\n", c, d->chapter[c].numFiles);

  for (int i = 0; i < d->chapter[c].numFiles; i++) {
    printf ("    %s\n", d->chapter[c].files[i]);
  }
}

void printDictionary (struct dictionary *d) {
  printf ("DICTIONARY (%3d chapters)\n", d->numChapters);
  printf ("-------------------------\n");

  for (int i = 0; i < d->numChapters; i++) {
    printChapter (d, i);
  }
  printf ("\n");
}
void intersection (struct chapter *accumulator, struct chapter *comparator) {
  for (int i = 0; i < accumulator->numFiles; i++) {
    bool intersection = false;
    for (int j = 0; j < comparator->numFiles; j++) {
      if (!strcmp (accumulator->files[i], comparator->files[j])) {
        intersection = true;
        break;
      }
    }
    if (!intersection) {
      strcpy (accumulator->files[i], "");
    }
  }
}
void fileIntersection (struct dictionary *d) {
  for (int i = 1; i < d->numChapters; i++) {
    if (d->chapter[i].numFiles != 0) {
      intersection (&d->chapter[0], &d->chapter[i]);
    }
  }
}
void possibleFile (struct dictionary *d) {
  printf ("POSSIBLE FILES\n");
  printf ("--------------\n");
  for (int i = 0; i < d->chapter[0].numFiles; i++) {
    if (!strcmp (d->chapter[0].files[i], "")) {
      continue;
    }
    else (printf ("%s\n", d->chapter[0].files[i]));
  }
}