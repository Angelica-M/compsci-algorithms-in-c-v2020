// To compile: 
//    Enter "gcc -o Red_Black_Tree_using_Recursion Red_Black_Tree_using_Recursion.c" 
//    then "./Red_Black_Tree_using_Recursion" 

#include <stdio.h>  
#include <stdlib.h>       	
#include <string.h> 	
#include <assert.h>  
#include <math.h> // need to add flag -lm to gcc command is math.h is used
#define MAXSIZEOFTREE 200 // column size of our array
#define INITIALIZEVALUE 200 // value that is initialized in array, INITIALIZEVALUE must be equal to MAXSIZEOFTREE for data integrity
int scouterIndex; // used to traverse through inputTree array 

/*
Learning Goals:
1. Understanding of Red-Black Trees (structure & constraints).
2. Application of Recursion.
Requirements:
1. Produce a linear-time program that tests whether a binary search tree satisfies the red-black tree 
   properties. Input is given in the preorder traversal format. An “R” represents a red tree node, a 
   “B” represents a black tree node, and a “.” represents the sentinel. If a tree includes n data nodes, 
   then the sentinel will be processed n + 1 times. The input
      BBB..B..BB..RB..BR..R..
      ABC D EF GH IJ K                (This line is not part of the input.)
   corresponds to the tree depicted in the file RBTuR-Req1_InputTreeExample.JPG. 
   Input strings have a maximum length of 200 characters.
2. Examples of inputs through standard input (program does not prompt for an input file) include:
      BRB..B..BR..R..
      BBB..B..BB..RB..RB..B..
      BBRB..B..BR..R..BB..BR..RB..B..
      BRBR..R..BR..R..RBR..R..BR..R..
3. The program must print, to standard output, a determination on whether the tree satisfies the red-black 
   tree properties or does not. The program must read the inputted data structure recursively and print 
   the tree, from inputted data structure, recursively. 
4. An example of program output is (where the first line is the input):
      BBRB..B..BR..R..BB..BR..RB..B..
      BBRB..B..BR..R..BB..BR..RB..B..
      The Root should be Black and Children of Red nodes should be Black.
         Red-parent Black-children property? - Verdict: SATISFACTORY
      Black height should be 3 for the input tree to be a a Red-Black Tree.
         Black height property? - Verdict: UNSATISFACTORY
      Problem Instance Solution? - Verdict: Input is NOT a Red-Black Tree
*/

int findNextInsertIndex(int treeInfo[3][MAXSIZEOFTREE])
{   int rootRightChild = 0;
    int r, c; for(r=0; r<3; r++) // traverse treeInfo to find highest element that is not 200 and return it
        {   for(c=0; c<MAXSIZEOFTREE; c++)
                {   if(treeInfo[r][c] != INITIALIZEVALUE && treeInfo[r][c] > rootRightChild)
                        rootRightChild = treeInfo[r][c];
                }
        }
    return (rootRightChild + 1); // rootRightChild + 1 is the index of the root's right child in inputTree
}

int evaluateRight(char inputTree[MAXSIZEOFTREE], int treeInfo[3][MAXSIZEOFTREE], int insertIndex) //
{   treeInfo[0][scouterIndex] = insertIndex; // note parent relationship
    treeInfo[2][insertIndex] = scouterIndex; // note right child relationship    
    insertIndex = treeInfo[2][insertIndex]; // insertIndex becomes insertIndex's right child
    traverseLeftSide(inputTree, treeInfo, insertIndex); // go as far left until sentinel is found, then iterate through the parents of insertIndex and call evaluateRight
    // printf("Left evaluateRight\n"); // for debugging
    return 0;
}

int traverseLeftSide(char inputTree[MAXSIZEOFTREE], int treeInfo[3][MAXSIZEOFTREE], int insertIndex)
{   // go as far left until sentinel is found, then iterate through the parents of insertIndex and call evaluateRight
    int origInsertIndex = insertIndex;
    int sentinelFound = 0;
    while(sentinelFound == 0)  
        {   scouterIndex++;
            treeInfo[0][scouterIndex] = insertIndex; // note parent relationship
            treeInfo[1][insertIndex] = scouterIndex; // note left child relationship
            if(inputTree[scouterIndex] == '.') 
                {   sentinelFound = 1;
                    scouterIndex++; // move to second sentinel
                    treeInfo[0][scouterIndex] = insertIndex; // note parent relationship
                    treeInfo[2][insertIndex] = scouterIndex; // note right child relationship
                    // until insertIndex becomes the root, iterate through the parents of insertIndex
                    while(insertIndex != origInsertIndex) // origInsertIndex (1) was the argument value of insertIndex to traverseLeftSide()
                        {   insertIndex = treeInfo[0][insertIndex]; // move to the parent of insertIndex
                            // printf("Is 200?: insertIndex becomes %d\n", treeInfo[0][insertIndex]); // for debugging
                            if(treeInfo[2][insertIndex] == INITIALIZEVALUE) // if right child of insertIndex is INITIALIZEVALUE, then execute the code below 
                            {   scouterIndex++; 
                                evaluateRight(inputTree, treeInfo, insertIndex);
                            }
                        }
                }
             else
                insertIndex++;
        }
    // printf("Left traverseLeftSide\n"); // for debugging
    return 0;
} 

int traverseRightSide(char inputTree[MAXSIZEOFTREE], int treeInfo[3][MAXSIZEOFTREE], int insertIndex)
{   int i = insertIndex;
    while(inputTree[i] != '\0' && i < MAXSIZEOFTREE)
        {   if(inputTree[i-1] == '.' && inputTree[i] != '.')
                {   scouterIndex = i;
                    traverseLeftSide(inputTree, treeInfo, i); 
                }
            i++;
        }
    return 0;
}

int setUpTreeInfo(char inputTree[MAXSIZEOFTREE], int treeInfo[3][MAXSIZEOFTREE]) // sets up tree info
{   treeInfo[1][0] = 1; // note the parent of the 2nd element in inputTree is the root
    treeInfo[0][1] = 0; // note left child of the root is the 2nd element in inputTree
    int insertIndex = 1; 
    scouterIndex = 1; // will be incremented in traverseLeftSide so incrementation is not needed here
    traverseLeftSide(inputTree, treeInfo, insertIndex); 
    scouterIndex = findNextInsertIndex(treeInfo); // scouterIndex is the root's right child 
    // printf("traverseLeftSide ends: scouterIndex is _%d_.\n", scouterIndex); // for debugging
    treeInfo[0][scouterIndex] = 0; // note the parent of the root's right child is the root
    treeInfo[2][0] = scouterIndex; // note right child of the root is the element with index scouterIndex in inputTree
    insertIndex = scouterIndex; // insertIndex is now the index of the root's right child in inputTree
    // At this point in the code, scouterIndex+1 will be the index of the root's right child's left child in inputTree
    traverseRightSide(inputTree, treeInfo, insertIndex); 
    return 0;
}

int printRecursively(char inputTree[MAXSIZEOFTREE], int index) // prints recursively
{   if(inputTree[index] != '\0') // if a R, B, or . is in inputTree, then ...
        {   printf("%c", inputTree[index]);
            index++;
            printRecursively(inputTree, index); // prints recursively
        }
    return 0;
}

int testChildrenOfRedNode(char inputTree[MAXSIZEOFTREE], int treeInfo[3][MAXSIZEOFTREE], int index, int isValid) // uses recursion 
{   // find 1st red node and check to see if it is valid, stop recursion when isValid becomes 0 or another Red node is not found
    int foundRedNode = 0; 
    while(inputTree[index+1] != '\0' && foundRedNode == 0) // if a R, B, or . is in inputTree & foundRedNode is false, then ...
        {   if(inputTree[index] == 'R')
                 foundRedNode = 1;
            else // move to next element in inputTree
                index++;// 
        }
    // printf("Test 1: inputTree[%d] is %c. foundRedNode is %d.\n", index, inputTree[index], foundRedNode); // for debugging
    if(foundRedNode == 1)
        {   // check if isValid should be assigned to 0 by checking the whether the left or right kids of node is Red
            if(inputTree[treeInfo[1][index]] == 'R' || inputTree[treeInfo[2][index]] == 'R') // checks if left and right child is a red node
               isValid = 0; // inputTree is not valid if left or right child of the red node found at index is a red node
            if(isValid == 1) // if we were unable to disprove on the red node we found, run test again.
                {   // printf("Test 1: inputTree[%d] is %c. Calling again.\n", index, inputTree[index], foundRedNode); // for debugging
                    index++;
                    isValid = testChildrenOfRedNode(inputTree, treeInfo, index, isValid);
                }
        }
    // printf("Test 1: Returning %d for isValid.\n", isValid); // for debugging
    return isValid; // returns 1 if inputTree passes test, 0 otherwise
}

int testRootToLeafDepth(char inputTree[MAXSIZEOFTREE], int treeInfo[3][MAXSIZEOFTREE], int index, int isValid, int lastDepthFound) // uses recursion
{   // find 1st leaf and trace it to root and compare to the last depth found, stop recursion when isValid becomes 0 or another sentinel is not found
    int tempIndex = 0;
    int foundSenteniel = 0; 
    while(inputTree[index] != '\0' && foundSenteniel == 0) // if a R, B, or . is in inputTree & foundSenteniel is false, then ...
        {   if(inputTree[index] == '.')
                 foundSenteniel = 1;
            else // move to next element in inputTree
                    index++;
        }
    // printf("Test 2: inputTree[%d] is %c. foundSenteniel is %d.\n", index, inputTree[index], foundSenteniel); // for debugging
    if(foundSenteniel == 1)
        {   // check if isValid should be assigned to 0 by checking if the former depth found is different than this depth found
            tempIndex = index;
            int depthFound = 0; // find depth
            while(treeInfo[0][tempIndex] != INITIALIZEVALUE) 
                {   tempIndex = treeInfo[0][tempIndex]; // move to the parent of index
                    if(inputTree[tempIndex] != 'R') // only black height of the root to any sentinel matters
                        depthFound++;
                    // printf("Test 2: treeInfo[0][%d] is %d. depthFound is %d.\n", tempIndex, treeInfo[0][tempIndex], depthFound); // for debugging
                }
            // printf("Test 2: The depth of treeInfo[0][%d] has been found. index is %d. lastDepthFound is %d. depthFound is %d.\n", tempIndex, index, lastDepthFound, depthFound); // for debugging
            if(lastDepthFound != 0 && depthFound != lastDepthFound) // if this is not the first time testRootToLeafDepth() has been called and depths vary
                isValid = 0; // inputTree is not valid if left or right child of the red node found at index is a red node
            lastDepthFound = depthFound;
            if(isValid == 1) // if we were unable to disprove on the red node we found, run test again.
                {   index++;
                    // printf("Test 2: index is %d. Calling again.\n", index); // for debugging
                    isValid = testRootToLeafDepth(inputTree, treeInfo, index, isValid, lastDepthFound);
                }
        }
    return isValid; // returns 1 if inputTree passes test, 0 otherwise
}

int validate(char inputTree[MAXSIZEOFTREE], int treeInfo[3][MAXSIZEOFTREE], int index, int isValid) // validate(inputTree, treeInfo, 0, 1); 
{   isValid = testChildrenOfRedNode(inputTree, treeInfo, index, isValid); // uses recursion
    // printf("In validate(): isValid is %d.\n", isValid); // for debugging
    if(isValid == 1)  // if inputTree is found to be valid in first test, then perform 2nd test 
        {   isValid = testRootToLeafDepth(inputTree, treeInfo, index, isValid, 0); // uses recursion
        }
    return isValid; // returns 1 if inputTree follows red-black properties, 0 otherwise
}

int main(int argc, char* argv[]) 
{   // DECLARE AND INITIALIZE inputTree ARRAY
    char inputTree[MAXSIZEOFTREE];
    int r, c; 
    for(c=0; c<MAXSIZEOFTREE; c++) // initalize inputTree array
        inputTree[c]= '\0'; // a space character is 32 in ASCII, 0 is NULL // 0 prevents confusion of another node being a child
    
    // PROMPT FOR INPUT LINE
    printf("Enter a preorder traversal of a binary tree (\"B\" for black, \"R\" for red, & \".\" for sentinel) with no spaces:\n");
    scanf("%s", inputTree);
    
    // Test cases are below are for debugging, code works correctly with all test cases
        // strcpy(inputTree, "BBB..B..BB..RB..BR..R..\0"); // test case 1 // passes Test 1 and 2
        // strcpy(inputTree, "BRB..B..BR..R..\0"); // test case 2 // passes Test 1 and 2
        // strcpy(inputTree, "BBB..B..BB..RB..RB..B..\0"); // test case 3 // fails Test 1 and passes Test 2
        // strcpy(inputTree, "BBRB..B..BR..R..BB..BR..RB..B..\0"); // test case 4 // passes Test 1 and fails Test 2
        // strcpy(inputTree, "BRBR..R..BR..R..RBR..R..BR..R..\0"); // test case 5 // passes Test 1 and 2
     // printf("Input: inputTree is _%s_.\n", inputTree); // for debugging
    
    // DECLARE AND INITIALIZE treeInfo ARRAY
    int treeInfo[3][MAXSIZEOFTREE]; // rows 0 is parent, row 1 is left child, row 2 is right child, columns correspond to each element in inputTree
    for(r=0; r<3; r++) // initialize array
        {   for(c=0; c<MAXSIZEOFTREE; c++)
                treeInfo[r][c]= INITIALIZEVALUE; // initialized to 200
        }
    
    // SET UP NODE RELATIONS AND DATA ABOUT inputTree IN THE treeInfo ARRAY
     setUpTreeInfo(inputTree, treeInfo); 
        
    // PRINTS FOR DEBUGGING PURPOSES
    printf("\nDebugging info: \n"); 
    for(c=0; c<MAXSIZEOFTREE; c++) // prints the first row of array
        {   if(inputTree[c] != '\0') // prints until terminating null character is found
                printf("\tinputTree[%d] is _%c_.\tparent:_%d_ \tleft-child: _%d_ \tright-child: _%d_.\n", c, inputTree[c], treeInfo[0][c], treeInfo[1][c], treeInfo[2][c]); 
            else if(inputTree[c] == '\0')
                c = MAXSIZEOFTREE; // stops the for loop because we have already printed entire input
        }
    
    // ECHO (OR PRINT) THE TREE AND CHECK, IN LINEAR TIME, IF THE TREE SATISFIES ALL RED-BLACK TREE PROPERTIES USING RECURSION
    printf("\ninputTree recursively printed is: \n"); 
    printRecursively(inputTree, 0); // prints recursively
    printf("\n");
    int isRedBlackTree = validate(inputTree, treeInfo, 0, 1); // inputTree is passed by reference, assume inputTree is a valid red-black tree until proven otherwise, validate() calls functions that uses recursion
    if(isRedBlackTree == 1)
        printf("\nTree is a red-black tree.\n");
    else // isRedBlackTree == 0 
        printf("\nTree is not a red-black tree.\n");
    return 0;
}
