#include "astprint.h"
#include "lang.h"
#include <stdio.h>

#define MAX_INDENT_LEVEL 256
#ifndef INDENT_STRIDE
#define INDENT_STRIDE 2
#endif

// global variable `var_decl_expr_printer` is
// used to switch the printer function
static void (*var_decl_expr_printer)(struct var_decl_expr* e) =
    &astprint_var_decl_expr_backtrace;

static int       _indent = 0;
static const int _indent_stride = INDENT_STRIDE;
static char      _indent_char[MAX_INDENT_LEVEL]; // [0, _indent) are printed

static inline void inc_indent() {
    int old_indent = _indent;
    while (_indent < old_indent + _indent_stride) {
        _indent_char[_indent] = ' ';
        _indent++;
    }
}

static inline void dec_indent() { _indent -= _indent_stride; }

static inline void indent() {
    for (int i = 0; i < _indent; ++i) {
        putchar(_indent_char[i]);
    }
}

static inline void indent_mod(int delta) {
    for (int i = 0; i < _indent + delta; ++i) {
        putchar(_indent_char[i]);
    }
}

static inline void space(int n) {
    for (int i = 0; i < n; ++i) {
        putchar(' ');
    }
}

// Register the indent character at the given index.
// Index = -1 means the last character, -2 means the second last character, and
// so on.
static inline void register_indent_char_at(int index, char c) {
    if (index >= 0) {
        _indent_char[index] = c;
    } else {
        _indent_char[_indent + index] = c;
    }
}

// clang-format off
void astprint_glob_item_list(struct glob_item_list* gs) {
    while (gs != NULL) {
        printf("Global item:\n");
        inc_indent();
            astprint_glob_item(gs->data);
        dec_indent();
        gs = gs->next;
    }
}

void astprint_glob_item(struct glob_item* const g) {
    switch (g->t) {
    case T_STRUCT_DEF:
        indent(), printf("Type: struct definition\n");
        indent(), printf("Name: %s\n", g->d.STRUCT_DEF.name);
        indent(), printf("Fields:\n");
        inc_indent();
            astprint_type_list(g->d.STRUCT_DEF.fld, "Field");
        dec_indent();
        break;
    case T_STRUCT_DECL:
        indent(), printf("Type: struct declaration\n");
        indent(), printf("Name: %s\n", g->d.STRUCT_DECL.name);
        break;
    case T_UNION_DEF:
        indent(), printf("Type: union definition\n");
        indent(), printf("Name: %s\n", g->d.UNION_DEF.name);
        indent(), printf("Fields:\n");
        inc_indent();
            astprint_type_list(g->d.UNION_DEF.fld, "Field");
        dec_indent();
        break;
    case T_UNION_DECL:
        indent(), printf("Type: union declaration\n");
        indent(), printf("Name: %s\n", g->d.UNION_DECL.name);
        break;
    case T_ENUM_DEF:
        indent(), printf("Type: enum definition\n");
        indent(), printf("Name: %s\n", g->d.ENUM_DEF.name);
        indent(), printf("Elements:\n");
        inc_indent();
            astprint_enum_ele_list(g->d.ENUM_DEF.ele);
        dec_indent();
        break;
    case T_ENUM_DECL:
        indent(), printf("Type: enum declaration\n");
        indent(), printf("Name: %s\n", g->d.ENUM_DECL.name);
        break;
    case T_TYPE_DEF:
        indent(), printf("Type: typedef\n");
        inc_indent();
            indent(), printf("Left type:\n");
            inc_indent();
                astprint_left_type(g->d.TYPE_DEF.t);
            dec_indent();
            indent(), printf("Right type:\n");
            inc_indent();
                var_decl_expr_printer(g->d.TYPE_DEF.e);
            dec_indent();
        dec_indent();
        break;
    case T_VAR_DEF:
        indent(), printf("Type: var definition\n");
        inc_indent();
            indent(), printf("Left type:\n");
            inc_indent();
                astprint_left_type(g->d.VAR_DEF.t);
            dec_indent();
            indent(), printf("Right type:\n");
            inc_indent();
                var_decl_expr_printer(g->d.VAR_DEF.e);
        dec_indent();
        dec_indent();
        break;
    }
}

void astprint_type_list(struct type_list* fld, const char* entry_name) {
    if (entry_name == NULL)
        entry_name = "Type List Entry"; // can be set as Field, Argument, etc.

    register_indent_char_at(-_indent_stride, '|');

    int index = 0;
    while (fld != NULL) {
        indent_mod(-_indent_stride), putchar('+'), space(_indent_stride - 1),
            printf("%s %d:\n", entry_name, index++);
        inc_indent();
            indent(), printf("Left type:\n");
            inc_indent();
                astprint_left_type(fld->t);
            dec_indent();
            indent(), printf("Right type:\n");
            inc_indent();
                var_decl_expr_printer(fld->e);
            dec_indent();
        dec_indent();
        fld = fld->next;
    }
}

void astprint_enum_ele_list(struct enum_ele_list* ele) {
    while (ele != NULL) {
        indent(), printf("Element: %s\n", ele->name);
        ele = ele->next;
    }
}

void astprint_left_type(struct left_type* t) {
    switch (t->t) {
    case T_STRUCT_TYPE:
        indent(), printf("struct %s\n", t->d.STRUCT_TYPE.name);
        break;
    case T_NEW_STRUCT_TYPE:
        indent(), printf("new struct %s\n", t->d.NEW_STRUCT_TYPE.name);
        indent(), printf("Fields:\n");
        inc_indent();
            astprint_type_list(t->d.NEW_STRUCT_TYPE.fld, "Field");
        dec_indent();
        break;
    case T_UNION_TYPE:
        indent(), printf("union %s\n", t->d.UNION_TYPE.name);
        break;
    case T_NEW_UNION_TYPE:
        indent(), printf("new union %s\n", t->d.NEW_UNION_TYPE.name);
        indent(), printf("Fields:\n");
        inc_indent();
            astprint_type_list(t->d.NEW_UNION_TYPE.fld, "Field");
        dec_indent();
        break;
    case T_ENUM_TYPE:
        indent(), printf("enum %s\n", t->d.ENUM_TYPE.name);
        break;
    case T_NEW_ENUM_TYPE:
        indent(), printf("new enum %s\n", t->d.NEW_ENUM_TYPE.name);
        indent(), printf("Elements:\n");
        inc_indent();
            astprint_enum_ele_list(t->d.NEW_ENUM_TYPE.ele);
        dec_indent();
        break;
    case T_INT_TYPE:
        indent(), printf("int\n");
        break;
    case T_CHAR_TYPE:
        indent(), printf("char\n");
        break;
    case T_DEFINED_TYPE:
        indent(), printf("%s\n", t->d.DEFINED_TYPE.name);
        break;
    }
}

static void astprint_var_decl_expr_impl(struct var_decl_expr* e) {
    switch (e->t) {
    case T_ORIG_TYPE:
        indent();
        (e->d.ORIG_TYPE.name == NULL)
            ? printf("Original type: <empty>\n")
            : printf("Original type: %s\n", e->d.ORIG_TYPE.name);
        break;

    case T_PTR_TYPE:
        indent(), printf("Ptr:\n");
        inc_indent();
            astprint_var_decl_expr_impl(e->d.PTR_TYPE.base);
        dec_indent();
        break;

    case T_ARRAY_TYPE:
        indent(), printf("Arr[%u]:\n", e->d.ARRAY_TYPE.size);
        inc_indent();
            astprint_var_decl_expr_impl(e->d.ARRAY_TYPE.base);
        dec_indent();
        break;

    case T_FUNC_TYPE:
        indent(), printf("Function:\n");
        inc_indent();
            register_indent_char_at(-_indent_stride, '|');
            astprint_var_decl_expr_impl(e->d.FUNC_TYPE.ret);
        dec_indent();
        indent(), printf("Arguments:\n");
        inc_indent();
            astprint_type_list(e->d.FUNC_TYPE.args, "Arg");
        dec_indent();
        break;
    }
}

void astprint_var_decl_expr(struct var_decl_expr* e) {
    var_decl_expr_printer = &astprint_var_decl_expr;
    astprint_var_decl_expr_impl(e);
}

// first go all the way down to the base type.
// when returning, first print the message at this deeper level,
// then increment the indent level and return.
static void astprint_var_decl_expr_backtrace_impl(struct var_decl_expr* e) {
    switch (e->t) {
    case T_ORIG_TYPE:
        indent(), (e->d.ORIG_TYPE.name == NULL)
                      ? printf("Original type: <empty>\n")
                      : printf("Original type: %s\n", e->d.ORIG_TYPE.name);
        indent(), printf("is\n");
        inc_indent();
        break;
    case T_PTR_TYPE:
        astprint_var_decl_expr_backtrace_impl(e->d.PTR_TYPE.base);
        indent(), printf("pointer to\n");
        inc_indent();
        break;
    case T_ARRAY_TYPE:
        astprint_var_decl_expr_backtrace_impl(e->d.ARRAY_TYPE.base);
        indent(), printf("array[%u] of\n", e->d.ARRAY_TYPE.size);
        inc_indent();
        break;
    case T_FUNC_TYPE:
        astprint_var_decl_expr_backtrace_impl(e->d.FUNC_TYPE.ret);
        indent(), printf("function of arguments\n");
        inc_indent();
            astprint_type_list(e->d.FUNC_TYPE.args, "Arg");
        dec_indent();
        indent(), printf("returning\n");
        inc_indent();
        break;
    }
}

void astprint_var_decl_expr_backtrace(struct var_decl_expr* e) {
    var_decl_expr_printer = &astprint_var_decl_expr_backtrace;
    int old_indent = _indent;
    astprint_var_decl_expr_backtrace_impl(e);
    indent(), printf("the LHS type\n");
    _indent = old_indent;
}
