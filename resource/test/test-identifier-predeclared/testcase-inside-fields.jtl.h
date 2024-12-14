struct Foo {
    int var1;
    int var2;
    char var2; // Warning: redeclaration of 'var2' field
};

union Bar {
    char var1; // OK: 'var1' is not declared yet in this scope
    int (*var2)(int); // OK: 'var2' is not declared yet in this scope
    struct Foo var2; // Warning: redeclaration of 'var2' field
};
