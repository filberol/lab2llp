%define parse.error verbose

%{
    #include <stdio.h>
    #include <math.h>
    #include <stdbool.h>
    #include "../include/ast.h"
    extern void yyerror (char const *);
    extern int yylex(void);

    #ifdef YYDEBUG
      yydebug = 1;
    #endif
%}

%union {
    struct AstNode* node;
    double d_val;
    int i_val;
    char* s_val;
    bool b_val;
}

%token T_FOR "FOR"
%token T_FILTER "FILTER"
%token T_RETURN "RETURN"

%token T_REMOVE "REMOVE"
%token T_INSERT "INSERT"
%token T_UPDATE "UPDATE"
%token T_REPLACE "REPLACE"
%token T_IN "IN"
%token T_QUOTED_STRING "q-string"

%token T_NULL "null"
%token T_TRUE "true"
%token T_FALSE "false"
%token T_STRING "string"
%token T_INTEGER "int"
%token T_DOUBLE "double"

%token T_EQ "=="
%token T_NE "!="
%token T_LT "<"
%token T_GT ">"
%token T_LE "<="
%token T_GE ">="

%token T_COMMA ","
%token T_OPEN "("
%token T_CLOSE ")"
%token T_OBJECT_OPEN "{"
%token T_OBJECT_CLOSE "}"
%token T_ARRAY_OPEN "["
%token T_ARRAY_CLOSE "]"
%token T_COLON ":"
%token T_END 0 "end"
%token T_NOT "not operator"
%token T_AND "and operator"
%token T_OR "or operator"

/* define operator precedence */
%left T_OR
%left T_AND
%left T_COMMA
%left T_EQ T_NE
%left T_LT T_GT T_LE T_GE
%left T_PLUS T_MINUS
%left T_TIMES T_DIV T_MOD
%right UMINUS UPLUS UNEGATION
%left REFERENCE
%right T_COLON

/* define token return types */
%type <s_val> T_QUOTED_STRING
%type <s_val> T_STRING
%type <node> T_INTEGER
%type <node> T_DOUBLE
%type <node> expression;
%type <node> reference;
%type <node> value_literal;
%type <s_val> variable_name;
%type <node> numeric_value;
%type <node> for_output_variable;
%type <node> in_collection;
%type <node> in_collection_name;
%type <node> operator_binary;
%type <node> object;
%type <node> optional_object_elements;
%type <node> object_elements_list;
%type <node> object_element;
%type <s_val> object_element_name;
%start queryStart

%%
queryStart: query {};

query: optional_statement_block_statements final_statement {};

final_statement:
    return_statement {}
  | remove_statement {}
  | insert_statement {}
  | update_statement {}
  ;

optional_statement_block_statements:
    /* empty */ {
    }
  | optional_statement_block_statements statement_block_statement {
    }
  ;

statement_block_statement:
    for_statement {}
  | filter_statement {}
  | remove_statement {}
  | insert_statement {}
  | update_statement {}
  ;

for_output_variable:
    variable_name {
          struct AstNode* node = create_string_node($1);
          $$ = node;
        }
    ;

for_statement:
    T_FOR for_output_variable T_IN expression {
        struct AstNode* variableNameNode = (struct AstNode*)($2);
        struct AstNode* variableNode = create_variable_node(variableNameNode->value._string);
        add_variable(variableNameNode->value._string, variableNode);
        add_operation(get_current_scope(), create_for_node(variableNode, $4));
    }
  ;

filter_statement:
    T_FILTER expression {
      struct AstNode* filterNode = create_filter_node($2);
      add_operation(get_current_scope(), filterNode);
    }
  ;

return_statement:
    T_RETURN expression {
      struct AstNode* node = create_return_node($2);
      add_operation(get_current_scope(), node);
    }
  ;

in_collection:
    T_IN in_collection_name {
      $$ = $2;
    }
   ;

remove_statement:
    T_REMOVE expression in_collection  {
      struct AstNode* node = create_remove_node($2, $3);
      add_operation(get_current_scope(), node);
    }
  ;

insert_statement:
    T_INSERT expression in_collection {
      struct AstNode* node = create_insert_node($2, $3);
      add_operation(get_current_scope(), node);
    }
  ;

update_parameters:
    expression in_collection {
      struct AstNode* node = create_update_node($1, $2);
      add_operation(get_current_scope(), node);
    }
  ;

update_statement:
    T_UPDATE update_parameters {  
    }
  ;


object:
    T_OBJECT_OPEN {
      struct AstNode* node = create_node_object();
      push_common(node);
    } optional_object_elements T_OBJECT_CLOSE {
      $$ = (struct AstNode*)(pop_common());
    }
  ;

optional_object_elements:
    /* empty */ {
    }
  | object_elements_list {
    }
  | object_elements_list T_COMMA {
    }
  ;

object_elements_list:
    object_element {
    }
  | object_elements_list T_COMMA object_element {
    }
  ;

object_element:
    T_STRING {
      struct AstNode* variable = get_variable($1);
      struct AstNode* node = create_reference_node(variable);
      push_object_element($1, node);
      $$ = node;
    }
  | object_element_name T_COLON expression {
      push_object_element($1, $3);
    }
  ;


object_element_name:
    T_STRING {
      $$ = copy_string($1);
    }
  | T_QUOTED_STRING {
      $$ = copy_quoted_string($1);
    }

operator_binary:
    expression T_OR expression {
      $$ = create_binary_op_node(NODE_TYPE_OPERATOR_BINARY_OR, $1, $3);
    }
  | expression T_AND expression {
      $$ = create_binary_op_node(NODE_TYPE_OPERATOR_BINARY_AND, $1, $3);
    }
  | expression T_EQ expression {
      $$ = create_binary_op_node(NODE_TYPE_OPERATOR_BINARY_EQ, $1, $3);
    }
  | expression T_NE expression {
      $$ = create_binary_op_node(NODE_TYPE_OPERATOR_BINARY_NE, $1, $3);
    }
  | expression T_LT expression {
      $$ = create_binary_op_node(NODE_TYPE_OPERATOR_BINARY_LT, $1, $3);
    }
  | expression T_GT expression {
      $$ = create_binary_op_node(NODE_TYPE_OPERATOR_BINARY_GT, $1, $3);
    }
  | expression T_LE expression {
      $$ = create_binary_op_node(NODE_TYPE_OPERATOR_BINARY_LE, $1, $3);
    }
  | expression T_GE expression {
      $$ = create_binary_op_node(NODE_TYPE_OPERATOR_BINARY_GE, $1, $3);
    }
  | expression T_IN expression {
      $$ = create_binary_op_node(NODE_TYPE_OPERATOR_BINARY_IN, $1, $3);
    }
  ;

expression:
 operator_binary {
      $$ = $1;
    }
 | value_literal {
      $$ = $1;
    }
  | reference {
      $$ = $1;
    }
  ;

reference:
    T_STRING {
      struct AstNode* node = NULL;
      void* variable = NULL;

      variable = get_variable($1);
      if (variable == NULL) {
          node = create_data_source_node($1);
      } else {
        node = variable;
      }
      $$ = node;
    }
    | reference '.' T_STRING {
        struct AstNode* ref = (struct AstNode*)($1);
        struct AstNode* access = create_attribute_access_node($3);
        add_operation(ref, access);
        $$ = ref;
    }
    | T_OPEN expression T_CLOSE {
        $$ = $2;
    }
    | object {
      $$ = $1;
    }
  ;

numeric_value:
    T_INTEGER {
      $$ = $1;
    }
  | T_DOUBLE {
      $$ = $1;
    }
  ;

value_literal:
    T_QUOTED_STRING {
      $$ = create_string_node($1);
    }
  | numeric_value {
      $$ = $1;
    }
  | T_NULL {
      $$ = create_null_value_node();
    }
  | T_TRUE {
      $$ = create_bool_node(true);
    }
  | T_FALSE {
      $$ = create_bool_node(false);
    }
  ;

in_collection_name:
    T_STRING {
      $$ = create_data_source_node($1);
    }
  ;

variable_name:
    T_STRING {
      $$ = copy_string($1);
    }
  ;

%%
