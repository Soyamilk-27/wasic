#pragma once

#include "../llvm_codegen.h"

inline llvm::Value* LLVMCodegen::generateNode(const Node& node) {
    switch (node.kind) {
        case Node::Kind::Number: {
            auto* num = builder.getInt32(std::stoll(node.text));
            return num;
        }

        default:
            throw std::runtime_error(
                "Unsupported AST node"
            );
    }
}