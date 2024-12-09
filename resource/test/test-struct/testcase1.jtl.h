// declare struct but not define it
struct SomeStruct;

// define a simple struct with two fields
struct Foo {
    int a;
    int b;
};

// define a struct with more complex fields
struct Bar {
    struct Foo foo;
    struct SomeStruct* ptr;

    // declare a function pointer
    void (*func)(int);
};

// define an empty struct
struct EmptyStruct {};