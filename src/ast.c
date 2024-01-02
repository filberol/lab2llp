#include "ast.h"

static struct AstNode *root = NULL;
static struct AstNode *curr_scope = NULL;

static struct VariableStack *variable_stack = NULL;
static struct CommonStack *common_stack = NULL;

void init_ast() {
    variable_stack = init_variable_stack(16);
    common_stack = init_common_stack();
}

static struct AstNode *create_node(enum AstNodeType node_type) {
    struct AstNode *node = (struct AstNode *) malloc(sizeof(struct AstNode));
    node->type = node_type;
    node->children = INIT_CHILDREN_LIST;
    return node;
}

struct AstNode *create_filter_node(struct AstNode *node) {
    struct AstNode *filterNode = create_node(NODE_TYPE_FILTER);
    add_operation(filterNode, node);
    return filterNode;
}

struct AstNode *create_return_node(struct AstNode *node) {
    struct AstNode *new_node = create_node(NODE_TYPE_RETURN);
    add_operation(new_node, node);
    return node;
}

struct AstNode *create_remove_node(
        struct AstNode *expression, struct AstNode *collection
) {
    struct AstNode *removeNode = create_node(NODE_TYPE_REMOVE);
    add_operation(removeNode, expression);
    add_operation(removeNode, collection);
    return removeNode;
}

struct AstNode *create_insert_node(
        struct AstNode *expression, struct AstNode *collection
) {
    struct AstNode *insertNode = create_node(NODE_TYPE_INSERT);
    add_operation(insertNode, expression);
    add_operation(collection, collection);
    return insertNode;
}

struct AstNode *create_update_node(
        struct AstNode *docExpression, struct AstNode *collection
) {
    struct AstNode *updateNode = create_node(NODE_TYPE_UPDATE);
    add_operation(updateNode, docExpression);
    add_operation(updateNode, collection);
    return updateNode;

}

struct AstNode *create_reference_node(struct AstNode *astNode) {
    struct AstNode *string_node = create_node(NODE_TYPE_REFERENCE);
    string_node->value.type = COLUMN_REF_TYPE;
    string_node->value._string = copy_string(astNode->value._string);
    return string_node;

}

#define CREATE_NODE_WITH_TYPE(field_type, field_link, value_expr) \
    struct AstNode *node = create_node(NODE_TYPE_VARIABLE); \
    node->value.type = field_type; \
    node->value.field_link = value_expr; \
    return node;

struct AstNode *create_bool_node(bool value) { CREATE_NODE_WITH_TYPE(BOOL_TYPE, _bool, value) }

struct AstNode *create_int_node(int32_t value) { CREATE_NODE_WITH_TYPE(INT_TYPE, _int, value) }

struct AstNode *create_double_node(double value) { CREATE_NODE_WITH_TYPE(DOUBLE_TYPE, _double, value) }

struct AstNode *create_string_node(char *value) { CREATE_NODE_WITH_TYPE(STRING_TYPE, _string, copy_string(value)) }

struct AstNode *create_null_value_node(void) { CREATE_NODE_WITH_TYPE(NULL_TYPE, _int, 0) }

struct AstNode *create_variable_node(char* name) { CREATE_NODE_WITH_TYPE(STRING_TYPE, _string, copy_string(name)) }

struct AstNode *create_data_source_node(char *name) {
    struct AstNode *datasourceNode = create_node(NODE_TYPE_DATA_SOURCE);
    datasourceNode->value.type = DATASOURCE_REF_TYPE;
    datasourceNode->value._string = copy_string(name);
    return datasourceNode;
}

static struct AstNode *createRoot(void) {
    struct AstNode *astRoot = create_node(NODE_TYPE_ROOT);
    astRoot->value = INVALID_AST_NODE_VALUE;
    return astRoot;

}

struct AstNode *get_ast_root(void) {
    if (root == NULL) root = createRoot();
    return root;
}

struct AstNode *get_current_scope(void) {
    if (curr_scope == NULL) return get_ast_root();
    return curr_scope;
}


struct AstNode *create_attribute_access_node(char *name) {
    struct AstNode *attribute = create_node(NODE_TYPE_ATTRIBUTE_ACCESS);
    attribute->value.type = ATTRIBUTE_TYPE;
    attribute->value._string = copy_string(name);
    return attribute;

}

struct AstNode *create_binary_op_node(enum AstNodeType astNodeType, struct AstNode *left, struct AstNode *right) {
    struct AstNode *astNode = create_node(astNodeType);
    add_operation(astNode, left);
    add_operation(astNode, right);
    return astNode;
}

struct AstNode *create_node_object() {
    struct AstNode *astNode = create_node(NODE_TYPE_OBJECT);
    return astNode;
}

struct AstNode *create_object_element(char *name, struct AstNode *expr) {
    struct AstNode *astNode = create_node(NODE_TYPE_OBJECT_ELEMENT);
    astNode->value.type = ATTRIBUTE_TYPE;
    astNode->value._string = name;
    add_operation(astNode, expr);
    return astNode;
}

void add_operation(struct AstNode *parent, struct AstNode *child) {
    add_ast_node(parent, child);
    if (child->type == NODE_TYPE_FOR) {
        curr_scope = child;
    }
}

void push_common(struct AstNode *node) {
    push_common_stack(common_stack, node);
}

struct AstNode *pop_common() {
    return (struct AstNode *) pop_common_stack(common_stack);
}


void push_object_element(char *attributeName, struct AstNode *node) {
    struct AstNode *curr_object = (struct AstNode *) (peek_common_stack(common_stack));
    struct AstNode *obj_attr = create_object_element(attributeName, node);
    add_operation(curr_object, obj_attr);
}

void add_variable(char *name, struct AstNode *value) {
    struct VariableNode variableNode = {
            .variable_name = copy_string(name),
            .value = value
    };
    add_variable_data(variable_stack, variableNode);
}

struct AstNode *get_variable(char *name) {
    struct VariableNode *vn = get_stack_variable(variable_stack, name);
    struct AstNode *var = vn == NULL ? NULL : create_variable_node(vn->variable_name);
    return var;
}

struct AstNode *create_for_node(struct AstNode *variableNode, struct AstNode *expression) {
    struct AstNode *astNode = create_node(NODE_TYPE_FOR);
    add_operation(astNode, variableNode);
    add_operation(astNode, expression);
    return astNode;
}
