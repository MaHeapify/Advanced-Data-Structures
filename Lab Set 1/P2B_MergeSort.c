// Write a program to implement merge sort.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Custom datatype to hold the linked list node data
struct node {
    int data;
    struct node *next;
};

// Inserts an element in the linked list
void insert(struct node **s, int element) {
    struct node *p;

    // Creates a new node with the given element
    p = (struct node*)malloc(sizeof(struct node));

    // Check if node creation was successful
    if (p == NULL) {
        printf("\nNode creation failed due to insufficient memory.\n");
        return;
    }

    p->data = element;
    p->next = NULL;

    if (*s == NULL) {
        *s = p;
    } else {
        struct node *temp = *s;

        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = p;
    }
}

// Prints the elements in the linked list
void display(struct node *s) {
    while (s != NULL) {
        printf("%d ", s->data);
        s = s->next;
    }

    printf("\n");
}

// Returns the middle node address from amongst all the nodes in the linked list
struct node* middleNodeAddress(struct node *s) {
    struct node *p, *q;
    p = q = s;

    // Uses fast pointer method to determine the middle node address of the linked list
    while (q != NULL && q->next != NULL && q->next->next != NULL) {
        p = p->next;
        q = q->next->next;
    }

    return p;
}

// Merge algorithm to combine 2 sorted linked lists
struct node* merge(struct node *s1, struct node *s2) {
    // Create a dummy node to track the final resultant linked list
    struct node dummy;
    struct node *p = &dummy;

    dummy.next = NULL;

    // Compare the elements from the corresponding linked lists
    while (s1 != NULL && s2 != NULL) {
        if (s1->data <= s2->data) {
            p->next = s1;
            s1 = s1->next;
        } else {
            p -> next = s2;
            s2 = s2->next;
        }

        p = p->next;
    }

    // Attach the remaining nodes of the linked list
    if (s1 != NULL) {
        p->next = s1;
    } else {
        p->next = s2;
    }

    // Return the head of the new linked list
    return dummy.next;
}

// Perform merge sort algorithm
struct node* mergeSort(struct node *s) {
    struct node *p, *q, *s1, *s2;

    // If the head is pointing to NULL or there is only one element then return head
    if (s == NULL || s->next == NULL) {
        return s;
    }

    // Divide the linked list into two parts
    p = middleNodeAddress(s);
    q = p->next;
    p->next = NULL;

    // Recursively split and sort the individual linked lists
    s1 = mergeSort(s);
    s2 = mergeSort(q);
    
    // Apply merge algorithm in the end
    s = merge(s1, s2);

    return s;
}

int main() {
    int element;
    int size;
    char response;
    int i = 0;
    struct node *s = NULL;
    struct node *p = NULL;

    // Seed for random number generation
    srand(time(NULL));

    printf("\nEnter the number of elements you would like to insert in the linked list: ");
    scanf("%d", &size);

    printf("\nWould you like to randomly populate elements in the linked list? [Press 'Y' or 'y' for Yes, otherwise treated as No]: ");
    scanf(" %c", &response);

    while (i < size) {
        if (response == 'Y' || response == 'y') {
            int randomNumber = rand() % 100;
            insert(&s, randomNumber);
        } else {
            printf("\nEnter the element %d you want to insert into the linked list: ", i + 1);
            scanf("%d", &element);

            insert(&s, element);
        }
        i++;
    }

    printf("\nThe elements in the linked list before applying merge sort are as follows: ");
    display(s);

    // Apply merge sort algorithm
    p = mergeSort(s);

    printf("\nThe elements in the linked list after applying merge sort are as follows: ");
    display(p);

    return 0;
}
