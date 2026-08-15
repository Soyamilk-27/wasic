#pragma once

#include "../llvm_codegen.h"

inline void LLVMCodegen::generateStatement(const Statement& stmt) {
    if (stmt.kind == Statement::Kind::Expression) {
        generateNode(stmt.value);
        return;
    }

    // ------------------------------------------------------------
    // x = value
    // ------------------------------------------------------------

    if (stmt.target.kind == Node::Kind::Name) {
        llvm::Value* value = generateNode(stmt.value);

        auto* storage = builder.CreateAlloca(
            value->getType(),
            nullptr,
            stmt.target.text
        );

        builder.CreateStore(value, storage);

        variables[stmt.target.text] = storage;
        return;
    }

    // ------------------------------------------------------------
    // a[index] = value
    // ------------------------------------------------------------

    if (stmt.target.kind == Node::Kind::Index) {
        llvm::Value* elementPtr =
            generateIndexPointer(stmt.target);

        llvm::Value* value =
            generateNode(stmt.value);

        llvm::Type* arrayType =
            variables.at(stmt.target.children[0].text)->getAllocatedType();

        auto* elementType =
            llvm::cast<llvm::ArrayType>(arrayType)->getElementType();

        // [float] arrays accept integer scalar assignment via promotion.
        if (elementType->isFloatingPointTy() &&
            value->getType()->isIntegerTy()) {

            value = builder.CreateSIToFP(
                value,
                elementType,
                "int_to_float"
            );
            }

        if (value->getType() != elementType) {
            throw std::runtime_error(
                "Array element type mismatch"
            );
        }

        builder.CreateStore(value, elementPtr);
        return;
    }

    throw std::runtime_error(
        "Invalid assignment target"
    );
}