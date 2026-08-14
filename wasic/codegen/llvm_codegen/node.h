#pragma once

#include "../llvm_codegen.h"

inline llvm::Value* LLVMCodegen::generateNode(const Node& node) {
    switch (node.kind) {
        case Node::Kind::Number:
            return builder.getInt32(std::stoll(node.text));

        case Node::Kind::Name: {
            auto it = variables.find(node.text);

            if (it == variables.end()) {
                throw std::runtime_error(
                    "Unknown variable: " + node.text
                );
            }

            return builder.CreateLoad(
                builder.getInt32Ty(),
                it->second,
                node.text
            );
        }

        case Node::Kind::Binary: {
            llvm::Value* lhs = generateNode(node.children[0]);
            llvm::Value* rhs = generateNode(node.children[1]);

            if (node.text == "+")
                return builder.CreateAdd(lhs, rhs, "add");

            if (node.text == "-")
                return builder.CreateSub(lhs, rhs, "sub");

            if (node.text == "*")
                return builder.CreateMul(lhs, rhs, "mul");

            if (node.text == "/")
                return builder.CreateSDiv(lhs, rhs, "div");

            throw std::runtime_error(
                "Unknown binary operator: " + node.text
            );
        }

        default:
            throw std::runtime_error(
                "Unsupported AST node"
            );
    }
}