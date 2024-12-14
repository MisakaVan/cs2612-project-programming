#ifndef LIB_H_INCLUDED
#define LIB_H_INCLUDED

#include "lang.h"

#define NONE 4294967295

#define VERBOSE 1

#ifdef VERBOSE
    #define pdebug(fmt, ...) printf("[DEBUG] " fmt, ##__VA_ARGS__)
#else
    #define pdebug(fmt, ...)
#endif

extern int yylineno;

// clang-format off
unsigned int build_nat(char * c, int len);
char * new_str(char * str, int len);
struct SLL_hash_table;
struct SLL_hash_table * init_SLL_hash();
long long SLL_hash_get(struct SLL_hash_table * t, char * key);
void SLL_hash_set(struct SLL_hash_table * t, char * key, long long value);
void SLL_hash_delete(struct SLL_hash_table * t, char * key);
// clang-format on

// helper functions

// return the innermost core type. gaurenteed that ret->t == T_ORIG_TYPE.
struct var_decl_expr *get_core_type(struct var_decl_expr *ptr);


enum IdentifierType {
    IDENT_TYPE_VARIABLE = 0,
    IDENT_TYPE_ENUMERATOR,
    IDENT_TYPE_STRUCT,
    IDENT_TYPE_UNION,
    IDENT_TYPE_ENUM,
    IDENT_TYPE_TYPEDEF,
};

#define IDENT_TYPE_COUNT 6

struct IdentifierInfo {
    long long flags;
    int lineno[IDENT_TYPE_COUNT];
};

// #define IDENT_TYPE_VARIABLE   0
// #define IDENT_TYPE_ENUMERATOR 1
// #define IDENT_TYPE_STRUCT     2
// #define IDENT_TYPE_UNION      3
// #define IDENT_TYPE_ENUM       4
// #define IDENT_TYPE_TYPEDEF    5




struct IdentifierInfo *init_identifier_info();

void register_identifier_in_table(char* name, enum IdentifierType type, struct SLL_hash_table *table, int lineno);
void register_identifier(char *name, enum IdentifierType type);

void register_identifier_variable(char *name);
void register_identifier_enumerator(char *name);
void register_identifier_struct(char *name);
void register_identifier_union(char *name);
void register_identifier_enum(char *name);
void register_identifier_typedef(char *name);

// check if the identifier is already declared when using it
void check_identifier_in_table(char* name, enum IdentifierType type, struct SLL_hash_table *table);
void check_identifier(char *name, enum IdentifierType using_type);

void check_identifier_enum(char *name);
void check_identifier_struct(char *name);
void check_identifier_union(char *name);
void check_identifier_typedef(char *name);

// check if identifiers overlap within a field list
void check_field_list(struct type_list *field_list);

#endif
