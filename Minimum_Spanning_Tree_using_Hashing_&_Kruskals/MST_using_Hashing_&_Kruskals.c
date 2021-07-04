// To compile: 
//    1) Change OUTPUTFILENAME to the appropriate output file (e.g. "lab5a.out")
//    2) Enter  "gcc -o MST_using_Hashing_&_Kruskals MST_using_Hashing_&_Kruskals.c" 
//       then "./MST_using_Hashing_&_Kruskals < lab5a.dat" 

#include <stdio.h>  
#include <stdlib.h>       	
#include <string.h> 	
#include <assert.h>  
#define INITIALIZEVALUE "-1\0" // value that arrays are initialized to
#define MAXCHARSPERFILELINE 200 // holds an entire line of the input file so it can be strtok'd or atoi'd
#define MAXCHARSPERDATAVALUE 30 // used to malloc every element of the vertexTable and edgeTable
#define OUTPUTFILENAME "lab5a.out" // output file name

/*
Learning Goals:
1. Understanding of hashing, graphs, & Krukal’s algorithm.
Requirements:
1. Design, code, and test a C program to determine a minimum spanning forest for a weighted, directed graph.
   a. Input is to be read from standard input:
      1. The first line is two integer values: n, the number of vertices, and m, the number of edges.
      2. The remaining m lines will each contain three values defining an edge: a tail name (string of no more than 25
      characters), a head name (another string), and an integer value for the weight of the edge.
   b. While reading the input, new vertex names should be stored in a double hash table along with consecutively assigned
   vertex numbers. In addition, a table of vertex names to allow printing the name for a vertex number will be needed.
   If the input does not include n different vertex names, give a warning and continue. If the input has more than n
   different names, print a warning and stop. Your double hash table should have the smallest prime size 
   to assure a load factor no larger than 50%. You will need code for a simple function to compute this value.
   c. Sort the edges by ascending weights using any technique.
   d. Initialize the disjoint-subset representation.
   e. Perform Kruskal’s algorithm; you may overwrite the rejected edges with edges that are kept, but
   avoid Theta(n^2) implementation. 
   f. Print the trees in the forest. If the input graph is connected, there will be only one tree in the final result. Otherwise,
   the edges must be sorted so that the edges in each tree are output together and labeled as such. The edges in each tree 
   will be printed by their vertex name and in ascending order. 
*/

int findNumOfCycles(int* minSpanningTree, int vertexNum, char*** edgeTable, int numOfEdges, char** leaderArr, int sizeOfLeaderArr, char* tempStr)
{   int cycles [vertexNum-2][vertexNum-2]; // row maps to an edge in eT whose starting vertex is a leader, column maps to an edge in eT associated with that cycle
    int lAIndex = 0;
    int mSTIndex = 0;
    while(strcmp(edgeTable[mSTIndex][0], INITIALIZEVALUE) != 0) // iterates mST
        {   // check if edgeTable[mSTIndex][0] is an edge in mST whose starting vertex is a leader
            // if yes, check if its leader already has a row in cycles[] (if no, create a row with [r][0], else add onto the existing row) 
            // and traverse through mST adding edges that have a starting or ending vertex in common with an edge in its corresponding row in cycles[]
            mSTIndex++;
        }
    return 0; // leaderArrIndex is the number of leaders found
}

int inLeaderArr(char** leaderArr, int sizeOfLeaderArr, char* tempStr) // returns 0 if tempStr is not already in leaderArr and if leaderArr != NULL
{   int isInLeaderArr = 0;
    if(sizeOfLeaderArr != 0) // if sizeOfLeaderArr == 0 then leaderArr is empty
        {   int r; for(r=0; r<sizeOfLeaderArr; r++) // search for tempStr in leaderArr
                {   // printf("inLeaderArr: tempStr is %s. leaderArr[%d] is %s.\n", tempStr, r, leaderArr[r]); // FOR DEBUGGING
                    if(strcmp(tempStr, leaderArr[r]) == 0)
                        isInLeaderArr = 1;
                }   
        }
    return isInLeaderArr;
}

int findNumOfLeaders(int* minSpanningTree, int vertexNum, char*** edgeTable, int numOfEdges, char** leaderArr, int sizeOfLeaderArr, char* tempStr) // returns # of cycles formed
{   // tempStr is later assigned to the value of mST that we are searching for when finding the source/origin vertex of a disjoint tree within the solution mST
    int leaderArrIndex = 0; // index 0 of mST will always have its own solution
    int r; for(r=0; r<(vertexNum-2); r++) // traverse mST
        {   if(minSpanningTree[r] != -1) // for every element in mST, back-trace it to find the source vertex
                {   strcpy(tempStr, edgeTable[minSpanningTree[r]][0]); // the key (tempStr) is assigned to the starting index of the edge mapped to by mST
                    int r2 = 0;
                    int searchIsDone = 0;
                    // printf("DEBUG 1: tempStr or edgeTable[minSpanningTree[r]][0] is %s.\n", tempStr);
                    while(searchIsDone != 1) // back-trace mST to find an edge whose ending index is the key (tempStr)
                        {   // printf("DEBUG 2: tempStr is now %s. sizeOfLeaderArr is %d. r2 is %d. vertexNum-2 is %d\n", tempStr, sizeOfLeaderArr, r2, vertexNum-2);
                            if(r2 == (vertexNum-2))
                                {   // printf("DEBUG 4: inLeaderArr() is %d\n", inLeaderArr(leaderArr, sizeOfLeaderArr, tempStr)); // FOR DEBUGGING
                                    if(inLeaderArr(leaderArr, sizeOfLeaderArr, tempStr) == 0) // check if tempStr is not already in leaderArr before adding new vertex in leaderArr so redundancy does not occur
                                        {   // printf("Debug 5: LEADER IS %s.\n", tempStr); // FOR DEBUGGING, tempStr is the key that was never found to have an another vertex that led to it
                                            // add new vertex (tempStr) in leaderArr by allocating memory for a row 
                                            strcpy(leaderArr[leaderArrIndex], tempStr);
                                            leaderArrIndex++;
                                            // printf("DEBUG 6: leaderArrIndex is %d. leaderArr[%d] is %s.\n", leaderArrIndex, r, leaderArr[r]); // FOR DEBUGGING
                                        }
                                    searchIsDone = 1; // end the while-loop, at this point (where r2 == (vertexNum-2)) mST has been searched for the key and the key may or may not of been found
                                }
                            else if(minSpanningTree[r2] != -1 && strcmp(tempStr, edgeTable[minSpanningTree[r2]][1]) == 0) // if key matches the ending vertex of an edge in mST then change the key and reset r2
                                {   strcpy(tempStr, edgeTable[minSpanningTree[r2]][0]); // change key to the starting vertex of the edge that map to our original key/vertex-value
                                    r2 = -1; // reset r2 to start another back-trace search
                                }
                            r2++;
                        }
                }
        }
    return leaderArrIndex; // leaderArrIndex is the number of leaders found
}

int formsCycle(int* minSpanningTree, int vertexNum, char*** edgeTable, int numOfEdges, int eTIndex, char* tempStr) // returns 1 is a cycle is formed, 0 otherwise
{   // traverse through mST and check if eT[eTI] makes a cycle with mST, if not return 0.
    int formsCycle = 0; // assume a cycle is not formed at first
    char* startVertex = edgeTable[eTIndex][0]; // a cycle is formed if eT[eTI][1] maps to an edge in mST and edges in mST eventually map to eT[eTI][0]
    char* endVertex = edgeTable[eTIndex][1]; 
    // a cycle is formed if eT[eTI][1] or eV maps to a starting vertex of an edge in mST and edges in mST eventually map to eT[eTI][0] as an ending vertex
    // find the value in eT[eTI][1] in mST, if one is found then map through mST to find a value of eT[eTI][0] as a ending vertex, if found then a cycle is formed
    int runTestTwo = -1;
    int r; for(r=0; r<(vertexNum-2); r++) // traverse mST for values that are not equal to -1 (these are the indexes of eT that are part of the solution)
        {   if(minSpanningTree[r] != -1) // check if eT[eTI][1] or eV is equal to the starting vertex of an edge in mST
                {   if(strcmp(endVertex, edgeTable[minSpanningTree[r]][0]) == 0) // if endVertex (eV) is equal to the starting vertex of an edge in mST where mST[r] maps to the index of a row in eT, then perform 2nd test
                        {   runTestTwo = r; // runTestTwo is equal to the row of mST that, after mapping through edges in mST, may map to the startVertex (sV) 
                            break;
                        }
                }
        }
    if(runTestTwo != -1) // traverse mST to find a value of eT[eTI][0] as an ending vertex in mST, if eT[eTI][0] is found then a cycle is formed
        {   // runTestTwo = index of eT where endVertex is found as a starting index in mST
            strcpy(tempStr, endVertex); // check is tempStr is the starting index in an edge mapped to be mST
            int it = 0; // counts how many checks of tempStr (the starting index searched for) is in mST as a staring index
            while(it < (vertexNum-2))
                {   for(r=0; r<(vertexNum-2); r++) // traverse mST to find if tempStr is a starting index within it
                        {   if(minSpanningTree[r] != -1) 
                                {   if(strcmp(tempStr, edgeTable[minSpanningTree[r]][0]) == 0) // compare tempStr and every starting index of eT that is mapped to by mST
                                        {   strcpy(tempStr, edgeTable[minSpanningTree[r]][1]); // tempStr becomes ending vertex of row with matching starting vertices 
                                            if(strcmp(startVertex, tempStr) == 0)
                                                formsCycle = 1; // if sV is found as the ending vertex of an edge in mST, then a cycle is formed 
                                        }
                                }
                        }
                    it++;
                }
        }
    return formsCycle;
}

int krushalAlg(int* minSpanningTree, int vertexNum, char*** edgeTable, int numOfEdges, char* tempStr) // fills the minSpanningTree with a solution (the indexes that point to a row of edgeTable)
{   // Find the shortest path that connects all vertices (the least cost/weight solution) 
    int mSTIndex = 0;
    int eTIndex = 0;
    while(mSTIndex < (vertexNum-2) && eTIndex < numOfEdges) // include first (vertexNum-2) edges in solution that do NOT from a cycle & ensure that eTIndex does not exceed numOfEdges
        {   // traverse through eT and check if value makes a cycle with current solution
            if(formsCycle(minSpanningTree, vertexNum, edgeTable, numOfEdges, eTIndex, tempStr) == 0) // if a cycle is not formed, add that edge to the solution
                {   minSpanningTree[mSTIndex] =  eTIndex; // add edge to solution
                    mSTIndex++; // increment mSTIndex after an edge has been added to the solution
                }
            eTIndex++;
        }
    return 0;
}

int isValidInsertIndex(char*** vertexTable, int hashVal)
{   int isValid = 0; // assume hashVal is not a valid insert index
    if(strcmp(vertexTable[hashVal][0], INITIALIZEVALUE) == 0) // strcmp() returns 0 if strings are identical
                isValid = 1; // the computed insert index (AKA hashVal) is not already occupied, so we can use this hashVal
    return isValid;
}

int getDoubleHash(char*** vertexTable, int vertexTableSize, int vertexNum) // returns the index of vertexTable where input should be inserted
{   //  h(key, i) = (h1(key) + i*h2(key)) % m, m = vertexTableSize, i = 1 and increments by 1 with every additional probe, key = vertexNum
    //        h1(key) = key % m                h2(key) = 1 + key % (m - 1)
    // if computed hashOne is a valid insertIndex (not in use), then use hashOne 
    // if computed hashOne is not valid (index already in use), then compute doubleHash over and over (with i++ at each consequent computation) until a valid hashVal (doubleHash) is found
    int hashVal = 0; // computed index from hashing or double hashing
    // COMPUTE hashOne AND CHECK IF IT IS VALID
    int hashOne = vertexNum % vertexTableSize; //  h1(key) = key % m
    if(isValidInsertIndex(vertexTable, hashOne) == 1)
        {   hashVal = hashOne; 
        }
    else // we must use double hash to avoid collision
        {   // compute hashTwo (this value will be not changing like how hashOne will not be changing)
            // printf("DOUBLE HASH HAS BEEN ACTIVATED. hashOne is %d.\n", hashOne); // FOR DEBUGGING
            int doubleHash = 0; // initialize doubleHash
            int i = 1;
            int hashTwo = 1 + vertexNum % (vertexTableSize - 1); // h2(key) = 1 + key % (m - 1)
            do  {   doubleHash = (hashOne + i * hashTwo) % vertexTableSize; // h(key, i) = (h1(key) + i*h2(key)) % m
                    i++;
                    // printf("To be verified: doubleHash computed is %d.\n", doubleHash); // FOR DEBUGGING
                }
            while(isValidInsertIndex(vertexTable, doubleHash) == 0); // do-while loop ends when doubleHash becomes valid
            hashVal = doubleHash; 
        }
    // printf("hashOne is %d. hashTwo is %d. doubleHash is %d.\n", hashOne, hashTwo, doubleHash); // FOR DEBUGGING
    // printf("hashVal computed is %d.\n", hashVal); // FOR DEBUGGING
    return hashVal;
}   

int insertInVertexTable(char*** vertexTable, int vertexTableSize, int numOfVertices, char* vertexName, int vertexNum) // check if fileLine is in vertexTable, if not add it using a hash or double hash
{   int isInVertexTable = 0; // 0 is false
    // strcpy(vertexTable[3][0], "HAHHAA"); // FOR TESTING CODE // Code works correctly
    int r; for(r=0; r<vertexTableSize; r++) // search vertexTable to check if fileLine is already inside it
        {   if(strcmp(vertexTable[r][0], vertexName) == 0) // strcmp(a,b) returns 0 when the strings are identical
                isInVertexTable = 1; // 1 is true
        }
    //printf("isInVertexTable is %d.\n", isInVertexTable); // FOR DEBUGGING
    if(isInVertexTable == 0) // if fileLine is not in vertexTable, then add fileLine to vertexTable
        {   if((vertexNum-1) > numOfVertices) // (vertexNum-1) is how many vertex names are found in the file
                {   printf("ERROR: More than %d vertices have been found in the file: %d have been found. Terminating with assert().\n", numOfVertices, vertexNum-1);
                    assert((vertexNum-1) <= numOfVertices); // (vertexNum-1) must always be less than or equal to numOfVertices // ERROR HANDLING 1
                }
            char vertexNumAsStr[3];
            sprintf(vertexNumAsStr, "%d", vertexNum); // convert vertexNum to a string and store in vertexNumAsStr
            // printf("vertexNumAsStr is %s.\n", vertexNumAsStr); // FOR DEBUGGING
            int hashVal = getDoubleHash(vertexTable, vertexTableSize, vertexNum); // returns the row of vertexTable where vertexName and vertexNum should be inserted
            strcpy(vertexTable[hashVal][0], vertexName); // hashVal is the hash-computed index returned by getDoubleHash()
            strcpy(vertexTable[hashVal][1], vertexNumAsStr);
            vertexNum++;
        }
    return vertexNum; 
}

int main(int argc, char* argv[]) // input file is shell directed to standard input
{   // OPEN OUTPUT FILE AND OBTAIN FIRST LINE OF SHELL DIRECTED INPUT FILE
    FILE* fOutPtr = fopen(OUTPUTFILENAME, "w"); 
    assert(fOutPtr != NULL); // open output file
    char* fileLine = (char*) malloc(MAXCHARSPERFILELINE * sizeof(char)); // holds an entire line of the input file so it can be strtok'd or atoi'd
    char* tempStr; // tempStr is used to hold tokens of fileLine so assignments can be made
    char* tempStr2; // tempStr2 is used to hold tokens of fileLine so assignments can be made
    strcpy(fileLine, "\0"); // initialize fileLine with strcpy(dest, src)
    // READ FIRST FILE LINE AND STORE numOfVertices AND numOfEdges
    size_t maxSizeOfFileLine = MAXCHARSPERFILELINE; 
    getline(&fileLine, &maxSizeOfFileLine, stdin); // getline(addrDestination, size, addrSource) returns charsReadFromLine as a char* // get the 1st file line from stdin
    int numOfVerticesFound = 1;
    // Note: strtok does not allocate memory so variables assigned to its return value should not be malloc'd
    tempStr = strtok(fileLine, " "); // get the 1st token from the fileLine
    tempStr2 = strtok(NULL, " "); // will continue to tokenize fileLine as first argument is NULL
    // printf("tempStr is %s. tempStr2 is %s.\n", tempStr, tempStr2); // FOR DEBUGGING
    int numOfVertices, numOfEdges; // the two integers on the first line is n, the # of vertices, and m, the # of edges
    if(tempStr2 == NULL) // ERROR HANDLING 2
        {   numOfVerticesFound = 0;
            numOfEdges = (int) atoi(tempStr); // numOfEdges is also how many lines are after the 1st fileline
            numOfVertices = numOfEdges + 1; // make numOfVertices equal to numOfEdges+1 by default
        }
    else{   numOfVertices = (int) atoi(tempStr); 
            numOfEdges = (int) atoi(tempStr2); // numOfEdges is also how many lines are after the 1st fileline
        }
    printf("numOfVertices is %d. numOfEdges is %d.\n", numOfVertices, numOfEdges); //FOR DEBUGGING
    // CREATE AND INITIALIZE DATA STRUCTURES TO HOLD FILE INFO
    int vertexTableSizeArr[60] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 
                                149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281}; // the 1st 60 prime numbers
    // int vertexTableSize = numOfVertices; // vertexTableSize needs to be the smallest prime number that can hold all numOfVertices vertices
    int vertexTableSize = 0; // initialize vertexTableSize
    int i; for(i=0; i<59; i++) // assign vertexTableSize to the smallest prime number that is greater than numOfVertices
        {   if(numOfVertices == vertexTableSizeArr[i]) // if numOfVertices = vertexTableSizeArr[i]
                {   printf(">>> vertexTableSize should be equal to %d.\n", vertexTableSizeArr[i]);
                    vertexTableSize = vertexTableSizeArr[i];
                    break;
                }
            else if(numOfVertices == vertexTableSizeArr[i+1] || (vertexTableSizeArr[i] < numOfVertices && numOfVertices < vertexTableSizeArr[i+1])) // if numOfVertices = vertexTableSizeArr[i+1] OR (vertexTableSizeArr[i] < numOfVertices < vertexTableSizeArr[i+1])
                {   printf(">>> vertexTableSize should be equal to %d.\n", vertexTableSizeArr[i+1]);
                    vertexTableSize = vertexTableSizeArr[i+1];
                    break;
                }
        }
    char*** vertexTable = (char***) malloc(vertexTableSize * sizeof(char**)); // dimensions are [numOfVertices][2], columns are vertex name & appearance in file as a numerical string
    char*** edgeTable = (char***) malloc(numOfEdges * sizeof(char**)); // dimensions are [numOfEdges][3], columns are start vertex, end vertex, edge weight as a numerical string
    int r, c; for(r=0; r<vertexTableSize; r++) // initialize vertexTable 
        {   vertexTable[r] = (char**) malloc(2 * sizeof(char*));
            for(c=0; c<2; c++)
                {   vertexTable[r][c] = (char*) malloc(MAXCHARSPERDATAVALUE * sizeof(char));
                    strcpy(vertexTable[r][c], INITIALIZEVALUE); // a string is assigned
                }
            // printf("vertexTable[%d][0] is %s. vertexTable[%d][1] is %s.\n", r, vertexTable[r][0], r, vertexTable[r][1]); // FOR DEBUGGING
        }
    for(r=0; r<numOfEdges; r++) // initialize edgeTable 
        {   edgeTable[r] = (char**) malloc(3 * sizeof(char*));
            for(c=0; c<3; c++)
                {   edgeTable[r][c] = (char*) malloc(MAXCHARSPERDATAVALUE * sizeof(char));
                    strcpy(edgeTable[r][c], INITIALIZEVALUE); // a string is assigned
                }
            // printf("edgeTable[%d][0] is %s. edgeTable[%d][1] is %s. edgeTable[%d][2] is %s.\n", r, edgeTable[r][0], r, edgeTable[r][1],  r, edgeTable[r][2]); // FOR DEBUGGING
        }
    // ETRACT THE CONTENTS OF THE INPUT FILE AND USE DOUBLE-HASHING TO ASSIGN VERTEX NAMES AN INDEX IN THE vertexTable
    int vertexNum = 1;
    for(i=0; i<numOfEdges; i++) // numOfEdges is how many lines after the initial fileLine are in the file
        {   getline(&fileLine, &maxSizeOfFileLine, stdin); // getline(addrDestination, size, addrSource) returns charsReadFromLine as a char* // get the 1st file line from stdin
            //printf("fileLine is %s.\n", fileLine); // FOR DEBUGGING
            // Note: strtok does not allocate memory so variables assigned to its return value should be malloc'd
            tempStr = strtok(fileLine, " ");  
            strcpy(edgeTable[i][0], tempStr); 
            vertexNum = insertInVertexTable(vertexTable, vertexTableSize, numOfVertices, tempStr, vertexNum); // check if fileLine is in vertexTable, if not add it using a hash or double hash
        //for(r=0; r<vertexTableSize; r++) // FOR DEBUGGING
        //    printf("vertexTable[%d][0] is %s. vertexTable[%d][1] is %s.\n", r, vertexTable[r][0], r, vertexTable[r][1]); // FOR DEBUGGING
            tempStr = strtok(NULL, " "); 
            strcpy(edgeTable[i][1], tempStr);
            vertexNum = insertInVertexTable(vertexTable, vertexTableSize, numOfVertices, tempStr, vertexNum); // check if fileLine is in vertexTable, if not add it using a hash or double hash
        //for(r=0; r<vertexTableSize; r++) // FOR DEBUGGING
        //    printf("vertexTable[%d][0] is %s. vertexTable[%d][1] is %s.\n", r, vertexTable[r][0], r, vertexTable[r][1]); // FOR DEBUGGING
            tempStr = strtok(NULL, "\n"); 
            strcpy(edgeTable[i][2], tempStr);
        }  // vertexNum at this point is really vertexNum-1
    // OUTPUT A WARNING TO USER IF numOfVertices DOES NOT MATCH NUMBER OF VERTEX NAMES FOUND 
    // Note: 2 other cases have already been handled at this point: 
        // 1.   (vertexNum-1) > numOfVertices (we end the program if this happens)  // ERROR HANDLING 1
        // 2.   numOfVertices isn't in file (we continue if this happens)   // ERROR HANDLING 2
    if((vertexNum-1) < numOfVertices && numOfVerticesFound == 1)
        {   printf("WARNING: Continuing with %d vertices!\n.", vertexNum-1);
            fprintf(fOutPtr, "WARNING: Continuing with %d vertices!\n.", vertexNum-1);
        }
    // SORT edgeTable IN ORDER OF INCREASING WEIGHT/COST VALUE 
    int insertIndex = 0; // index of edgeTable the smallest elemental value from indexes insertIndex to numOfEdges-1 will be inserted
    int smallestIndex = 0; // the smallest elemental value from indexes insertIndex to numOfEdges-1
    while(insertIndex <= numOfEdges-1)
        {   // use r (our scouter Index) to find the smallest elemental value from indexes insertIndex to numOfEdges-1
            smallestIndex = insertIndex;
            for(r=insertIndex; r<numOfEdges; r++) // search edgeTable for the smallest elemental value from indexes insertIndex to numOfEdges-1
                {   if(atoi(edgeTable[r][2]) < atoi(edgeTable[smallestIndex][2])) 
                        smallestIndex = r; // smaller elemental value is found
                }
            // now that the smallest elemental value is found from indexes insertIndex to numOfEdges-1, switch value at insertIndex with value at smallestIndex
            strcpy(tempStr, edgeTable[insertIndex][0]); // switch start vertex value
            strcpy(edgeTable[insertIndex][0], edgeTable[smallestIndex][0]);
            strcpy(edgeTable[smallestIndex][0], tempStr);
            strcpy(tempStr, edgeTable[insertIndex][1]); // switch end vertex value
            strcpy(edgeTable[insertIndex][1], edgeTable[smallestIndex][1]);
            strcpy(edgeTable[smallestIndex][1], tempStr);
            strcpy(tempStr, edgeTable[insertIndex][2]); // switch weight/cost value
            strcpy(edgeTable[insertIndex][2], edgeTable[smallestIndex][2]);
            strcpy(edgeTable[smallestIndex][2], tempStr);
            insertIndex++;
        }
    // USE KRUSHAL'S ALGORITHM TO FIND THE SHORTEST PATH THAT CONNECTS ALL VERTICES (THE LEAST COST/WEIGHT SOLUTION)     
    int minSpanningTree[vertexNum-2]; // (vertexNum-1) is how many vertices were found in the file, minSpanningTree has (vertexNum-2) edges in its solution
    for(r=0; r<(vertexNum-2); r++) // initialize minSpanningTree 
        {   minSpanningTree[r] = -1; // minSpanningTree is a 1D array of indexes to edgeTable that are part of the minSpanningTree solution
            // printf("minSpanningTree[%d] is %d.\n", r, minSpanningTree[r]); // FOR DEBUGGING
        }    
    krushalAlg(minSpanningTree, vertexNum, edgeTable, numOfEdges, tempStr); // finds the shortest path that connects all vertices (the least cost/weight solution) and fills the minSpanningTree with a solution (the indexes that point to a row of edgeTable)
    // PRINT RESULTS OUT
    int sumOfWeights = 0;
    for(r=0; r<(vertexNum-2); r++) // print minSpanningTree
        {   if(minSpanningTree[r] != -1)
                sumOfWeights += atoi(edgeTable[minSpanningTree[r]][2]); 
        }
    printf("Sum of weights of spanning edges is %d.\n", sumOfWeights); 
    fprintf(fOutPtr, "Sum of weights of spanning edges is %d.\n", sumOfWeights); 
    for(r=0; r<(vertexNum-2); r++) // print minSpanningTree
        {   if(minSpanningTree[r] != -1)
                {   printf("%s %s %s\n", edgeTable[minSpanningTree[r]][0], edgeTable[minSpanningTree[r]][1], edgeTable[minSpanningTree[r]][2]); 
                    fprintf(fOutPtr, "%s %s %s\n", edgeTable[minSpanningTree[r]][0], edgeTable[minSpanningTree[r]][1], edgeTable[minSpanningTree[r]][2]); 
                }
        }    
    int sizeOfLeaderArr = (vertexNum-2); // keeps track of the size of leaderArr so leaderArr can be deallocated // (vertexNum-2) is the size of minSpanningTree
    char** leaderArr = (char**) malloc(sizeOfLeaderArr * sizeof(char*)); // a dynamic array of tree leaders (each is a vertex) within the mST solution // (vertexNum-2) is the size of minSpanningTree 
    for(r=0; r<sizeOfLeaderArr; r++) // initialize leaderArr 
        {   leaderArr[r] = (char*) malloc(MAXCHARSPERDATAVALUE * sizeof(char));
            strcpy(leaderArr[r], INITIALIZEVALUE); // a string is assigned
        }
    int result = findNumOfLeaders(minSpanningTree, vertexNum, edgeTable, numOfEdges, leaderArr, sizeOfLeaderArr, tempStr);
    printf("There are %d leaders.\n", result);
    fprintf(fOutPtr, "There are %d leaders.\n", result);
    /*
    // PRINT DEBUGGING INFO
    for(r=0; r<vertexTableSize; r++) // FOR DEBUGGING
        printf("vertexTable[%d][0] is %s. vertexTable[%d][1] is %s.\n", r, vertexTable[r][0], r, vertexTable[r][1]); // FOR DEBUGGING
    for(r=0; r<numOfEdges; r++) // FOR DEBUGGING
        printf("edgeTable[%d][0] is %s. edgeTable[%d][1] is %s. edgeTable[%d][2] is %s.\n", r, edgeTable[r][0], r, edgeTable[r][1],  r, edgeTable[r][2]); // FOR DEBUGGING
    for(r=0; r<(vertexNum-2); r++)  // FOR DEBUGGING
        printf("minSpanningTree[%d] is %d.\n", r, minSpanningTree[r]); // FOR DEBUGGING
    for(r=0; r<sizeOfLeaderArr; r++) // search for tempStr in leaderArr
        printf("leaderArr[%d] is %s.\n", r, leaderArr[r]);
    */
    // FREE ALLOCATED MEMORY
    free(fileLine);
    for(r=vertexTableSize-1; r>=0; r--) 
        {   free(vertexTable[r]);
        }
    free(vertexTable);
    for(r=numOfEdges-1; r>=0; r--) 
        {   free(edgeTable[r]);
        }
    free(edgeTable);
    return 0;
}
