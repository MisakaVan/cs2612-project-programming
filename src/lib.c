#include "lib.h"
#include "lang.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int build_nat(char* c, int len) {
    int s = 0, i = 0;
    for (i = 0; i < len; ++i) {
        if (s > 429496729) {
            printf(
                "We cannot handle natural numbers greater than 4294967295.\n");
            exit(0);
        }
        if (s == 429496729 && c[i] > '5') {
            printf(
                "We cannot handle natural numbers greater than 4294967295.\n");
            exit(0);
        }
        s = s * 10 + (c[i] - '0');
    }
    return s;
}

char* new_str(char* str, int len) {
    char* res = (char*)malloc(sizeof(char) * (len + 1));
    if (res == NULL) {
        printf("Failure in malloc.\n");
        exit(0);
    }
    strcpy(res, str);
    return res;
}

struct SLL_hash_cell {
    char*                 key;
    long long             value;
    struct SLL_hash_cell* tail;
};

struct SLL_hash_table {
    struct SLL_hash_cell*(h[2048]);
};

unsigned int hash_fun(char* str) {
    unsigned int s = 0;
    while (str[0] != 0) {
        s = (s * 307 + 97 + str[0]) & 2047;
        str++;
    }
    return s;
}

struct SLL_hash_table* init_SLL_hash() {
    struct SLL_hash_table* res =
        (struct SLL_hash_table*)malloc(sizeof(struct SLL_hash_table));
    if (res == NULL) {
        printf("Failure in malloc.\n");
        exit(0);
    }
    memset(res, 0, sizeof(struct SLL_hash_table));
    return res;
}

long long SLL_hash_get(struct SLL_hash_table* t, char* key) {
    unsigned int          s = hash_fun(key);
    struct SLL_hash_cell* p = t->h[s];
    while (p != NULL) {
        if (strcmp(key, p->key) == 0) {
            return p->value;
        }
        p = p->tail;
    }
    return NONE;
}

void SLL_hash_set(struct SLL_hash_table* t, char* key, long long value) {
    unsigned int           s = hash_fun(key);
    struct SLL_hash_cell** d = &(t->h[s]);
    while ((*d) != NULL) {
        if (strcmp(key, (*d)->key) == 0) {
            (*d)->value = value;
            return;
        }
        d = &((*d)->tail);
    }
    *d = (struct SLL_hash_cell*)malloc(sizeof(struct SLL_hash_cell));
    if (*d == NULL) {
        printf("Failure in malloc.\n");
        exit(0);
    }
    (*d)->key = new_str(key, strlen(key));
    (*d)->value = value;
    (*d)->tail = NULL;
    return;
}

void SLL_hash_delete(struct SLL_hash_table* t, char* key) {
    unsigned int           s = hash_fun(key);
    struct SLL_hash_cell** d = &(t->h[s]);
    while ((*d) != NULL) {
        if (strcmp(key, (*d)->key) == 0) {
            struct SLL_hash_cell* p = *d;
            *d = p->tail;
            free(p);
            return;
        }
        d = &((*d)->tail);
    }
}

struct SLL_hash_table* identifier_table;



struct var_decl_expr *get_core_type(struct var_decl_expr *ptr){
    while (ptr->t != T_ORIG_TYPE) {
        switch (ptr->t) {
            case T_PTR_TYPE:
                ptr = ptr->d.PTR_TYPE.base;
                break;
            case T_ARRAY_TYPE:
                ptr = ptr->d.ARRAY_TYPE.base;
                break;
            case T_FUNC_TYPE:
                ptr = ptr->d.FUNC_TYPE.ret;
                break;
            case T_ORIG_TYPE:
                break;
        }
    }
    return ptr;
}


struct IdentifierInfo* init_identifier_info() {
    struct IdentifierInfo* res =
        (struct IdentifierInfo*)malloc(sizeof(struct IdentifierInfo));
    if (res == NULL) {
        printf("Failure in malloc.\n");
        exit(0);
    }
    res->flags = 0;
    for (int i = 0; i < IDENT_TYPE_COUNT; i++) {
        res->lineno[i] = -1;
    }
    return res;
}


/*
struct, union, enum   names should not overlap with each other.

*/

int conflict_identifier_register_lut[6][6] = {
    {1, 1, 0, 0, 0, 1}, // variable
    {1, 1, 0, 0, 0, 1}, // enumerator
    {0, 0, 1, 1, 1, 0}, // struct
    {0, 0, 1, 1, 1, 0}, // union
    {0, 0, 1, 1, 1, 0}, // enum
    {1, 1, 0, 0, 0, 1}, // typedef
};

char* identifier_type_str[6] = {
    "variable",
    "enumerator",
    "struct",
    "union",
    "enum",
    "typedef",
};

// use yylineno to record the line number of the first registration
extern int yylineno;

// lineno: use -1 to fallback to yylineno.
void register_identifier_in_table(char* name, enum IdentifierType type, struct SLL_hash_table *table, int lineno, char *description) {
    if (lineno == -1){
        lineno = yylineno;
    }
    if (description == NULL){
        // fallback to the default description
        description = identifier_type_str[type];
    }

    pdebug("Line %d:\n", lineno);
    pdebug("Registering identifier %s as %s\n", name, description);

    if (name==NULL){
        pdebug("Identifier name is NULL (Anonymous), skip registering\n");
        return;
    }

    struct IdentifierInfo *info = NULL;

    info = (struct IdentifierInfo*)SLL_hash_get(table, name);
    if ((long long)info == NONE) {
        pdebug("Identifier %s is not in the hashtable\n", name);
        info = init_identifier_info();
        SLL_hash_set(table, name, (long long)info);
    }
    else{
        pdebug("Identifier %s is in the hashtable\n", name);
    }

    pdebug("Identifier %s flags: %lld\n", name, info->flags);

    int conflict = 0;
    // check if the identifier is already registered.
    for (int i = 0; i < IDENT_TYPE_COUNT; i++) {
        if (conflict_identifier_register_lut[type][i] == 0){
            continue;
        }
        if (info->flags & (1 << i)) {
            // identifier is already registered as a conflicting type.
            printf("Warning: (Line %d) Identifier %s is already registered as %s at line %d\n", lineno, name, description, info->lineno[i]);
            conflict = 1;
        }
    }

    if (!conflict) {
        pdebug("No conflict. Now register %s as %s\n", name, identifier_type_str[type]);
        info->flags |= (1 << type);
        info->lineno[type] = lineno; // the line number of the first registration
    }
}

void register_identifier(char* name, enum IdentifierType type) {
    register_identifier_in_table(name, type, identifier_table, -1, NULL);
}

void register_identifier_variable(char* name) {
    register_identifier(name, IDENT_TYPE_VARIABLE);
}

void register_identifier_enumerator(char* name) {
    register_identifier(name, IDENT_TYPE_ENUMERATOR);
}

void register_identifier_struct(char* name) {
    register_identifier(name, IDENT_TYPE_STRUCT);
}

void register_identifier_union(char* name) {
    register_identifier(name, IDENT_TYPE_UNION);
}

void register_identifier_enum(char* name) {
    register_identifier(name, IDENT_TYPE_ENUM);
}

void register_identifier_typedef(char* name) {
    register_identifier(name, IDENT_TYPE_TYPEDEF);
}

void check_identifier_in_table(char* name, enum IdentifierType using_type, struct SLL_hash_table *table){
    pdebug("Line %d:\n", yylineno);
    struct IdentifierInfo *info = NULL;
    info = (struct IdentifierInfo*)SLL_hash_get(table, name);
    if ((long long)info == NONE) {
        printf("Warning: (Line %d) Identifier %s has never been registered\n", yylineno, name);
        return;
    }
    pdebug("Identifier %s flags: %lld\n", name, info->flags);

    if (!(info->flags & (1 << using_type))) {
        printf("Warning: (Line %d) Identifier %s is not registered as %s\n", yylineno, name, identifier_type_str[using_type]);
        for (int i = 0; i < IDENT_TYPE_COUNT; i++) {
            if (info->flags & (1 << i)) {
                pdebug("  - Identifier %s is registered as %s at line %d\n", name, identifier_type_str[i], info->lineno[i]);
            }
        }
    }
    else {
        pdebug("OK: Identifier %s has been registered as %s at line %d\n", name, identifier_type_str[using_type], info->lineno[using_type]);
    }
}

void check_identifier(char *name, enum IdentifierType using_type){
    check_identifier_in_table(name, using_type, identifier_table);
}

void check_identifier_enum(char *name){
    check_identifier(name, IDENT_TYPE_ENUM);
}

void check_identifier_struct(char *name){
    check_identifier(name, IDENT_TYPE_STRUCT);
}

void check_identifier_union(char *name){
    check_identifier(name, IDENT_TYPE_UNION);
}

void check_identifier_typedef(char *name){
    check_identifier(name, IDENT_TYPE_TYPEDEF);
}

void check_field_list(struct type_list *field_list){
    // build a new hash table for the field list
    struct SLL_hash_table *field_table = init_SLL_hash();
    while (field_list != NULL){
        struct left_type *t = field_list->t;
        struct var_decl_expr *e = field_list->e;
        struct var_decl_expr *core_type = get_core_type(e);
        register_identifier_in_table(
            core_type->d.ORIG_TYPE.name, 
            IDENT_TYPE_VARIABLE,
            field_table,
            core_type->d.ORIG_TYPE.lineno,
            "field variable"
        );
        field_list = field_list->next;
    }
}