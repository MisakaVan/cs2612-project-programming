struct foo {};
struct foo foo; // OK

enum foo {a}; // Warning: Identifier foo has been registered as struct, not enum
int foo;  // Warning : Identifier foo is already registered as variable at line 2