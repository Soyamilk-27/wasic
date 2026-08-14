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

        default:
            throw std::runtime_error(
                "Unsupported AST node"
            );
    }
}