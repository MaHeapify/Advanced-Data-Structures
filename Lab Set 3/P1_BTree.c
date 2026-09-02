/*
    Implement a B-Tree of minimum degree 't' supporting insertion, search and deletion. 
    Demonstrate with sample keys.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure for the B-Tree node
typedef struct BTreeNode {
    int *keys;  // Array of keys
    struct BTreeNode **child;   // Array of child pointers  
    int n;  // Number of keys currently stored
    bool leaf;  // Is this node a leaf node?
} BTreeNode;

// Structure for the B-Tree
typedef struct {
    BTreeNode *root;
    int t;  // Minimum degree of the B-Tree
} BTree;

// Create the B-Tree node
BTreeNode *createNode(BTree *tree, bool leaf) {
    BTreeNode *node = malloc(sizeof(BTreeNode));

    if (node == NULL) {
        printf("\nMemory allocation failed.\n");
        exit(1);
    }

    node->leaf = leaf;
    node->n = 0;

    // Maximum keys = 2t - 1 
    // Maximum children = 2t
    node->keys = malloc((2 * tree->t - 1) * sizeof(int));
    node->child = malloc((2 * tree->t) * sizeof(BTreeNode *));

    if (node->keys == NULL || node->child == NULL) {
        free(node->keys);
        free(node->child);
        free(node);
        printf("\nMemory allocation failed.\n");
        exit(1);
    }

    for (int i = 0; i < 2 * tree->t; i++) {
        node->child[i] = NULL;
    }

    return node;
}

// Creates the B-Tree
BTree *createBTree(int t) {
    if (t < 2) {
        printf("\nMinimum degree must be at least 2.\n");
        return NULL;
    }

    BTree *tree = malloc(sizeof(BTree));

    if (tree == NULL) {
        printf("\nMemory allocation failed.\n");
        exit(1);
    }

    tree->t = t;
    tree->root = createNode(tree, true);

    return tree;
}

// Search for a key within the B-Tree
BTreeNode *search(BTreeNode *node, int key) {
    if (node == NULL) {
        return NULL;
    }

    int i = 0;

    // Find first key >= key
    while (i < node->n && key > node->keys[i]) {
        i++;
    }

    // Key found
    if (i < node->n && key == node->keys[i]) {
        return node;
    }

    // If leaf, key doesn't exist
    if (node->leaf) {
        return NULL;
    }

    // Otherwise search appropriate child
    return search(node->child[i], key);
}

// Traverse the B-Tree
void traverse(BTreeNode *node) {
    if (node == NULL) {
        return;
    }

    int i;

    for (i = 0; i < node->n; i++) {
        // Visit child before key
        if (!node->leaf) {
            traverse(node->child[i]);
        }

        printf("%d ", node->keys[i]);
    }

    // Visit last child
    if (!node->leaf) {
        traverse(node->child[i]);
    }
}

// Get height of the B-Tree
int height(BTreeNode *node) {
    if (node == NULL || node->leaf) {
        return 0;
    }

    return 1 + height(node->child[0]);
}

// Display nodes at a given level
void displayLevel(BTreeNode *node, int level) {
    if (node == NULL) {
        return;
    }

    if (level == 0) {
        printf("[");
        for (int i = 0; i < node->n; i++) {
            printf(" %d ", node->keys[i]);
        }
        printf("] ");
        return;
    }

    if (!node->leaf) {
        for (int i = 0; i <= node->n; i++) {
            displayLevel(node->child[i], level - 1);
        }
    }
}

// Display the B-Tree level by level
void levelOrder(BTreeNode *root) {
    int h = height(root);

    printf("\n");
    for (int level = 0; level <= h; level++) {
        printf("Level %d: ", level);
        displayLevel(root, level);
        printf("\n");
    }
}

// Split the child node of the B-Tree
void splitChild(BTree *tree, BTreeNode *parent, int index) {
    int t = tree->t;

    BTreeNode *fullChild = parent->child[index];

    // Create new node that receives the right half
    BTreeNode *newChild = createNode(tree, fullChild->leaf);

    // newChild gets t-1 keys from fullChild
    newChild->n = t - 1;

    for (int j = 0; j < t - 1; j++)
        newChild->keys[j] = fullChild->keys[j + t];

    // If fullChild is not a leaf, move its children as well
    if (!fullChild->leaf) {
        for (int j = 0; j < t; j++) {
            newChild->child[j] = fullChild->child[j + t];
        }
    }

    // Reduce number of keys in fullChild
    fullChild->n = t - 1;

    // Shift parent's children to the right to make space for new child
    for (int j = parent->n; j >= index + 1; j--) {
        parent->child[j + 1] = parent->child[j];
    }

    parent->child[index + 1] = newChild;

    // Move parent's keys to the right
    for (int j = parent->n - 1; j >= index; j--)
        parent->keys[j + 1] = parent->keys[j];

    // Middle key moves up to parent
    parent->keys[index] =
        fullChild->keys[t - 1];

    parent->n++;
}

// Insert a key into non full node of the B-tree
void insertNonFull(BTree *tree, BTreeNode *node, int key){
    int i = node->n - 1;

    // Case 1: node is a leaf
    if (node->leaf) {
        // Shift larger keys right
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        // Insert key
        node->keys[i + 1] = key;
        node->n++;

        return;
    }

    // Case 2: node is an internal node
    while (i >= 0 && key < node->keys[i]) {
        i--;
    }

    // Child where key should be inserted
    i++;

    // If child is full, split it first
    if (node->child[i]->n == 2 * tree->t - 1) {
        splitChild(tree, node, i);

        // After splitting, decide whether to go left or right
        if (key > node->keys[i]) {
            i++;
        }
    }

    insertNonFull(tree, node->child[i], key);
}

// Insert a key into the B-Tree
void insert(BTree *tree, int key) {
    if (search(tree->root, key) != NULL) {
        printf("Key %d already exists in the B-Tree.\n", key);
        return;
    }
    BTreeNode *root = tree->root;

    // Root is full
    if (root->n == 2 * tree->t - 1) {
        // Create a new root
        BTreeNode *newRoot = createNode(tree, false);

        tree->root = newRoot;

        // Old root becomes child 0
        newRoot->child[0] = root;

        // Split old root
        splitChild(tree, newRoot, 0);

        // Insert into appropriate child
        int i = 0;

        if (key > newRoot->keys[0]) {
            i++;
        }

        insertNonFull(tree, newRoot->child[i], key);
    } else {
        insertNonFull(tree, root, key);
    }
}

// Find key index in the B-Tree
int findKey(BTreeNode *node, int key) {
    int index = 0;

    while (index < node->n &&
           node->keys[index] < key) {
        index++;
    }

    return index;
}

// Get predecessor of a key
int getPredecessor(BTreeNode *node) {
    while (!node->leaf) {
        node = node->child[node->n];
    }

    return node->keys[node->n - 1];
}

// Get successor of a key
int getSuccessor(BTreeNode *node) {
    while (!node->leaf) {
        node = node->child[0];
    }

    return node->keys[0];
}

// Borrow from previous child
void borrowFromPrevious(BTree *tree, BTreeNode *parent, int index) {
    BTreeNode *child = parent->child[index];
    BTreeNode *sibling = parent->child[index - 1];

    // Shift child's keys right
    for (int i = child->n - 1; i >= 0; i--) {
        child->keys[i + 1] = child->keys[i];
    }

    // Move parent's separator down
    child->keys[0] = parent->keys[index - 1];

    // If child isn't leaf, shift children
    if (!child->leaf) {
        for (int i = child->n; i >= 0; i--) {
            child->child[i + 1] = child->child[i];
        }

        child->child[0] = sibling->child[sibling->n];
    }

    // Move sibling's last key up into parent
    parent->keys[index - 1] = sibling->keys[sibling->n - 1];

    child->n++;
    sibling->n--;
}

// Borrow from next child
void borrowFromNext(BTree *tree, BTreeNode *parent, int index) {
    BTreeNode *child = parent->child[index];
    BTreeNode *sibling = parent->child[index + 1];

    // Parent key goes into child
    child->keys[child->n] = parent->keys[index];

    // Move sibling's first child
    if (!child->leaf) {
        child->child[child->n + 1] = sibling->child[0];
    }

    // Sibling's first key moves up
    parent->keys[index] = sibling->keys[0];

    // Shift sibling keys left
    for (int i = 1; i < sibling->n; i++) {
        sibling->keys[i - 1] = sibling->keys[i];
    }

    // Shift sibling children left
    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->n; i++) {
            sibling->child[i - 1] = sibling->child[i];
        }
    }

    child->n++;
    sibling->n--;
}

// Merge children
void merge(BTree *tree, BTreeNode *parent, int index) {
    int t = tree->t;

    BTreeNode *child = parent->child[index];
    BTreeNode *sibling = parent->child[index + 1];

    // Move parent's separator into child
    child->keys[t - 1] = parent->keys[index];

    // Copy sibling's keys
    for (int i = 0; i < sibling->n; i++) {
        child->keys[i + t] = sibling->keys[i];
    }

    // Copy sibling's children
    if (!child->leaf) {
        for (int i = 0; i <= sibling->n; i++) {
            child->child[i + t] = sibling->child[i];
        }
    }

    // Update number of keys
    child->n += sibling->n + 1;

    // Remove separator from parent
    for (int i = index + 1; i < parent->n; i++) {
        parent->keys[i - 1] = parent->keys[i];
    }

    // Remove sibling pointer
    for (int i = index + 2; i <= parent->n; i++) {
        parent->child[i - 1] = parent->child[i];
    }

    parent->n--;

    // Sibling is no longer needed
    free(sibling->keys);
    free(sibling->child);
    free(sibling);
}

// Fill child
void fill(BTree *tree, BTreeNode *parent, int index) {
    int t = tree->t;

    // Borrow from left sibling
    if (index != 0 && parent->child[index - 1]->n >= t) {
        borrowFromPrevious(tree, parent, index);
    } else if (index != parent->n && parent->child[index + 1]->n >= t) { // Borrow from right sibling
        borrowFromNext(tree, parent, index);
    } else { // Otherwise merge
        if (index != parent->n) {
            merge(tree, parent, index);
        } else {
            merge(tree, parent, index - 1);
        }
    }
}

// Delete from node
void deleteFromNode(BTree *tree, BTreeNode *node, int key) {
    int t = tree->t;

    int index = findKey(node, key);

    // CASE 1: Key is present in this node
    if (index < node->n && node->keys[index] == key) {
        // CASE 1A: Node is leaf.
        if (node->leaf) {
            for (int i = index + 1; i < node->n; i++) {
                node->keys[i - 1] = node->keys[i];
            }

            node->n--;

            return;
        }

        // CASE 1B: Left child has >= t keys
        if (node->child[index]->n >= t) {
            int predecessor = getPredecessor(node->child[index]);

            node->keys[index] = predecessor;

            deleteFromNode(tree, node->child[index], predecessor);

            return;
        }

        // CASE 1C: Right child has >= t keys
        if (node->child[index + 1]->n >= t) {
            int successor = getSuccessor(node->child[index + 1]);

            node->keys[index] = successor;

            deleteFromNode(tree, node->child[index + 1], successor);

            return;
        }

        // CASE 1D: Both children have t-1 keys, merge them
        merge(tree, node, index);

        deleteFromNode(tree, node->child[index], key);

        return;
    }

    // CASE 2: Key is not present in this node
    if (node->leaf) {
        printf("Key %d not found.\n", key);
        return;
    }

    // Find child containing key
    bool lastChild = (index == node->n);

    // Before descending, make sure child has at least t keys
    if (node->child[index]->n < t) {
        fill(tree, node, index);
    }

    // If merge happened with previous child, index changes
    if (lastChild && index > node->n) {
        deleteFromNode(tree, node->child[index - 1], key);
    } else {
        deleteFromNode(tree, node->child[index], key);
    }
}

// Deletes the key from the B-Tree
void deleteKey(BTree *tree, int key) {
    if (tree == NULL || tree->root == NULL) {
        return;
    }

    // Check if key exists before deleting
    if (search(tree->root, key) == NULL) {
        printf("Key %d not found. Nothing to delete.\n", key);
        return;
    }

    deleteFromNode(tree, tree->root, key);

    // If root becomes empty, make its only child the new root
    if (tree->root->n == 0) {
        BTreeNode *oldRoot = tree->root;

        if (oldRoot->leaf) {
            printf("Key %d deleted from the B-Tree.\n", key);
            return;
        }

        tree->root = oldRoot->child[0];

        free(oldRoot->keys);
        free(oldRoot->child);
        free(oldRoot);
    }

    printf("Key %d deleted from the B-Tree.\n", key);
}

// Free up resources
void freeTree(BTreeNode *node) {
    if (node == NULL) {
        return;
    }

    if (!node->leaf) {
        for (int i = 0; i <= node->n; i++) {
            freeTree(node->child[i]);
        }
    }

    free(node->keys);
    free(node->child);
    free(node);
}

int main() {
    int t;

    printf("\nEnter the minimum degree t of the B-Tree: ");
    scanf("%d", &t);

    BTree *tree = createBTree(t);

    if (tree == NULL) {
        return 1;
    }

    int choice;
    int key;

    do {
        printf("\nEnter the operation you want to perform on the B-Tree:\n");
        printf("1. Insert a key\n2. Search for a key\n3. Delete a key\n4. Display B-Tree\n5. Level Order Display\n6. Exit\n");

        printf("\nChoose an operation to perform: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nEnter the key to insert into the B-Tree: ");
                scanf("%d", &key);

                insert(tree, key);
                break;

            case 2:
                printf("\nEnter the key to search into the B-Tree: ");
                scanf("%d", &key);

                if (search(tree->root, key) != NULL) {
                    printf("\nKey %d found in the B-Tree.\n", key);
                } else {
                    printf("\nKey %d not found in the B-Tree.\n", key);
                }

                break;

            case 3:
                printf("\nEnter the key to delete from the B-Tree: ");
                scanf("%d", &key);

                deleteKey(tree, key);
                break;

            case 4:
                printf("\nTraversal: ");

                traverse(tree->root);

                printf("\n");
                break;

            case 5:
                levelOrder(tree->root);
                break;

            case 6:
                freeTree(tree->root);
                free(tree);
                exit(0);

            default:
                printf("\nInvalid choice! Select a valid choice.\n");
                break;
        }
    } while (choice != 6);

    return 0;
}
