#include "../include/common_stack.h"

#define STACK_INIT_CAPACITY 16

struct CommonStack* init_common_stack() {
    struct CommonStack* stack = malloc(sizeof(struct CommonStack));
    stack->data = calloc(STACK_INIT_CAPACITY, sizeof(void*));
    stack->capacity = STACK_INIT_CAPACITY;
    stack->stored_count = 0;
    return stack;
}

static void realloc_common_stack(struct CommonStack* stack) {
    void *new_data = calloc(stack->capacity * 2, sizeof(void*));
    memcpy(new_data, stack->data, stack->stored_count * sizeof(void*));
    free(stack->data);
    stack->data = new_data;
}

void push_common_stack(struct CommonStack* stack, void* data) {
    if (stack->capacity < stack->stored_count + 1) {
        realloc_common_stack(stack);
    }
    stack->data[stack->stored_count] = data;
    stack->stored_count++;
}

void* pop_common_stack(struct CommonStack* stack) {
    if (stack->stored_count <= 0) return NULL;
    stack->stored_count--;
    return stack->data[stack->stored_count];
}

void* peek_common_stack(struct CommonStack* stack) {
    if (stack->stored_count <= 0) return NULL;
    return stack->data[stack->stored_count - 1];
}