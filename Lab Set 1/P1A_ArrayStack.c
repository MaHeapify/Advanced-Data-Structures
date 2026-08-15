// Write a program to implement a stack using array.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Pushes an element onto the stack
void push(int array[], int size, int *top, int element) {
    if (*top == size - 1) {
        printf("\nStack overflow! Stack has reached it's limit.\n");
        return;
    }

    array[++(*top)] = element;
}

// Pushes random elements onto the stack based on limit specified
void pushRandom(int array[], int size, int *top) {
    int randomNumber;
    srand(time(NULL));

    while (*top < size - 1) {
        randomNumber = rand() % 100;
        push(array, size, top, randomNumber);
    }
}

// Pops an element from the stack
int pop(int array[], int *top) {
    int elementToPop;

    if (*top == -1) {
        return -1;
    }

    elementToPop = array[(*top)--];

    return elementToPop;
}

// Pops all the elements from the stack
void multipopWithoutCount(int *top) {
    *top = -1;
    printf("\nPopped all the elements from the stack.\n");
}

// Pops elements from the stack based on the count specified
void multipopWithCount(int array[], int *top, int noOfElementsToPop) {
    int i = 0;

    if (noOfElementsToPop == 0) {
        return;
    } else if (noOfElementsToPop > *top + 1) {
        multipopWithoutCount(top);
        return;
    }

    printf("\nElements popped from the stack: ");
    while (i < noOfElementsToPop) {
        printf("%d ", array[(*top)--]);
        i++;
    }
    printf("\n");
}

// Prints the elements in the stack
void printStack(int array[], int *top) {
    if (*top == -1) {
        printf("\nStack is empty. Nothing to print.\n");
        return;
    }

    printf("\nStack contents are as follows:\n");
    for (int i = *top; i >= 0; i--) {
        printf("%d\n", array[i]);
    }
}

int main() {
    int size;
    int top = -1;
    int choice;
    int element;
    int poppedElement;
    int noOfElementsToPop;

    printf("Choose the limit for the number of elements in the stack: ");
    scanf("%d", &size);

    if (size == 0) {
        printf("Size of the stack must greater than 0.\n");
        return 0;
    }

    int *array = malloc(size * sizeof(int));

    printf("\nThe stack operations are as follows:\n");
    printf("1. Push\n2. Push random elements based on limit specified\n3. Pop\n4. Multipop with specific count\n5. Multipop without count\n6. Print stack\n7. Exit\n");

    do {
        printf("\nChoose an operation to perform: ");
        scanf("%d", &choice);

        switch (choice) {
            // Perform PUSH operation
            case 1:
                printf("\nEnter the element you want to push onto the stack: ");
                scanf("%d", &element);

                push(array, size, &top, element);
                break;

            // Perform PUSH operation of random elements based on the limit specified
            case 2:
                pushRandom(array, size, &top);
                break;

            // Perform POP operation
            case 3:
                poppedElement = pop(array, &top);

                if (poppedElement == -1) {
                    printf("\nStack underflow! Stack is empty.\n");
                } else {
                    printf("\nElement popped from the stack: %d\n", poppedElement);
                }

                break;

            // Perform MULTIPOP operation with a specific count
            case 4:
                printf("\nEnter the count of elements to be popped: ");
                scanf("%d", &noOfElementsToPop);

                multipopWithCount(array, &top, noOfElementsToPop);
                break;

            // Perform MULTIPOP operation without a specific count
            case 5:
                multipopWithoutCount(&top);
                break;

            // Print the elements in the stack
            case 6:
                printStack(array, &top);
                break;

            // Exit the program
            case 7:
                exit(0);
                break;

            // Default case for invalid input
            default:
                printf("Invalid choice! Select a valid choice.");
        }
    } while (choice != 7);

    free(array);

    return 0;
}
