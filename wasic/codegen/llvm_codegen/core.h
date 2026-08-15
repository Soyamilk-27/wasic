#pragma once

#include "../llvm_codegen.h"

inline void LLVMCodegen::generate(const Program& program) {
    declareBuiltins();

    for (auto& func : program.functions) {
        generateFunction(func);
    }
}

inline void LLVMCodegen::dump() const {
    if (llvm::verifyModule(*module, &llvm::errs())) {
        throw std::runtime_error(
            "Invalid LLVM module\n"
        );
    }

    module->print(llvm::outs(), nullptr);
}
