#ifndef AST_H
#define AST_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

#include "./ast_node.h"
#include "./common_stack.h"

void init_ast(void);

struct AstNode *get_ast_root(void);
struct AstNode* get_current_scope(void);

char* copy_quoted_string(const char* src);
char* copy_string(const char* src);

void addVariable(char *name, struct AstNode* value);
struct AstNode *get_variable(char *name);

struct AstNode *create_filter_node(struct AstNode *node);
struct AstNode *create_return_node(struct AstNode *node);
struct AstNode *create_remove_node(struct AstNode *expression, struct AstNode *collection);
struct AstNode *create_insert_node(struct AstNode *expression, struct AstNode *collection);
struct AstNode *create_update_node(struct AstNode *docExpression, struct AstNode *collection);
struct AstNode *create_variable_node(char *name);
struct AstNode *create_reference_node(struct AstNode *astNode);
struct AstNode *create_bool_node(bool value);
struct AstNode *create_int_node(int32_t value);
struct AstNode *create_double_node(double value);
struct AstNode *create_string_node(char* value);
struct AstNode *create_data_source_node(char *name);
struct AstNode *create_null_value_node(void);

struct AstNode* create_attribute_access_node(char* name);
struct AstNode* create_for_node(struct AstNode* variableNode, struct AstNode* expression);
struct AstNode* create_binary_op_node(enum AstNodeType astNodeType, struct AstNode* left, struct AstNode* right);
struct AstNode* create_node_object();
struct AstNode* create_object_element(char* name, struct AstNode* expr);

void add_operation(struct AstNode* parent, struct AstNode* child);
void push_common(struct AstNode* node);
struct AstNode* pop_common();
void push_object_element(char * attributeName, struct AstNode* node);

#endif //AST_H
