#pragma once

#include "../llvm_codegen.h"

inline void LLVMCodegen::generateFunction(const Function& fn) {
    // int main()
    auto* functionType = llvm::FunctionType::get(builder.getInt32Ty(), false);
    auto* function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, fn.name, *module);
    auto* entry = llvm::BasicBlock::Create(context, "entry", function);
    builder.SetInsertPoint(entry);

    for (auto& stmt : fn.body) {
        generateStatement(stmt);
    }

    // Success
    builder.CreateRet(llvm::ConstantInt::get(builder.getInt32Ty(), 0));
}