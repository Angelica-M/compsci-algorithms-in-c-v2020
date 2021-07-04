// To compile: 
//    1) Change OUTPUTFILENAME to the appropriate output file (e.g. "lab3c.out")
//    2) Enter "gcc -o Dynamic_Programming_Subset_Sums Dynamic_Programming_Subset_Sums.c -lm" 
//       then "./Dynamic_Programming_Subset_Sums < inF.txt" where inF.txt is the input file shell directed to stdin
// Code below can be tested with Inputs A, B, C, and D.

#include <stdio.h>  
#include <stdlib.h>       	
#include <string.h> 	
#include <assert.h>  
#include <math.h> 
#define OUTPUTFILENAME "lab3a.out" // output file name
#define MAXCHARSPERFILELINE 30 // maximum characters that can be read from a line of the input file
int sequenceSize; // total number of subsequences in input sequence

/*
Learning Goals:
1. Understanding of dynamic programming & subset sums
Requirements:
1.  Use dynamic programming to determine two separate subsequences of the input 
        such that the 1st subsequence sums to the 1st target value and the 2nd
        subsequence sums to the 2nd target value.
    Input is read from standard input (I'm using shell redirect (<) from a file). 
    The first line of the input will give n, the length of the sequence, along 
        with the two target values. Each of the remaining input lines will 
        include one sequence value. All values will be positive integers.
    Your program should echo the target values and the input sequence. If a problem instance has a
        solution, each of the two subsequences should be output and clearly labeled. A message should be
        provided for instances without a solution.
*/

int findTarget(int memoArr[][sequenceSize], int numOfSumValues, int target1, int target2, FILE* fOutPtr)
{   int indexOfT1 = -1; 
    int indexOfT2 = -1; 
    int r, c; 
    r=0; while(r < numOfSumValues) 
        {   if(memoArr[r][0] == target1)
            {   indexOfT1 = r;
                r = numOfSumValues;
            } r++;
        }
    r=0;
    if(target1==target2)
        r=indexOfT1+1; // continue where we left off
    while(r < numOfSumValues) 
        {   if(memoArr[r][0] == target2)
                {   indexOfT2 = r;
                    r = numOfSumValues;
                } r++;
        }
    if(indexOfT1 != -1 && indexOfT2 != -1)
        {   int isSameSubsequence = 1; 
            for(c=1; c<sequenceSize; c++) // check if elements that summed to target values are the same subsequences
                {   if(memoArr[indexOfT1][c] != memoArr[indexOfT2][c])
                        isSameSubsequence = 0;  
                }
            if(isSameSubsequence)
                {   printf("\nThere is no solution.\n"); // There is no solution as subsequences for each target value are identical.
                    fprintf(fOutPtr, "\nThere is no solution.\n");
                }
            else
                {   printf("\nBoth target values found.\nSubsequence for %d.\n", target1);
                    fprintf(fOutPtr, "\nBoth target values found.\nSubsequence for %d.\n", target1);
                    for(c=1; c<sequenceSize; c++) // print elements involved in summation
                    {   if(memoArr[indexOfT1][c] != -1)
                            {   printf("%d\n", memoArr[ (memoArr[indexOfT1][c]) ][0]); // for debugging, prints to screen
                                fprintf(fOutPtr, "%d\n", memoArr[ (memoArr[indexOfT1][c]) ][0]);
                            }
                    }
                    printf("Subsequence for %d.\n", target2);
                    fprintf(fOutPtr, "Subsequence for %d.\n", target2);
                    for(c=1; c<sequenceSize; c++) // print elements involved in summation
                    {   if(memoArr[indexOfT2][c] != -1)
                            {   printf("%d\n", memoArr[ (memoArr[indexOfT2][c]) ][0]); // for debugging, prints to screen
                                fprintf(fOutPtr, "%d\n", memoArr[ (memoArr[indexOfT2][c]) ][0]);
                            }
                    }
                }
        }
    else
        {   printf("\nThere is no solution.\n"); // Neither target value was found.
            fprintf(fOutPtr, "\nThere is no solution.\n");
        }
    printf("This output was also printed to %s. Type vim %s to access.\n", OUTPUTFILENAME, OUTPUTFILENAME);
    return 0;
}

int validSubsequence(int memoArr[][sequenceSize],int sequenceSize, int origSeqItr, int prevSumItr) // works correctly
{   int isValid = 1;
    int c; for(c=1; c<sequenceSize; c++) 
        {   if(memoArr[prevSumItr][c] != -1) // iterate through columns of row prevSumItr
            {   if(origSeqItr == memoArr[prevSumItr][c] || memoArr[prevSumItr][c] > origSeqItr) // Does origSeqItr appear in memoArr[prevSumItr]?
                    isValid = 0;
            }
        }
    return isValid;
}

int bottomUp(int memoArr[][sequenceSize],int numOfSumValues, int sequenceSize) // Uses dynamic programming to complete the rest of memoArr, which is passed by reference
{   // memoArr is passed by reference
    int origSeqItr = 0; // index of a sum variable that is from the sequence, value ranges from 0 to (sequenceSize-1)
    int prevSumItr = 0; // index of the second sum variable that is usually previous sum, value ranges from 0 to (numOfSumValues-1)
    int rowItr = sequenceSize; // rowItr, an index, is used to insert newly computed sums, value ranges from sequenceSize to (numOfSumValues-1)
    while(rowItr < numOfSumValues) // rowItr can be values sequenceSize to (numOfSumValues-1)
        {   if(validSubsequence(memoArr, sequenceSize, origSeqItr, prevSumItr) && prevSumItr!=origSeqItr) // true if new summation should be computed
                {   memoArr[rowItr][0] = memoArr[origSeqItr][0] + memoArr[prevSumItr][0]; // transfer summed value
                    int c, lc; for(c=1; c<sequenceSize; c++) // transfer indexes of elements that were summed
                        {   if(memoArr[prevSumItr][c] != -1)
                                {   memoArr[rowItr][c] = memoArr[prevSumItr][c]; 
                                    lc = c; // last column = column on in loop
                                }
                        } // lc should always be defined at this point
                    memoArr[rowItr][lc+1] = origSeqItr; // transfer index of new element that was summed
                    rowItr++; // move to new row for insertion   
                }
            // printf("rowItr is %d, origSeqItr is %d, prevSumItr is %d.\n", rowItr, origSeqItr, prevSumItr); // for debugging
            if(origSeqItr!=sequenceSize-1) // origSeqItr has not reached the end of the original sequence
                {   origSeqItr++;
                }
            if(origSeqItr==sequenceSize-1) // origSeqItr reaches end of original sequence
                {   origSeqItr = 0; // set the index of the first sum variable to zero
                    prevSumItr++; 
                }         
        }
    // printf("Exiting bottomUp function.\n"); // for debugging
    return 0;
}

int main (int argc, char* argv[]) // shell direct is used to store filename in stdin
{   // OPEN OUTPUT FILE AND OBTAIN FIRST LINE OF SHELL DIRECTED INPUT FILE
    FILE* fOutPtr = fopen(OUTPUTFILENAME, "w"); 
    assert(fOutPtr != NULL); // open output file
    char* fileLine = (char*) malloc(MAXCHARSPERFILELINE * sizeof(char)); // holds an entire line of the input file so it can be strtok'd or atoi'd
    char* tempStr; // a tempStr used to hold tokens of fileLine so assignments can be made
    strcpy(fileLine, "\0"); // initialize fileLine with strcpy(dest, src)
    // READ FIRST FILE LINE AND STORE SEQUENCE SIZE, TARGET 1, AND TARGET 2
    size_t maxSizeOfFileLine = MAXCHARSPERFILELINE; 
    getline(&fileLine, &maxSizeOfFileLine, stdin); // getline(addrDestination, size, addrSource) returns charsReadFromLine as a char* // get the 1st file file line from stdin
    // Note: strtok does not allocate memory so variables assigned to it's return value should not be malloc'd
    tempStr = strtok(fileLine, " "); // get the 1st token from the fileLine
    sequenceSize = (int) atoi(tempStr); // global variable
    tempStr = strtok(NULL, " "); // will continue to tokenize fileLine as first argument is NULL
    int target1 = (int) atoi(tempStr);
    tempStr = strtok(NULL, "\n"); // will continue to tokenize fileLine as first argument is NULL
    int target2 = (int) atoi(tempStr);
    // STORE SEQUENCE ELEMENTS IN AN ARRAY
    int numOfSumValues = (int) pow(2.0, (double) sequenceSize); // possible number of subsequences of of a sequence of sequenceSize size & number of all summation combinations of sequence elements.
    
if(numOfSumValues>999) // for debugging
    numOfSumValues = 1000; // for debugging

    sequenceSize = sequenceSize +1; // +1 MUST be added so that a subsequence involving all all elements of our sequence can be included in a summation
    int memoArr[numOfSumValues][sequenceSize]; // memoize array
    // printf("Memory allocation success.\nThere are %d subsequences.\n", numOfSumValues); // for debugging
    int r, c; for(r=0; r<numOfSumValues; r++) // initialize the memoize array
    {   for(c=0; c<sequenceSize; c++)
            memoArr[r][c] = -1; 
    }
    // PRINT THE SEQUENCE THAT WAS INPUTTED
    printf("Targets are %d and %d.\n   i   S\n   -------\n", target1, target2); 
    fprintf(fOutPtr, "Targets are %d and %d.\n   i   S\n   -------\n", target1, target2); 
    for(r=0; r<sequenceSize; r++) 
    {   getline(&fileLine, &maxSizeOfFileLine, stdin);
        tempStr = strtok(fileLine, "\n");
        memoArr[r][0] = (int) atoi(tempStr);
        memoArr[r][1] = r; // column 0 represents sum, column 1 to sequenceSize-1 represents the index of sequence elements involved in the summation
        printf("   %d   %d\n", r, memoArr[r][0]); 
        fprintf(fOutPtr, "   %d   %d\n", r, memoArr[r][0]); 
    }
    // COMPUTE SUBSEQUENCE SUMS AND FILL THE REST OF THE MEMOIZE ARRAY
    bottomUp(memoArr, numOfSumValues, sequenceSize); // Use dynamic programming to complete the rest of the array
    // PRINT ENTIRE ARRAY FOR DEBUGGING
    /* // for debugging
    for(r=0; r<numOfSumValues; r++) // print all rows
    {   printf("\nRow %d of memoArr is: ", r); // for debugging
        for(c=0; c<sequenceSize; c++)
            printf("[%d] ", memoArr[r][c]); // for debugging, prints to screen
    }
    */
    // PRINT WHETHER TARGETS WERE FOUND OR NOT FOUND
    findTarget(memoArr, numOfSumValues, target1, target2, fOutPtr);
    // FREE ALLOCATED MEMORY
    free(fileLine);
    // printf("\nAll memory has been freed.\n"); // for debugging
    return 0;
}
