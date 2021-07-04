// To Compile:
//    Enter "gcc -o Heapsort_of_Files_Contents Heapsort_of_Files_Contents.c" 
//    then ./Heapsort_of_Files_Contents"
// Code below can be tested with Inputs A and B.

#include <stdio.h>  
#include <stdlib.h>       	
#include <string.h> 	
#include <assert.h> 
#define MaxCharsPerInputFileName 50 // How many chars are needed to hold "input_file_name_example.dat\n" file name of any file in the input directory
#define MaxCharsPerInputFileLine 50 // Note: each "in#.dat" file will have a max of 50 chars per a line
#define MaxLinesPerInputFile 100 

/*
Learning Goals:
1. Understanding of heaps & merging.
Requirements:
1. The C program must read n files where each file contains strings in ascending order (a file has no duplicates) 
   and generate the file out.dat. This output file has a line for each string (in ascending order) and the number 
   of files (k) that the string resides in. If a string str resides in more than one file, it will only be 
   outputted once. The program must perform only one heap-assisted, simultaneous merge of all n files.
2. The program's input file is in.dat:
      a. The numerical value for n resides in the first line
      b. A single filename resides in each of the following n lines
      c. Each of the n files will have at least one string that is up to 50 characters.
3. Pseudo-code:
      a. Open & read in.dat, then each of the n files, and then, finally, out.dat.
      b. Populate the heap with the first string of each file. Use a minHeap with
         the smallest strcmp() string at the root, theoretically.
      c. While at least one file remains to be read:
         1. Remove, from the heap, the minimum string.
         2. if the previous minimum is not equal to our new minimum string
               Print output in the file out.dat
            else
               Change k
         3. Try, within the same file, reading the next string.
               if EOF
                  Shrink the heap
               else
                  Place the string inside the heap
      d.  Last string is outputted, free any allocated memory, & close files.
*/

typedef struct Nodes 
{   char* fileLine;
    int appearances;
} Node;

void maxHeapify(Node nodeArr[], int sizeOfNodeArr, int i) // sizeOfNodeArr is size of nodeArr AKA heap
{   int largestIndex = i; // Initialize largestIndex as root, i=0
    int leftIndex = 2*i + 1; // leftIndex = 2*i + 1 
    int rightIndex = 2*i + 2; // rightIndex = 2*i + 2 
    if (leftIndex < sizeOfNodeArr && (strcmp(nodeArr[leftIndex].fileLine, nodeArr[largestIndex].fileLine) > 0)) // If left child is larger than root: nodeArr[leftIndex] > nodeArr[largestIndex] 
        {   largestIndex = leftIndex;   
        }
    if (rightIndex < sizeOfNodeArr && (strcmp(nodeArr[rightIndex].fileLine, nodeArr[largestIndex].fileLine) > 0)) // If right child is larger than largest so far: nodeArr[rightIndex] > nodeArr[largestIndex]
        {   largestIndex = rightIndex;  
        } 
    if (largestIndex != i) // If largest is not root 
    {   char* tempStr = (char*) malloc(MaxCharsPerInputFileLine * sizeof(char));
        int tempInt; 
        strcpy(tempStr, nodeArr[i].fileLine); // swap(nodeArr[i], nodeArr[largestIndex])
        tempInt =  nodeArr[i].appearances;
        strcpy(nodeArr[i].fileLine, nodeArr[largestIndex].fileLine);
        nodeArr[i].appearances = nodeArr[largestIndex].appearances;
        strcpy(nodeArr[largestIndex].fileLine, tempStr); 
        nodeArr[largestIndex].appearances = tempInt;
        free(tempStr);
        maxHeapify(nodeArr, sizeOfNodeArr, largestIndex); // Recursively maxHeapify the affected sub-tree 
    } 
}

void heapSort(Node nodeArr[], int sizeOfNodeArr) 
{   int i; for(i=(sizeOfNodeArr/2)-1; i>=0; i--) // Build heap (rearrange array) 
    {   maxHeapify(nodeArr, sizeOfNodeArr, i); 
    }
    for(i=sizeOfNodeArr-1; i>=0; i--) // One by one extract an element from heap 
    {   char* tempStr = (char*) malloc(MaxCharsPerInputFileLine * sizeof(char));
        int tempInt; 
        strcpy(tempStr, nodeArr[0].fileLine); // Move current root to end with swap(nodeArr[0], nodeArr[i])
        tempInt =  nodeArr[0].appearances;
        strcpy(nodeArr[0].fileLine, nodeArr[i].fileLine);
        nodeArr[0].appearances = nodeArr[i].appearances;
        strcpy(nodeArr[i].fileLine, tempStr);
        nodeArr[i].appearances = tempInt;
        free(tempStr);
        maxHeapify(nodeArr, i, 0); // call maxHeapify on the reduced heap 
    } 
}

void printArray(Node nodeArr[], int sizeOfNodeArr, FILE* fOutPtr) 
{   printf("All data in alphabetical order is:\n"); // for debugging
    fprintf(fOutPtr,"All data in alphabetical order is:\n\n");
    fprintf(fOutPtr,"fileline\t\tappearances\n\n");
    int i; for(i=0; i<sizeOfNodeArr; i++)
    {   if(strcmp(nodeArr[i].fileLine, "\0")) // If nodeArr[i].fileLine[i] is not "\0", then...
            {   // printf("\tAfter sort: nodeArr[%d].fileLine is _%s_ and appears %d times.\n", i, nodeArr[i].fileLine, nodeArr[i].appearances); // for debugging
                printf("%s\t\t%d\n", nodeArr[i].fileLine, nodeArr[i].appearances); // for debugging
                fprintf(fOutPtr,"%s\t\t%d\n", nodeArr[i].fileLine, nodeArr[i].appearances); 
            }
    }
    printf("Data was also printed to out.dat\n"); // for debugging
}

int main(char* argc, char ** argv)
{   // OPEN .DAT FILES AND DECLARE fNameArr AND fPtrArr ARRAYS 
    FILE* fInPtr = fopen("in.dat", "r"); 
    assert(fInPtr != NULL); 
    FILE* fOutPtr = fopen("out.dat", "w"); 
    assert(fOutPtr != NULL); // Both files are opened
    char* tempStr = (char*) malloc(MaxCharsPerInputFileName * sizeof(char)); 
    size_t maxSizeOfFileLine = MaxCharsPerInputFileName; 
    getline(&tempStr, &maxSizeOfFileLine, fInPtr); // getline(addrOfWhereToStore,intSize,addrWhereToGrab) returns CharactersReadFromLine as a string data type
    int numOfFiles = (int) atoi(tempStr);
    char** fNameArr = (char**) malloc(numOfFiles * sizeof(char*));  // holds file names that are listed in the in.dat file // fNameArr is the same as fNameArr[0], fNameArr is a char* with char*'s stored after it, char*'s are stored in the array
    FILE** fPtrArr = (FILE**) malloc(numOfFiles * sizeof(FILE*)); // holds pointers to the files whose name is stored in fNameArr
    // OPEN EVERY FILE WHOSE NAME IS STORED IN fNameArr AND STORE THE POINTER TO THOSE FILES IN fPtrArr
    char* addrOfNewlineChar = (char*) malloc(1 * sizeof(char));
    char* tempStr2 = (char*) malloc(MaxCharsPerInputFileName * sizeof(char));
    int fileNameSize;
    int i; for(i=0; i<numOfFiles; i++)
    {   *(fNameArr+i) = (char*) malloc(MaxCharsPerInputFileLine * sizeof(char));
        getline((fNameArr+i), &maxSizeOfFileLine, fInPtr); // getline(addrOfCharPtrOfWhereToPutIt, size_tHowManyCharsIn, filePtrOfWhereItCameFrom), 1st arg is in format &charPtrName)
        // Use strncpy to get rid of '\n' that is copied over at the end of file names by using  strncpy with n-1 as it's last argument where n==sizeOfFileName
        tempStr2 = strtok(fNameArr[i], "\n"); // get rid of \n
        strcpy(fNameArr[i], tempStr2);  
        fPtrArr[i] = (FILE*) malloc(sizeof(FILE*)); 
        fPtrArr[i] = fopen(fNameArr[i], "r");
        if(fPtrArr[i]==NULL) 
            {   printf("%d is in fPtrArr[%d] named _%s_.\n", fPtrArr[i], i, fNameArr[i]);    
            }
        assert(fPtrArr[i] != NULL); 
    }
    // CONSTRUCT A STRING ARRAY IN WHICH ALL LINES FOUND IN ALL INPUT FILES WILL BE STORED UNSSORTED
    char** allLinesFromInputArr = (char**) malloc(numOfFiles * MaxLinesPerInputFile * sizeof(char*)); // holds file lines or strings found in input files
    int indexOfAllLinesFromInputArr = 0; 
    // READ EVERY FILE WHOSE POINTER IS IN fPtrArr AND STORE THEIR LINES (A STRING) IN CHAR ARRAY fLineArr 
    size_t maxSizeOfCharsPerLine = MaxCharsPerInputFileLine; 
    // focus on obtaining file lines from one file then loop it for every file in fPtrArr
    char** fLineArr = (char**) malloc(MaxLinesPerInputFile * sizeof(char*)); // holds file lines or strings found in input files
    for(i=0; i<MaxLinesPerInputFile; i++) // loop through every line in an input file
        {   *(fLineArr+i) = (char*) malloc(MaxCharsPerInputFileLine * sizeof(char));
        }
    char* tempStr3 = (char*) malloc(MaxCharsPerInputFileLine * sizeof(char));
    for(i=0; i<numOfFiles; i++) // loop through every input file
    {   int j; for(j=0; j<MaxLinesPerInputFile; j++) // loop through every line in an input file
        {   strcpy(fLineArr[j], "\0"); // fLineArr[j] must be reset so data contamination doesn't happen
            getline((fLineArr+j), &maxSizeOfCharsPerLine, fPtrArr[i]); // getline(addrOfCharPtrOfWhereToPutIt, size_tHowManyCharsIn, filePtrOfWhereItCameFrom), 1st arg is in format &charPtrName)
            int k, m; for(k=0; k<MaxLinesPerInputFile; k++) // Loop through fLineArr[j] until \n is found, use this loop to find n value so strncpy can be performed
                {   if (fLineArr[j][k]=='\n')
                        {   m = k; // newline character is found at index m
                        }
                }
            strcpy(tempStr3, "\0"); // initialize tempStr3 so printf("_%s_", tempStr3); will print as __
            int m2 = m;
            int q; for(q=0; q<MaxCharsPerInputFileLine; q++) // loop through chars in a file line
            {   if(m2) // if m is not zero
                    {   tempStr3 = strcat(tempStr3, "\n");
                        m2--;
                    }
                else // if m is zero
                    {    tempStr3 = strcat(tempStr3, "\0");   
                    }
            } 
            strncpy(tempStr3, fLineArr[j], m); // Copies up to m characters from the string pointed to, by char* fLineArr[j] to  char* tempStr3.
            strcpy(fLineArr[j], tempStr3); // This code and line above removes the trailing '\n' in fLineArr[j] // fLineArr[j] &  tempStr3 are now identical
            if( !(*fLineArr[j]==0) && m!=0) 
                {   *(allLinesFromInputArr+indexOfAllLinesFromInputArr) = (char*) malloc(MaxCharsPerInputFileLine * sizeof(char));
                    strcpy(allLinesFromInputArr[indexOfAllLinesFromInputArr], fLineArr[j]);
                    indexOfAllLinesFromInputArr++;
                }
        } 
    } // indexOfAllLinesFromInputArr now represents how many valid lines total from input files there are
    // CREATE AN UNSORTED ARRAY nodeArr OF NODE STRUCTURES
    int sizeOfNodeArr = indexOfAllLinesFromInputArr; // indexOfAllLinesFromInputArr now represents how many valid lines total from input files there are
    Node nodeArr[sizeOfNodeArr]; // sizeOfNodeArr is the size of nodeArr & allLinesFromInputArr
    for (i=0; i<(sizeOfNodeArr); i++) // traverse, allocate memory, and initialize
    {   nodeArr[i].fileLine = (char*) malloc(MaxCharsPerInputFileLine * sizeof(char)); // malloc filename
        strcpy(nodeArr[i].fileLine, "\0");
        nodeArr[i].appearances = 0;
    }
    int indexOfNodeArr = 0; // index of nodes whose fileline is not "\0", sizeOfNodeArr nodes have their fileline malloc'd
    for(i=0; i<(sizeOfNodeArr); i++) // loop through allLinesFromInputArr &, for every non-repeating line, strcpy into nodeArr otherwise increment appearances
        {   if(!(*allLinesFromInputArr[i]==0)) // If a line is not "\0", then...
                {   int alreadyInNodeArr = -1; // ==-1 is false & >=0 is true (the index of appearance in nodeArr}
                    int j; for (j=0; j<(sizeOfNodeArr); j++) // loop through nodeArr to check if allLinesFromInputArr[i] is already in nodeArr 
                        {   if(!(strcmp(allLinesFromInputArr[i], nodeArr[j].fileLine))) // If nodeArr[j].fileLine & allLinesFromInputArr[i] are identical, then... // should only be activated once or zero per allLinesFromInputArr[i]
                            {   alreadyInNodeArr = j; // alreadyInNodeArr = the index of nodeArr were allLinesFromInputArr[i] appears
                            } 
                        }
                    if(alreadyInNodeArr>=0) // if alreadyInNodeArr is true, then...
                        {   nodeArr[alreadyInNodeArr].appearances = nodeArr[alreadyInNodeArr].appearances +1; // & move on to next i value (i++)   
                        }
                    else if(alreadyInNodeArr==-1) // if alreadyInNodeArr is false, then...
                        {   strcpy(nodeArr[indexOfNodeArr].fileLine, allLinesFromInputArr[i]);
                            nodeArr[indexOfNodeArr].appearances = 1;
                            indexOfNodeArr++; 
                        }
                }
        } 
    // MERGE ALL LINES FOUND IN INPUT FILES USING A HEAPSORT
    heapSort(nodeArr, sizeOfNodeArr); 
    printArray(nodeArr, sizeOfNodeArr, fOutPtr);
    // FREE ALLOCATED MEMORY
    free(addrOfNewlineChar);
    free(tempStr);
    free(tempStr2);
    free(tempStr3);
    for(i=numOfFiles-1; i>=0; i--)
        {   free(*(fNameArr+i));
            free(*(fPtrArr+i)); 
        }
    for(i=MaxLinesPerInputFile; i>=0; i--)
        {   free(*(allLinesFromInputArr+i));
            free(*(fLineArr+i));
        }
    free(fNameArr); // name of pointer arrays MUST be freed after contents (pointers) of pointer array are freed
    free(fPtrArr);  
    free(allLinesFromInputArr);
    free(fLineArr);
    for(i=0; i<sizeOfNodeArr; i++) // loop through every line in an input file
        {   free(nodeArr[i].fileLine);
        }
    printf("%s\n", "All memory has been freed."); 
    return 0;
}
