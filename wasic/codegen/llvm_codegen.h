#pragma once

#include <memory>
#include <unordered_map>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

#include "../types/types_parser.h"

class LLVMCodegen {
public:
    LLVMCodegen()
        : builder(context),
          module(std::make_unique<llvm::Module>("wasic", context)) {}

    void generate(const Program& program);

    void dump() const;

private:
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> module;
    std::unordered_map<std::string, llvm::AllocaInst*> variables;

    void generateFunction(const Function& fn);
    void generateStatement(const Statement& stmt);
    llvm::Value* generateNode(const Node& node);
    llvm::Value* generateIndexPointer(const Node& node);
};

#include "llvm_codegen/core.h"
#include "llvm_codegen/function.h"
#include "llvm_codegen/statement.h"
#include "llvm_codegen/node.h"
