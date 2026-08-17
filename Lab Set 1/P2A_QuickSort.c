// Write a program to implement quick sort.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Swap two numbers
void swap(int data[], int i, int j) {
    int temp = data[i];
    data[i] = data[j];
    data[j] = temp;
}

// Perform partition algorithm
int partition(int data[], int i, int j) {
    int x = data[i];
    int y = i;

    for (int k = i + 1; k <= j; k++) {
        if (data[k] <= x) {
            y++;
            swap(data, y , k);
        }
    }

    swap(data, y, i);

    return y;
}

// Perform quick sort algorithm
void quickSort(int data[], int i, int j) {
    int m;

    while (i <= j) {
        m = partition(data, i, j);
        
        if (m - i > j - m) {
            quickSort(data, m + 1, j);
            j = m - 1;
        } else {
            quickSort(data, i, m - 1);
            i = m + 1;
        }
    }
}

// Print the elements in the array
void display(int data[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", data[i]);
    }

    printf("\n");
}

int main() {
    int size;
    char response;

    // Seed for random number generation
    srand(time(NULL));
    
    printf("\nEnter the number of elements in the array: ");
    scanf("%d", &size);

    int data[size];
    
    printf("\nWould you like to randomly populate elements in the linked list? [Press 'Y' or 'y' for Yes, otherwise treated as No]: ");
    scanf(" %c", &response);

    for (int i = 0; i < size; i++) {
        if (response == 'Y' || response == 'y') {
            int randomNumber = rand() % 100;
            data[i] = randomNumber;
            continue;
        }

        printf("\nEnter the element %d of the array: ", i + 1);
        scanf("%d", &data[i]);
    }

    printf("\nThe elements in the array before applying quick sort are as follows: ");
    display(data, size);

    // Apply quick sort algorithm
    quickSort(data, 0, size - 1);

    printf("\nThe elements in the array after applying quick sort are as follows: ");
    display(data, size);

    return 0;
}
