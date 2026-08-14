#pragma once

#include "../llvm_codegen.h"

inline void LLVMCodegen::generateStatement(const Statement& stmt) {
    if (stmt.kind == Statement::Kind::Assignment) {
        llvm::Value* value = generateNode(stmt.value);

        auto* storage = builder.CreateAlloca(
            builder.getInt32Ty(),
            nullptr,
            stmt.name
        );

        builder.CreateStore(value, storage);

        variables[stmt.name] = storage;
    }
}