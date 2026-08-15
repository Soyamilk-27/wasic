#pragma once

#include "../llvm_codegen.h"

inline void LLVMCodegen::declareBuiltins() {
    llvm::LLVMContext& ctx = context;

    // wapi_print_i32(i32)
    {
        auto* type = llvm::FunctionType::get(
            builder.getVoidTy(),
            {builder.getInt32Ty()},
            false
        );

        llvm::Function::Create(
            type,
            llvm::Function::ExternalLinkage,
            "wapi_print_i32",
            *module
        );
    }

    // wapi_print_f32(f32)
    {
        auto* type = llvm::FunctionType::get(
            builder.getVoidTy(),
            {builder.getFloatTy()},
            false
        );

        llvm::Function::Create(
            type,
            llvm::Function::ExternalLinkage,
            "wapi_print_f32",
            *module
        );
    }

    // wapi_print_array_i32(i32*)
    {
        auto* type = llvm::FunctionType::get(
            builder.getVoidTy(),
            {
                llvm::PointerType::getUnqual(builder.getInt32Ty()),
                builder.getInt32Ty()
            },
            false
        );

        llvm::Function::Create(
            type,
            llvm::Function::ExternalLinkage,
            "wapi_print_array_i32",
            *module
        );
    }

    // wapi_print_array_f32(f32*)
    {
        auto* type = llvm::FunctionType::get(
            builder.getVoidTy(),
            {
                llvm::PointerType::getUnqual(builder.getFloatTy()),
                builder.getInt32Ty()
            },
            false
        );

        llvm::Function::Create(
            type,
            llvm::Function::ExternalLinkage,
            "wapi_print_array_f32",
            *module
        );
    }
}