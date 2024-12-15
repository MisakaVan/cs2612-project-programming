// declare a struct Foo and then define it
struct Foo;
struct Foo; // OK in C; OK here
struct Foo {
    int a;
    int b;
}; // OK in C; OK here
struct Foo; // OK in C; OK here

// define it again. should fail in C. OK here as we don't check duplicate definition
struct Foo {
    int a;
    int b;
}; 
struct Foo; // OK in C; OK here

enum Bar;
enum Bar {a}; // OK in C; OK here
enum Bar;     // OK in C; OK here
