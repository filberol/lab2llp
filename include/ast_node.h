#ifndef ASTNODE_H
#define ASTNODE_H

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>

enum AstNodeType {
    NODE_TYPE_ROOT = 0,
    NODE_TYPE_FOR = 1,
    NODE_TYPE_FILTER = 2,
    NODE_TYPE_RETURN = 3,
    NODE_TYPE_REMOVE = 4,
    NODE_TYPE_INSERT = 5,
    NODE_TYPE_UPDATE = 6,
    NODE_TYPE_VARIABLE = 7,
    NODE_TYPE_REFERENCE = 8,
    NODE_TYPE_DATA_SOURCE = 9,
    NODE_TYPE_ATTRIBUTE_ACCESS = 10,
    NODE_TYPE_OPERATOR_BINARY_OR = 11,
    NODE_TYPE_OPERATOR_BINARY_AND = 12,
    NODE_TYPE_OPERATOR_BINARY_EQ = 13,
    NODE_TYPE_OPERATOR_BINARY_NE = 14,
    NODE_TYPE_OPERATOR_BINARY_LT = 15,
    NODE_TYPE_OPERATOR_BINARY_GT = 16,
    NODE_TYPE_OPERATOR_BINARY_LE = 17,
    NODE_TYPE_OPERATOR_BINARY_GE = 18,
    NODE_TYPE_OPERATOR_BINARY_IN = 19,
    NODE_TYPE_OBJECT = 20,
    NODE_TYPE_OBJECT_ELEMENT = 21
};

enum AstNodeValueType {
    BOOL_TYPE = 0,
    INT_TYPE = 1,
    DOUBLE_TYPE = 2,
    STRING_TYPE = 3,
    COLUMN_REF_TYPE = 4,
    DATASOURCE_REF_TYPE = 7,
    NULL_TYPE = 6,
    INVALID = 5,
    ATTRIBUTE_TYPE = 9
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
    int32_t childrenCount;
    struct AstNode **data;

};

struct AstNode {
    enum AstNodeType astNodeType;
    struct AstNodeValue astNodeValue;
    struct ChildrenList childrenList;
};

#define INIT_CHILDREN_LIST (struct ChildrenList) {0, 0, NULL};
#define INVALID_AST_NODE_VALUE (struct AstNodeValue){INVALID, {._string = NULL}};

void addAstOperation(struct AstNode *parent, struct AstNode *children);

#endif //ASTNODE_H
