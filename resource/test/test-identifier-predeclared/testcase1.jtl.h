struct Foo;

struct Foo a; // OK: Foo is registered as struct at line 1

enum Foo b; // Warning: Identifier Foo has been registered as struct, not enum

union Foo c; // Warning: Identifier Foo has been registered as struct, not union


typedef Foo foo_t; // warning: should use `struct Foo` instead of `Foo`
foo_t d; // OK: foo_t has been registered as typedef
bar_t e; // Warning: Identifier bar_t has never been registered

