#ifndef ASTPRINT_H_INCLUDED
#define ASTPRINT_H_INCLUDED

#include "lang.h"
#include <stdio.h>
#include <stdlib.h>

void astprint_glob_item_list(struct glob_item_list* gs);

void astprint_glob_item(struct glob_item* g);

// each field is a (left_type, var_decl_expr) pair.
// entry_name is the name of the entry, e.g. Field, Argument, etc.
void astprint_type_list(struct type_list* fld, const char* entry_name);

// each element is a identifier
void astprint_enum_ele_list(struct enum_ele_list* ele);

void astprint_left_type(struct left_type* t);

// print as a ast tree (bottom up)
// * bar[10] -> ptr (array (`bar`))
void astprint_var_decl_expr(struct var_decl_expr* e);

// print as natural language (top down)
// * bar[10] -> `bar` is array[10] of pointer
void astprint_var_decl_expr_backtrace(struct var_decl_expr* e);

#endif // ASTPRINT_H_INCLUDED
