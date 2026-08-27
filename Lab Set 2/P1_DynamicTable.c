/*
    Write a C program to implement a dynamic table and calculate it's amortized cost using aggregate analysis.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Doubles the capacity of the dynamic table and return the number of elements copied
int doubleCapacity(int **array, int *totalSize, int currentSize) {
    printf("\nNo space in dynamic table! Doubling the current size.\n");

    int newSize = 2 * (*totalSize);
    int *newArray = malloc(newSize * sizeof(int));

    if (newArray == NULL) {
        printf("\nMemory allocation failed!\n");
        exit(1);
    }

    // Copy existing elements into new array
    for (int i = 0; i < currentSize; i++) {
        newArray[i] = (*array)[i];
    }

    // Free old array
    free(*array);

    // Point array to new array
    *array = newArray;

    // Update capacity
    *totalSize = newSize;

    printf("New total size of the dynamic table is %d.\n", *totalSize);

    return currentSize;
}

// Display the current and total size of the dynamic table
void displaySize(int currentSize, int totalSize) {
    printf("\nCurrent size of the dynamic table is %d and total size is %d.\n", currentSize, totalSize);
}

// Insert element into the dynamic table
void insertIntoDynamicTable(int element, int *totalSize, int *currentSize, int **array, int **analysisCost, int **analysisTotalSize, int *analysisSize) {
    int copies = 0;
    int *tempCost;
    int *tempTotalSize;
    
    // Check if the dynamic table is full and double the capacity
    if (*currentSize == *totalSize) {
        displaySize(*currentSize, *totalSize);
        copies = doubleCapacity(array, totalSize, *currentSize);
    }
    
    (*array)[*currentSize] = element;
    (*currentSize)++;

    // Increase the number of rows in the analysis table
    (*analysisSize)++;

    // Resize the analysis table
    tempCost = realloc(*analysisCost, (*analysisSize) * sizeof(int));

    if (tempCost == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    *analysisCost = tempCost;

    tempTotalSize = realloc(*analysisTotalSize, (*analysisSize) * sizeof(int));

    if (tempTotalSize == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    *analysisTotalSize = tempTotalSize;

    // Store cost and capacity for this insertion
    (*analysisCost)[*analysisSize - 1] = copies + 1;
    (*analysisTotalSize)[*analysisSize - 1] = *totalSize;

    printf("\nInserted %d into the dynamic table.\n", element);
}

// Prints the elements in the dynamic table
void printDynamicTable(int currentSize, int totalSize, int array[]) {
    if (currentSize == 0) {
        printf("\nDynamic table is empty! Nothing to print.\n");
        displaySize(currentSize, totalSize);
        return;
    }

    printf("\nDynamic table contents are:\n");
    for (int i = 0; i < currentSize; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    displaySize(currentSize, totalSize);
}

// Prints the amortized analysis
void displayAmortizedAnalysis(int analysisSize, int analysisCost[], int analysisTotalSize[]) {
    int totalCost = 0;

    printf("\n====================================================\n");
    printf("             AGGREGATE ANALYSIS\n");
    printf("====================================================\n");

    printf("\nOperation\tCurrent Size\tTotal Size\tCost\n");
    printf("----------------------------------------------------\n");

    for (int i = 0; i < analysisSize; i++) {
        printf("Insert %d\t%d\t\t%d\t\t%d\n", i + 1, i + 1, analysisTotalSize[i], analysisCost[i]);
        totalCost += analysisCost[i];
    }

    printf("----------------------------------------------------\n");

    printf("Total Cost (T(n))\t\t\t%d\n", totalCost);

    if (analysisSize > 0) {
        printf("Amortized Cost (T(n)/n)\t\t\t%.3f\n", (double)totalCost / analysisSize);
    }

    printf("\nTherefore:\n");
    printf("Total cost = O(n)\n");
    printf("Amortized cost per insertion = O(1)\n");

    printf("====================================================\n");
}

int main() {
    int choice;
    int element;
    int totalSize = 1;
    int currentSize = 0;
    int numberOfRandomElements;
    int *array = malloc(totalSize * sizeof(int));

    // Variables for the amortized analysis table
    int analysisSize = 0; // Number of rows currently stored
    int *analysisCost = NULL; // Stores cost of each insertion
    int *analysisTotalSize = NULL; // Stores capacity after each insertion

    srand(time(NULL));

    if (array == NULL) {
        printf("\nArray memory allocation failed!\n");
        return 1;
    }
    
    do {
        printf("\nEnter the operation you want to perform on the dynamic table:\n");
        printf("1. Insert an element\n2. Insert random elements\n3. Display the dynamic table\n4. Display amortized analysis\n5. Exit\n");
        printf("\nChoose an operation to perform: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nEnter the element you want to insert: ");
                scanf("%d", &element);

                insertIntoDynamicTable(element, &totalSize, &currentSize, &array, &analysisCost, &analysisTotalSize, &analysisSize);
                break;

            case 2:
                printf("\nEnter the number of random elements you want to insert: ");
                scanf("%d", &numberOfRandomElements);

                if (numberOfRandomElements < 0) {
                    printf("\nPlease enter a non-negative number.\n");
                    continue;
                }

                for (int i = 0; i < numberOfRandomElements; i++) {
                    element = rand() % 1000;

                    insertIntoDynamicTable(element, &totalSize, &currentSize, &array, &analysisCost, &analysisTotalSize, &analysisSize);
                }

                break;

            case 3:
                printDynamicTable(currentSize, totalSize, array);
                break;

            case 4:
                if (analysisSize == 0) {
                    printf("\nNo operations to analyse yet!\n");
                    break;
                }

                displayAmortizedAnalysis(analysisSize, analysisCost, analysisTotalSize);
                break;
            
            case 5:
                free(array);
                free(analysisCost);
                free(analysisTotalSize);
                exit(0);

            default:
                printf("\nInvalid choice! Select a valid choice.\n");
                break;
        }
    } while (choice != 5);

    return 0;
}
