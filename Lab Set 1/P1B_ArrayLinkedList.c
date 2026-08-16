// Write a program to implement a stack using linked list.

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Custom datatype to hold the linked list node data
struct node {
    int data;
    struct node *next;
};

// Pushes an element onto the stack
void push(struct node **top, int *size, int element) {
    struct node *p;
    
    // Creates a new node with the given element
    p = (struct node *)malloc(sizeof(struct node));

    // Check if node creation was successful
    if (p == NULL) {
        printf("Stack overflow! Node creation failed due to insufficient memory.");
        return;
    }

    p->data = element;
    p->next = *top;
    *top = p;

    (*size)++;
}

// Pops an element from the stack
int pop(struct node **top, int *size) {
    int poppedElement;
    struct node *p;
    p = *top;

    if (*top == NULL) {
        return INT_MIN;
    }
    
    poppedElement = p->data;
    *top = p->next;

    // Free the dynamic memory
    free(p);

    (*size)--;

    return poppedElement;
}

// Pops all the elements from the stack
void multipopWithoutCount(struct node **top, int *size) {
    struct node *p;
    p = *top;

    if (*top == NULL) {
        printf("\nStack underflow! Stack is empty.\n");
        return;
    }
    
    while (p != NULL) {
        *top = p->next;

        free(p);
        p = *top;
    }

    (*size) = 0;

    printf("\nPopped all the elements from the stack.\n");
}

// Pops elements from the stack based on the count specified
void multipopWithCount(struct node **top, int *size, int noOfElementsToPop) {
    int i = 0;

    if (noOfElementsToPop == 0) {
        printf("\nNo count specified. Nothing to pop.\n");
        return;
    } else if (*top == NULL) {
        printf("\nStack underflow! Stack is empty.\n");
        return;
    } else if (noOfElementsToPop > *size) {
        multipopWithoutCount(top, size);
        return;
    }

    struct node *p;
    
    printf("\nElements popped from the stack: ");
    while (i < noOfElementsToPop) {
        p = *top;
        printf("%d ", p->data);

        *top = p->next;

        free(p);

        (*size)--;
        i++;
    }
    printf("\n");
}

// Prints the top of the stack
void peek(struct node **top) {
    if (*top == NULL) {
        printf("\nStack is empty. Nothing to print.\n");
        return;
    }

    printf("\nTop of the stack: %d\n", (*top) -> data);
}

// Prints the elements in the stack
void printStack(struct node **top) {
    struct node *p;
    p = *top;

    if (*top == NULL) {
        printf("\nStack is empty. Nothing to print.\n");
        return;
    }

    printf("\nStack contents are as follows:\n");
    while (p != NULL) {
        printf("%d\n", p->data);
        p = p->next;
    }
}

int main() {
    int size = 0;
    int choice;
    int element;
    int poppedElement;
    int noOfElementsToPop;
    struct node *top = NULL;

    do {
        printf("\nThe stack operations are as follows:\n");
        printf("1. Push\n2. Pop\n3. Multipop with specific count\n4. Multipop without count\n5. Peek\n6. Print stack\n7. Exit\n");
        printf("\nChoose an operation to perform: ");
        scanf("%d", &choice);

        switch (choice) {
            // Perform PUSH operation
            case 1:
                printf("\nEnter the element you want to push onto the stack: ");
                scanf("%d", &element);

                push(&top, &size, element);
                break;

            // Perform POP operation
            case 2:
                poppedElement = pop(&top, &size);

                if (poppedElement == INT_MIN) {
                    printf("\nStack underflow! Stack is empty.\n");
                } else {
                    printf("\nElement popped from the stack: %d\n", poppedElement);
                }

                break;

            // Perform MULTIPOP operation with a specific count
            case 3:
                printf("\nEnter the count of elements to be popped: ");
                scanf("%d", &noOfElementsToPop);

                multipopWithCount(&top, &size, noOfElementsToPop);
                break;

            // Perform MULTIPOP operation without a specific count
            case 4:
                multipopWithoutCount(&top, &size);
                break;

            // Perform PEEK operation
            case 5:
                peek(&top);
                break;

            // Print the elements in the stack
            case 6:
                printStack(&top);
                break;

            // Exit the program
            case 7:
                exit(0);
                break;

            // Default case for invalid input
            default:
                printf("\nInvalid choice! Select a valid choice.\n");
                break;
        }
    } while (choice != 7);

    return 0;
}
