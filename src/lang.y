%{
	// this part is copied to the beginning of the parser 
	#include <stdio.h>
	#include "lang.h"
	#include "lexer.h"
	void yyerror(const char *);
	int yylex(void);
  struct glob_item_list * root;

  #define YYERROR_VERBOSE
%}

  /* define the yylval union */
%union {
  unsigned int             natVal;
  char *                   strVal;
  struct left_type *       leftType;
  struct var_decl_expr *   varDeclExpr;
  struct type_list *       typeList;
  struct enum_ele_list *   enumEleList;
  struct glob_item *       globalItem;
  struct glob_item_list *  globItemList;
  void *                   none;
}

// Terminals
%token <natVal> TM_NAT                        // natural number
%token <strVal> TM_IDENT                      // identifier
%token <none>   TM_LEFT_BRACE TM_RIGHT_BRACE  // { }
%token <none>   TM_LEFT_BRACKET TM_RIGHT_BRACKET  // [ ]
%token <none>   TM_LEFT_PAREN TM_RIGHT_PAREN  // ( )
%token <none>   TM_STAR TM_COMMA              // *, ,
%token <none>   TM_SEMICOL                    // ;

// New Terminals
%token <none>   TM_ENUM TM_STRUCT TM_UNION    // enum, struct, union
%token <none>   TM_TYPEDEF                    // typedef
%token <none>   TM_INT TM_CHAR                // int, char

// Nonterminals

%type <globItemList> NT_WHOLE // top level nonterminal is a list of global declarations
%type <globItemList> NT_GLOB_ITEM_LIST
%type <globalItem>   NT_GLOBAL_ITEM

  // global items are ended with a semicolon. the body is:
  // 1. definition and declaration of struct, union and enum
%type <globalItem>  NT_STRUCT_DEFINITION
%type <globalItem>  NT_STRUCT_DECLARATION
%type <globalItem>  NT_UNION_DEFINITION
%type <globalItem>  NT_UNION_DECLARATION
%type <globalItem>  NT_ENUM_DEFINITION
%type <globalItem>  NT_ENUM_DECLARATION
  // 2. type definition with typedef
%type <globalItem>  NT_TYPE_DEFINITION
  // 3. variable declaration
%type <globalItem>  NT_VAR_DECLARATION

  // in 1, field list is a list of field declarations seperated by space.
  // stored as type_list. Note that `field` is not defined as a symbol.
  // field ::= left_type named_right_type_expr
%type <typeList>     NT_FIELD_LIST
  // in 1, enum element list is a list of enum elements seperated by comma.
  // stored as enum_ele_list. Note that `enum_ele` is not defined as a symbol.
%type <enumEleList>  NT_ENUM_ELE_LIST

  // in 2 and 3, left type
%type <leftType>     NT_LEFT_TYPE

  // in 1, 2 and 3, named right type expr is an
  // expression about an identifier or about empty.
  // named_right_type_expr has a core identifier.
  // annon_right_type_expr has no core identifier / empty in the core.
%type <varDeclExpr>  NT_NAMED_RIGHT_TYPE_EXPR
%type <varDeclExpr>  NT_ANNON_RIGHT_TYPE_EXPR_NON_EMPTY
%type <varDeclExpr>  NT_ANNON_RIGHT_TYPE_EXPR

  // argument list is a list of arguments seperated by comma.
  // argument ::= left_type annon_right_type_expr
%type <typeList>     NT_ARGUMENT_LIST
%type <typeList>     NT_ARGUMENT_LIST_NON_EMPTY



// Associativity and Precedence

/* 1. * int () [] : () and [] have higher precedence than * */

// %nonassoc TM_ASGNOP                        // = has no associativity
// %left TM_OR TM_AND TM_NOT                  // ||, &&, ! are left associative
// %left TM_LT TM_LE TM_GT TM_GE TM_EQ TM_NE  // <, <=, >, >=, ==, != are left associative
// %left TM_PLUS TM_MINUS                     // +, - are left associative
// %left TM_MUL TM_DIV TM_MOD                 // *, /, % are left associative


%right TM_SEMICOL                           // ; is right associative
%right TM_COMMA                             // , is right associative
%right TM_STAR                              // * is right associative
%left TM_LEFT_PAREN TM_RIGHT_PAREN TM_LEFT_BRACKET TM_RIGHT_BRACKET 
                                            // () and [] are left associative





%%


          /****************************
          |* Top Level Nonterminals  *|
          ****************************/    


NT_WHOLE:
  NT_GLOB_ITEM_LIST
  {
    $$ = ($1);
    root = $$; // set the global variable root to the parsed result
  }
;

NT_GLOB_ITEM_LIST:
  NT_GLOBAL_ITEM
  {
    /* A single global item. This should be at the end of the source file. */
    /* make a list with a single item */
    $$ = TGCons($1, TGNil());
  }
| NT_GLOBAL_ITEM NT_GLOB_ITEM_LIST
  {
    /* A global item (contains a semicolon in its tail) followed by a list of global items */
    $$ = TGCons($1, $2);
  }
;


          /****************************
          |* 8 types of global items *|
          ****************************/


NT_GLOBAL_ITEM:
  NT_STRUCT_DEFINITION  { $$ = $1; }
| NT_STRUCT_DECLARATION { $$ = $1; }
| NT_UNION_DEFINITION   { $$ = $1; }
| NT_UNION_DECLARATION  { $$ = $1; }
| NT_ENUM_DEFINITION    { $$ = $1; }
| NT_ENUM_DECLARATION   { $$ = $1; }
| NT_TYPE_DEFINITION    { $$ = $1; }
| NT_VAR_DECLARATION    { $$ = $1; }
;

NT_STRUCT_DEFINITION:
  TM_STRUCT TM_IDENT TM_LEFT_BRACE NT_FIELD_LIST TM_RIGHT_BRACE TM_SEMICOL
  {
    // struct glob_item* TStructDef(char* name, struct type_list* fld)
    $$ = TStructDef($2, $4);
  }
;

NT_STRUCT_DECLARATION:
  TM_STRUCT TM_IDENT TM_SEMICOL
  {
    // struct glob_item* TStructDecl(char* name)
    $$ = TStructDecl($2);
  }
;

NT_UNION_DEFINITION:
  TM_UNION TM_IDENT TM_LEFT_BRACE NT_FIELD_LIST TM_RIGHT_BRACE TM_SEMICOL
  {
    // struct glob_item* TUnionDef(char* name, struct type_list* fld)
    $$ = TUnionDef($2, $4);
  }
;

NT_UNION_DECLARATION:
  TM_UNION TM_IDENT TM_SEMICOL
  {
    // struct glob_item* TUnionDecl(char* name)
    $$ = TUnionDecl($2);
  }
;

NT_ENUM_DEFINITION:
  TM_ENUM TM_IDENT TM_LEFT_BRACE NT_ENUM_ELE_LIST TM_RIGHT_BRACE TM_SEMICOL
  {
    // struct glob_item* TEnumDef(char* name, struct enum_ele_list* ele)
    $$ = TEnumDef($2, $4);
  }
;

NT_ENUM_DECLARATION:
  TM_ENUM TM_IDENT TM_SEMICOL
  {
    // struct glob_item* TEnumDecl(char* name)
    $$ = TEnumDecl($2);
  }
;

NT_TYPE_DEFINITION:
  TM_TYPEDEF NT_LEFT_TYPE NT_NAMED_RIGHT_TYPE_EXPR TM_SEMICOL
  {
    // struct glob_item* TTypeDef(struct left_type* t, struct var_decl_expr* e)
    $$ = TTypeDef($2, $3);
  }
;

NT_VAR_DECLARATION:
  NT_LEFT_TYPE NT_NAMED_RIGHT_TYPE_EXPR TM_SEMICOL
  {
    // struct glob_item* TVarDef(struct left_type* t, struct var_decl_expr* e)
    $$ = TVarDef($1, $2);
  }
;



/* Field List and Enum Element List */
NT_FIELD_LIST:
  /* empty field list is allowed */
  /* empty */
  {
    // struct type_list* TTNil()
    $$ = TTNil();
  }
| NT_LEFT_TYPE NT_NAMED_RIGHT_TYPE_EXPR TM_SEMICOL NT_FIELD_LIST
  {
    // struct type_list* TTCons(struct left_type* t, struct var_decl_expr* e,
    //                   struct type_list* next)
    $$ = TTCons($1, $2, $4);
  }
;

NT_ENUM_ELE_LIST:
  /* empty is not allowed */
  TM_IDENT
  {
    // struct enum_ele_list* TECons(char* name, struct enum_ele_list* next)
    $$ = TECons($1, TENil());
  }
| TM_IDENT TM_COMMA NT_ENUM_ELE_LIST
  {
    // struct enum_ele_list* TECons(char* name, struct enum_ele_list* next)
    $$ = TECons($1, $3);
  }
;

/* Left Type */
NT_LEFT_TYPE:

  /* struct */

  TM_STRUCT TM_IDENT TM_LEFT_BRACE NT_FIELD_LIST TM_RIGHT_BRACE
  {
    // struct left_type* TNewStructType(char* name, struct type_list* fld)
    $$ = TNewStructType($2, $4);
  }
| TM_STRUCT TM_LEFT_BRACE NT_FIELD_LIST TM_RIGHT_BRACE
  { /* anonymous struct */
    // struct left_type* TNewStructType(char* name, struct type_list* fld)
    $$ = TNewStructType(NULL, $3);
  }
| TM_STRUCT TM_IDENT
  {
    // struct left_type* TStructType(char* name)
    $$ = TStructType($2);
  }

  /* union */

| TM_UNION TM_IDENT TM_LEFT_BRACE NT_FIELD_LIST TM_RIGHT_BRACE
  {
    // struct left_type* TNewUnionType(char* name, struct type_list* fld)
    $$ = TNewUnionType($2, $4);
  }
| TM_UNION TM_LEFT_BRACE NT_FIELD_LIST TM_RIGHT_BRACE
  { /* anonymous union */
    // struct left_type* TNewUnionType(char* name, struct type_list* fld)
    $$ = TNewUnionType(NULL, $3);
  }
| TM_UNION TM_IDENT
  {
    // struct left_type* TUnionType(char* name)
    $$ = TUnionType($2);
  }

  /* enum */

| TM_ENUM TM_IDENT TM_LEFT_BRACE NT_ENUM_ELE_LIST TM_RIGHT_BRACE
  {
    // struct left_type* TNewEnumType(char* name, struct enum_ele_list* ele)
    $$ = TNewEnumType($2, $4);
  }
| TM_ENUM TM_LEFT_BRACE NT_ENUM_ELE_LIST TM_RIGHT_BRACE
  { /* anonymous enum */
    // struct left_type* TNewEnumType(char* name, struct enum_ele_list* ele)
    $$ = TNewEnumType(NULL, $3);
  }
| TM_ENUM TM_IDENT
  {
    // struct left_type* TEnumType(char* name)
    $$ = TEnumType($2);
  }

  /* basic types */
| TM_INT
  {
    // struct left_type* TIntType()
    $$ = TIntType();
  }
| TM_CHAR
  {
    // struct left_type* TCharType()
    $$ = TCharType();
  }

  /* typedef alias */
| TM_IDENT
  {
    // struct left_type* TDefinedType(char* name)
    $$ = TDefinedType($1);
  }
;

/* Named Right Type Expr, Annon Right Type Expr, Argument List */

NT_NAMED_RIGHT_TYPE_EXPR:
  TM_IDENT
  {
    // struct var_decl_expr* TOrigType(char* name)
    $$ = TOrigType($1);
  }
| TM_STAR NT_NAMED_RIGHT_TYPE_EXPR
  {
    // struct var_decl_expr* TPtrType(struct var_decl_expr* base)
    $$ = TPtrType($2);
  }
| NT_NAMED_RIGHT_TYPE_EXPR TM_LEFT_BRACKET TM_NAT TM_RIGHT_BRACKET
  {
    // struct var_decl_expr* TArrayType(struct var_decl_expr* base,
    //                                  unsigned int          size)
    $$ = TArrayType($1, $3);
  }
| NT_NAMED_RIGHT_TYPE_EXPR TM_LEFT_PAREN NT_ARGUMENT_LIST TM_RIGHT_PAREN
  {
    // struct var_decl_expr* TFuncType(struct var_decl_expr* base,
    //                                  struct type_list*      args)
    $$ = TFuncType($1, $3);
  }
| TM_LEFT_PAREN NT_NAMED_RIGHT_TYPE_EXPR TM_RIGHT_PAREN
  {
    /* support for parentheses */
    $$ = $2;
  }
;

NT_ANNON_RIGHT_TYPE_EXPR_NON_EMPTY:
  TM_STAR
  {
    // * <empty>
    $$ = TPtrType(TOrigType(NULL));
  }
| TM_STAR NT_ANNON_RIGHT_TYPE_EXPR_NON_EMPTY
  {
    // * <expr>
    $$ = TPtrType($2);
  }
| TM_LEFT_BRACKET TM_NAT TM_RIGHT_BRACKET
  {
    // <empty> [nat]
    $$ = TArrayType(TOrigType(NULL), $2);
  }
| NT_ANNON_RIGHT_TYPE_EXPR_NON_EMPTY TM_LEFT_BRACKET TM_NAT TM_RIGHT_BRACKET
  {
    // <expr> [nat]
    $$ = TArrayType($1, $3);
  }
| TM_LEFT_PAREN NT_ARGUMENT_LIST TM_RIGHT_PAREN
  {
    // <empty> (args)
    $$ = TFuncType(TOrigType(NULL), $2);
  }
| NT_ANNON_RIGHT_TYPE_EXPR_NON_EMPTY TM_LEFT_PAREN NT_ARGUMENT_LIST TM_RIGHT_PAREN
  {
    // <expr> (args)
    $$ = TFuncType($1, $3);
  }
| TM_LEFT_PAREN NT_ANNON_RIGHT_TYPE_EXPR_NON_EMPTY TM_RIGHT_PAREN
  {
    /* support for parentheses */
    $$ = $2;
  }
;

NT_ANNON_RIGHT_TYPE_EXPR:
  NT_ANNON_RIGHT_TYPE_EXPR_NON_EMPTY
  {
    $$ = $1;
  }
| /* empty */
  {
    $$ = TOrigType(NULL);
  }
;

NT_ARGUMENT_LIST:
  /* empty is allowed */
  /* empty */
  {
    $$ = TTNil();
  }
| NT_ARGUMENT_LIST_NON_EMPTY
  {
    $$ = $1;
  }

NT_ARGUMENT_LIST_NON_EMPTY:
  NT_LEFT_TYPE NT_ANNON_RIGHT_TYPE_EXPR
  {
    $$ = TTCons($1, $2, TTNil());
  }
| NT_LEFT_TYPE NT_ANNON_RIGHT_TYPE_EXPR TM_COMMA NT_ARGUMENT_LIST_NON_EMPTY
  {
    $$ = TTCons($1, $2, $4);
  }
;

%%


void yyerror(const char *s) 
{
  fprintf(stderr, "%s\n", s);
}
