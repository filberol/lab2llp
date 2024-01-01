//
// Created by moyak on 22.10.2023.
//

#ifndef LLP_VARIABLE_STACK_H
#define LLP_VARIABLE_STACK_H
#include <stdint.h>
#include "variable.h"
#include <stdlib.h>
#include <string.h>

struct VariableStack {
    int32_t countStored;
    int32_t capacity;
    struct VariableNode* data;
};


void addData(struct VariableStack* stack, struct VariableNode node);
struct VariableStack* initVariableStack(uint32_t capacity);
struct VariableNode* getStackVariable(struct VariableStack* stack, char* name);

#endif //LLP_VARIABLE_STACK_H
