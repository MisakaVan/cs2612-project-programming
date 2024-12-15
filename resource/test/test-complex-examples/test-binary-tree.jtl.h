// A simple binary tree implementation header file.

typedef char u64[8];

typedef u64 *value_t;

typedef struct BinaryTreeNode_ {
    value_t value;
    struct BinaryTreeNode_ *left;
    struct BinaryTreeNode_ *right;
} BinaryTreeNode;

typedef struct BinaryTree_ {
    BinaryTreeNode *root;
} BinaryTree;

BinaryTree* BinaryTree_init();
int BinaryTree_insert(BinaryTree *, value_t);
value_t BinaryTree_find(BinaryTree *, value_t);