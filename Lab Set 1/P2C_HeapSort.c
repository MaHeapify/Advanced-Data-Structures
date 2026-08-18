// Write a program to implement heap sort (using max heap).

#include <stdio.h>
#include <time.h>
#include <windows.h>

// Build the max heap using the array elements
void heapify(int data[], int size, int i) {
    int largest = i;
    int leftChild = 2 * i + 1;
    int rightChild = 2 * i + 2;

    if (leftChild < size && data[leftChild] > data[largest]) {
        largest = leftChild;
    }

    if (rightChild < size && data[rightChild] > data[largest]) {
        largest = rightChild;
    }

    if (largest != i) {
        int temp = data[i];
        data[i] = data[largest];
        data[largest] = temp;

        heapify(data, size, largest);
    }
}

// Perform heap sort algorithm
void heapSort(int data[], int size) {
    // Build max heap
    for (int i = size/2 - 1; i >= 0; i--) {
        heapify(data, size, i);
    }

    // Extract elements one by one
    for (int i = size - 1; i > 0; i--) {
        // Move max element to the end
        int temp = data[0];
        data[0] = data[i];
        data[i] = temp;

        // Restore max heap
        heapify(data, i, 0);
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
    LARGE_INTEGER start, end, freq;
    double timeTaken;

    QueryPerformanceFrequency(&freq);

    // Seed for random number generation
    srand(time(NULL));

    printf("\nEnter the number of elements in the array: ");
    scanf("%d", &size);

    if (size <= 0) {
        printf("Array size must be greater than 0.\n");
        return 1;
    }

    int data[size];

    printf("\nWould you like to randomly populate elements in the array? [Press 'Y' or 'y' for Yes, otherwise treated as No]: ");
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

    printf("\nThe elements in the array before applying heap sort are as follows: ");
    display(data, size);

    QueryPerformanceCounter(&start);

    // Apply heap sort algorithm
    heapSort(data, size);
    
    QueryPerformanceCounter(&end);

    printf("\nThe elements in the array after applying heap sort are as follows: ");
    display(data, size);
    
    timeTaken = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;

    printf("\nAlgorithm Execution Stats:\n");
    printf("\nHeap Sort algorithm took %.9f seconds to execute.\n", timeTaken);

    return 0;
}
