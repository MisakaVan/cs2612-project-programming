// A simple linked list implementation header file.

typedef char u64[8];

typedef u64 *value_t;

typedef struct LinkedListNode_ {
    value_t value;
    struct LinkedListNode_* next;
} LinkedListNode;

typedef struct LinkedList_ {
    LinkedListNode* head;
} LinkedList;

LinkedList* LinkedList_init();
int LinkedList_push(LinkedList *, value_t);
value_t LinkedList_pop(LinkedList *);

