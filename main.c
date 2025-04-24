#include <stdlib.h>
#include <stdio.h>

#include "lib/parseLib5.h"

typedef struct TreeNode {
    struct TreeNode* left;
    struct TreeNode* right;
    size_t id;
    long data;
} TreeNode;

enum nodeSide { LEFT, RIGHT };

TreeNode* createTreeNode(long data) {
    static size_t nextId = 0;
    TreeNode* node = malloc(sizeof(TreeNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->id = nextId++;
    return node;
}

int isTreeNodeLeaf(TreeNode* node) {
    return node->left == NULL && node->right == NULL;
}
int hasVacantChild(TreeNode* node) {
    return node->left == NULL || node->right == NULL;

}

void freeTree(TreeNode* tree) {
    if (tree->left) freeTree(tree->left);
    if (tree->right) freeTree(tree->right);
    free(tree);


}
void printTree(TreeNode* tree, int offset) {
    if (!tree) { printf("Tree is empty\n"); return; }

    if (tree->right) {
        printTree(tree->right, offset + 6);
    }
    printf("%*c%ld\n", offset, ' ', tree->data);
    if (tree->left) {
        printTree(tree->left, offset + 6);
    }
}

void printTreeWithAdditionId(TreeNode* tree, int offset) {
    if (!tree) { printf("Tree is empty\n"); return; }


    if (tree->right) {
        printTreeWithAdditionId(tree->right, offset + 6);
    }
    printf("%*c%ld", offset, ' ', tree->data);
    if (hasVacantChild(tree))printf(" (%zu)", tree->id);
    puts("");
    if (tree->left) {
        printTreeWithAdditionId(tree->left, offset + 6);
    }
}

void printTreeWithDelitionId(TreeNode* tree, int offset) {
    if (!tree) { printf("Tree is empty\n"); return; }

    if (tree->right) {
        printTreeWithDelitionId(tree->right, offset + 6);
    }
    printf("%*c%ld", offset, ' ', tree->data);
    printf(" (%zu)", tree->id);
    puts("");
    if (tree->left) {
        printTreeWithDelitionId(tree->left, offset + 6);
    }
}

TreeNode* findById(TreeNode* tree, size_t id) {
    TreeNode* ptr = NULL;
    if (tree->id == id) return tree;
    if (isTreeNodeLeaf(tree)) return NULL;
    if (tree->left && (ptr = findById(tree->left, id))) return ptr;
    if (tree->right && (ptr = findById(tree->right, id))) return ptr;
    return NULL;
}

TreeNode* findParentById(TreeNode* tree, size_t id) {
    TreeNode* ptr = NULL;
    if (tree->left) {
        if (tree->left->id == id) return tree;
        if ((ptr = findParentById(tree->left, id))) return ptr;
    }
    if (tree->right) {
        if (tree->right->id == id)  return  tree;
        if ((ptr = findParentById(tree->right, id))) return ptr;
    }
    return NULL;

}

int addById(TreeNode* tree, size_t id, int side, TreeNode* node) {
    if (!(tree = findById(tree, id))) {
        free(node);
        return 1;
    }
    if (side == RIGHT && !(tree->right)) {
        tree->right = node;
        return 0;
    }
    if (side == LEFT && !(tree->left)) {
        tree->left = node;
        return 0;
    }
    free(node);
    return 1;
}
int deleteById(TreeNode** treePtr, size_t id) {
    TreeNode* tree = *treePtr;
    if (tree->id == id) {
        *treePtr = NULL;
        freeTree(tree);
        return 0;
    }
    if (!(tree = findParentById(tree, id))) {
        return 1;
    }
    if (tree->left && tree->left->id == id) {
        freeTree(tree->left);
        tree->left = NULL;
        return 0;
    }
    if (tree->right && tree->right->id == id) {
        freeTree(tree->right);
        tree->right = NULL;
        return 0;
    }
    return 1;
}

int isNodeSide(long num) {
    return num == 0 || num == 1;
}

int isAcrionId(long num) {
    return num <= 4 && num >= 1;
}

int isNotNegative(long num) {
    return num >= 0;
}

void addNode(TreeNode** treeptr) {
    TreeNode* tree = *treeptr;
    long data = readLongWithDialog("Please enter data for the node (a natural number): ");
    printTreeWithAdditionId(tree, 0);
    if (!tree) { *treeptr = createTreeNode(data); return; }
    size_t id = (size_t)readLongWithDialog_v("Please enter an id of a node (number in brackets): ", isG0);
    int side = (int)readLongWithDialog_v("Please enter node side (0 for left and 1 for right): ", isNodeSide);

    if (addById(tree, id, side, createTreeNode(data))) printf("Cannot insert node in provided location.\n");

}

void deleteNode(TreeNode** treeptr) {
    printTreeWithDelitionId(*treeptr, 0);
    size_t id = (size_t)readLongWithDialog_v("Please enter an id of a node (number in brackets): ", isNotNegative);
    if (deleteById(treeptr, id)) printf("Could not find provided node\n");
}

size_t getNodeCount(TreeNode* tree) {
    size_t counter = 0;
    if (!tree) return 0;
    if (tree->left) counter += getNodeCount(tree->left);
    if (tree->right) counter += getNodeCount(tree->right);
    return counter + 1;

}

typedef struct __map_counters {
    long* map;
    size_t* counters;
} __map_counters;

void __addtocounter(long value, void* stu) {
    __map_counters* st = (__map_counters*)stu;
    if (value == 0) { st->counters[0]++; return; }
    size_t i = 1;
    long* map = st->map;
    for (; map[i] != value && map[i]; i++);
    if (!map[i]) map[i] = value;
    st->counters[i]++;
}
void triverceTree(TreeNode* tree, void (*func)(long, void*), void* data) {
    if (tree) func(tree->data, data);
    if (tree->left) triverceTree(tree->left, func, data);
    if (tree->right) triverceTree(tree->right, func, data);
}
size_t* getRepetitions(TreeNode* tree, long** mapPtr) {
    size_t nodeCount = getNodeCount(tree);
    long* map = calloc(nodeCount + 1, sizeof(long));
    size_t* counters = calloc(nodeCount + 1, sizeof(size_t));
    __map_counters stru;
    stru.counters = counters;
    stru.map = map;
    triverceTree(tree, __addtocounter, &stru);
    *mapPtr = map;
    return counters;
}
void getMaxRepetitions(TreeNode* tree) {
    size_t nodeCount = getNodeCount(tree);
    if(!nodeCount) {

        return;
    }
    long* map = NULL;
    size_t* counters = getRepetitions(tree, &map);

    size_t indexOfMax = 0;
    for (size_t i = 1; i < nodeCount && map[i]; i++) {
        if(counters[i] > counters[indexOfMax]) indexOfMax = i;
    }
    printf("Repetitions: %ld - %zu\n", map[indexOfMax], counters[indexOfMax]);
    free(map);
    free(counters);
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
            return 0;
            break;
        default:
            break;
        }

    }





    freeTree(tree);
    return 0;
}
