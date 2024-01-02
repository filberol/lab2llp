#ifndef ASTNODE_H
#define ASTNODE_H

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>

enum AstNodeType {
    NODE_TYPE_ROOT = 0,
    NODE_TYPE_FOR,
    NODE_TYPE_FILTER,
    NODE_TYPE_RETURN,
    NODE_TYPE_REMOVE,
    NODE_TYPE_INSERT,
    NODE_TYPE_UPDATE,
    NODE_TYPE_VARIABLE,
    NODE_TYPE_REFERENCE,
    NODE_TYPE_DATA_SOURCE,
    NODE_TYPE_ATTRIBUTE_ACCESS,
    NODE_TYPE_OPERATOR_BINARY_OR,
    NODE_TYPE_OPERATOR_BINARY_AND,
    NODE_TYPE_OPERATOR_BINARY_EQ,
    NODE_TYPE_OPERATOR_BINARY_NE,
    NODE_TYPE_OPERATOR_BINARY_LT,
    NODE_TYPE_OPERATOR_BINARY_GT,
    NODE_TYPE_OPERATOR_BINARY_LE,
    NODE_TYPE_OPERATOR_BINARY_GE,
    NODE_TYPE_OPERATOR_BINARY_IN,
    NODE_TYPE_OBJECT,
    NODE_TYPE_OBJECT_ELEMENT
};

enum AstNodeValueType {
    BOOL_TYPE = 0,
    INT_TYPE,
    DOUBLE_TYPE,
    STRING_TYPE,
    COLUMN_REF_TYPE,
    DATASOURCE_REF_TYPE,
    NULL_TYPE,
    INVALID,
    ATTRIBUTE_TYPE
};

struct AstNodeValue {
    enum AstNodeValueType type;
    union {
        double _double;
        int32_t _int;
        bool _bool;
        char *_string;
    };
};

struct ChildrenList {
    int32_t capacity;
    int32_t children_count;
    struct AstNode **data;

};

struct AstNode {
    enum AstNodeType type;
    struct AstNodeValue value;
    struct ChildrenList children;
};

#define INIT_CHILDREN_LIST (struct ChildrenList) {0, 0, NULL};
#define INVALID_AST_NODE_VALUE (struct AstNodeValue){INVALID, {._string = NULL}};

void add_ast_node(struct AstNode *parent, struct AstNode *children);
char* copy_quoted_string(const char* src);
char* copy_string(const char* src);

#endif //ASTNODE_H
