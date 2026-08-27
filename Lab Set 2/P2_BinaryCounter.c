/*
    Write a C program to implement a binary counter using an array of bits.
*/

#include <stdio.h>
#include <stdlib.h>

#define NUM_BITS 6
#define NUM_OPS 16

typedef struct {
    int bits[NUM_BITS];       // bits[0] is the LSB
    int credit[NUM_BITS];     // Credit stored on each bit
} BinaryCounter;

// Initializes the binary counter
void initCounter(BinaryCounter *bc) {
    for (int i = 0; i < NUM_BITS; i++) {
        bc->bits[i] = 0;
        bc->credit[i] = 0;
    }
}

// Prints the counter in binary form (MSB -> LSB)
void printBinary(BinaryCounter *bc) {
    for (int i = NUM_BITS - 1; i >= 0; i--) {
        printf("%d", bc->bits[i]);
    }
}

// Increments the binary counter
void increment(BinaryCounter *bc, int *actual_cost, int *amortized_cost, int *flips_0_to_1, int *flips_1_to_0) {
    int i = 0;

    *actual_cost = 0;
    *amortized_cost = 0;
    *flips_0_to_1 = 0;
    *flips_1_to_0 = 0;

    // Flip trailing 1s to 0s
    while (i < NUM_BITS && bc->bits[i] == 1) {
        bc->bits[i] = 0;
        bc->credit[i] = 0;

        (*flips_1_to_0)++;
        (*actual_cost)++;

        i++;
    }

    // Flip the first 0 to 1
    if (i < NUM_BITS) {
        bc->bits[i] = 1;
        bc->credit[i] = 1;

        (*flips_0_to_1)++;
        (*actual_cost)++;

        // 1 unit for the current flip + 1 unit of stored credit
        *amortized_cost = 2;
    }
    else {
        // Counter overflow
        *amortized_cost = 0;
    }
}

// Calculate total credit currently stored
int getTotalCredit(BinaryCounter *bc) {
    int sum = 0;

    for (int i = 0; i < NUM_BITS; i++) {
        sum += bc->credit[i];
    }

    return sum;
}

int main() {
    BinaryCounter bc;

    // Variables for the amortized analysis table
    int total_actual_cost = 0;
    int total_amortized_cost = 0;

    initCounter(&bc);

    printf("\n");
    printf("========================================================================================\n");
    printf("                    ACCOUNTING METHOD                                    \n");
    printf("========================================================================================\n");

    printf("%-8s %-12s %-12s %-12s %-13s %-15s %-12s\n", "Operation", "Counter", "1 -> 0", "0 -> 1", "Actual Cost", "Amortized Cost", "Credit");
    printf("----------------------------------------------------------------------------------------\n");

    // Initial state
    printf("%-8s %-12s %-12s %-12s %-13s %-15s %-12d\n", "Init", "000000", "-", "-", "-", "-", getTotalCredit(&bc));

    for (int op = 1; op <= NUM_OPS; op++) {
        int actual_cost = 0;
        int amortized_cost = 0;
        int flips_0_to_1 = 0;
        int flips_1_to_0 = 0;

        increment(&bc, &actual_cost, &amortized_cost, &flips_0_to_1, &flips_1_to_0);

        total_actual_cost += actual_cost;
        total_amortized_cost += amortized_cost;

        int bank_credit = getTotalCredit(&bc);

        printf("%-8d ", op);
        printBinary(&bc);
        printf("       %-12d %-12d %-13d %-15d %-12d\n", flips_1_to_0, flips_0_to_1, actual_cost, amortized_cost, bank_credit);
    }

    printf("\n");
    printf("========================================================================================\n");
    printf("                       MATHEMATICAL ANALYSIS                             \n");
    printf("========================================================================================\n");

    printf("Total Operations (n)             : %d\n", NUM_OPS);
    printf("Total Actual Cost (Sum c_i)      : %d\n", total_actual_cost);
    printf("Total Amortized Cost (Sum c^_i)  : %d\n", total_amortized_cost);
    printf("Remaining Credit in Bank         : %d\n", getTotalCredit(&bc));

    printf("\n");
    printf("------------------------ THEORETICAL VERIFICATION --------------------------------------\n");

    printf("\n1. Validity Condition:\n");
    printf("   Sum(c^_i) >= Sum(c_i) for all n.\n");
    printf("   %d >= %d\n", total_amortized_cost, total_actual_cost);

    printf("\n2. Credit Condition:\n");
    printf("   The credit stored in the bank is always non-negative.\n");
    printf("   Remaining Credit = %d >= 0\n", getTotalCredit(&bc));

    printf("\n3. Amortized Cost Per Operation:\n");
    printf("   - At most one 0 -> 1 flip occurs during an increment.\n");
    printf("   - Each 0 -> 1 flip is charged 2 units.\n");
    printf("   - 1 unit pays for the current flip.\n");
    printf("   - 1 unit is stored as credit for a future 1 -> 0 flip.\n");
    printf("   - Any number of 1 -> 0 flips are paid using stored credits.\n");

    printf("\n4. Complexity:\n");
    printf("   Amortized cost per increment = O(1)\n");
    printf("   Therefore, n increment operations take O(n) amortized time.\n");

    return 0;
}
