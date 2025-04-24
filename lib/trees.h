#include <stdlib.h>

typedef struct TreeNode {
    struct TreeNode* left;
    struct TreeNode* right;
    size_t id;
    long data;
} TreeNode;

enum nodeSide { LEFT, RIGHT };

TreeNode* createTreeNode(long data);

int isTreeNodeLeaf(TreeNode* node);
int hasVacantChild(TreeNode* node);

void freeTree(TreeNode* tree);
void printTree(TreeNode* tree, int offset);

void printTreeWithAdditionId(TreeNode* tree, int offset);

void printTreeWithDelitionId(TreeNode* tree, int offset);

TreeNode* findById(TreeNode* tree, size_t id);

TreeNode* findParentById(TreeNode* tree, size_t id);


int addById(TreeNode* tree, size_t id, int side, TreeNode* node);
int deleteById(TreeNode** treePtr, size_t id);


void addNode(TreeNode** treeptr);

void deleteNode(TreeNode** treeptr);

size_t getNodeCount(TreeNode* tree);

typedef struct __map_counters {
    long* map;
    size_t* counters;
} __map_counters;

void __addtocounter(long value, void* stu);
void triverceTree(TreeNode* tree, void (*func)(long, void*), void* data);
size_t* getRepetitions(TreeNode* tree, long** mapPtr);
void getMaxRepetitions(TreeNode* tree);