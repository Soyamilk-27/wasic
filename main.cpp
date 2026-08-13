#include <iostream>
#include <memory>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

int main() {
    // LLVMContext owns LLVM's types data structures.
    llvm::LLVMContext context;

    // A Module represents a single LLVM IR module/program.
    auto module = std::make_unique<llvm::Module>("wasic_module", context);

    // IRBuilder makes it easier to construct LLVM instructions.
    llvm::IRBuilder<> builder(context);

    // i32 type.
    llvm::Type* i32 = llvm::Type::getInt32Ty(context);

    // Function type:
    // i32 add(i32, i32)
    llvm::FunctionType* functionType =
        llvm::FunctionType::get(
            i32,
            {i32, i32},
            false
        );

    // Create the function.
    llvm::Function* addFunction =
        llvm::Function::Create(
            functionType,
            llvm::Function::ExternalLinkage,
            "add",
            module.get()
        );

    // Name the function arguments.
    auto args = addFunction->arg_begin();

    llvm::Value* a = args++;
    a->setName("a");

    llvm::Value* b = args++;
    b->setName("b");

    // Create the function's entry basic block.
    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(
            context,
            "entry",
            addFunction
        );

    // Tell IRBuilder to insert instructions into this block.
    builder.SetInsertPoint(entry);

    // Generate:
    //
    // %result = add i32 %a, %b
    llvm::Value* result =
        builder.CreateAdd(a, b, "result");

    // Generate:
    //
    // ret i32 %result
    builder.CreateRet(result);

    // Verify that the generated LLVM IR is valid.
    if (llvm::verifyModule(*module, &llvm::errs())) {
        std::cerr << "Generated invalid LLVM IR!\n";
        return 1;
    }

    // Print the generated LLVM IR.
    module->print(llvm::outs(), nullptr);

    return 0;
}