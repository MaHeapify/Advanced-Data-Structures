/*
    Write a C program to implement a dynamic table and calculate it's amortized cost using aggregate analysis
*/

#include <stdio.h>
#include <stdlib.h>

// Doubles the capacity of the dynamic table
void doubleCapacity(int **array, int *totalSize, int currentSize) {
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
}

// Display the current and total size of the dynamic table
void displaySize(int currentSize, int totalSize) {
    printf("Current size of the dynamic table is %d and total size is %d.\n", currentSize, totalSize);
}

void insertIntoDynamicTable(int element, int *totalSize, int *currentSize, int **array) {
    // Check if the dynamic table is full and double the capacity
    if (*currentSize == *totalSize) {
        displaySize(*currentSize, *totalSize);
        doubleCapacity(array, totalSize, *currentSize);
    }
    
    (*array)[*currentSize] = element;
    (*currentSize)++;
    printf("\nInserted %d into the dynamic table.\n", element);
}

// Prints the elements in the dynamic table
void printDynamicTable(int currentSize, int totalSize, int array[]) {
    if (currentSize == 0) {
        printf("\nDynamic table is empty! Nothing to print.\n");
        displaySize(currentSize, totalSize);
        return;
    }

    printf("\nDynamic table contents are: ");
    for (int i = 0; i < currentSize; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    displaySize(currentSize, totalSize);
}

int main() {
    int choice;
    int element;
    int totalSize = 1;
    int currentSize = 0;
    int *array = malloc(totalSize * sizeof(int));

    if (array == NULL) {
        printf("\nArray memory allocation failed!\n");
        return 1;
    }
    
    do {
        printf("\nEnter the operation you want to perform on the dynamic table:\n");
        printf("1. Insert an element\n2. Display the dynamic table\n3. Exit\n");
        printf("\nChoose an operation to perform: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nEnter the element you want to insert: ");
                scanf("%d", &element);

                insertIntoDynamicTable(element, &totalSize, &currentSize, &array);
                break;

            case 2:
                printDynamicTable(currentSize, totalSize, array);
                break;

            case 3:
                free(array);
                exit(0);

            default:
                printf("\nInvalid choice! Select a valid choice.\n");
                break;
        }
    } while (choice != 3);

    return 0;
}
