#include <stdlib.h>
#include <stdio.h>
#include "lib/parseLib5.h"
#include "lib/trees.h"

int isAcrionId(long num) {
    return num <= 4 && num >= 1;
}
int main(int argc, char const* argv[]) {
    TreeNode* tree = createTreeNode(0);
    tree->left = createTreeNode(1);
    tree->right = createTreeNode(2);
    tree->right->right = createTreeNode(3);
    tree->right->left = createTreeNode(4);
    tree->left->right = createTreeNode(55);
    while (1) {
        printTree(tree, 0);
        printf("What action would you like to perform?\n");
        printf("  1. Add node\n");
        printf("  2. Delete node with children\n");
        printf("  3. Find max count of repetitions\n");
        printf("  4. Exit programm\n");
        switch (readLongWithDialog_v("> ", isAcrionId)) {
        case 1:
            addNode(&tree);
            break;
        case 2:
            deleteNode(&tree);
            break;
        case 3:
            getMaxRepetitions(tree);
            break;
        case 4:
            freeTree(tree);
            return 0;
            break;
        default:
            break;
        }

    }
    return 0;
}
