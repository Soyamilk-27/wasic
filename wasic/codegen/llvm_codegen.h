#pragma once

#include <memory>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include "../types/types_parser.h"

class LLVMCodegen {
public:
    LLVMCodegen()
        : builder(context),
          module(std::make_unique<llvm::Module>("wasic", context)) {}

    void generate(const Program& program);

    void dump();

private:
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> module;

    void generateFunction(const Function& fn);
    void generateStatement(const Statement& stmt);
    llvm::Value* generateNode(const Node& node);
};