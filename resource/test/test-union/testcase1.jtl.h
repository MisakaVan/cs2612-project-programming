// declare union but not define it
union SomeUnion;

// define a simple union with two fields
union Foo {
    int a;
    int b;
};

// define a union with more complex fields
union Bar {
    union Foo foo;
    union SomeUnion* ptrToUnion;
    struct SomeStruct* ptrToStruct;

    // declare a function pointer
    void (*func)(int);
};

// define an empty union
union EmptyUnion {};