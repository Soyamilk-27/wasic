#pragma once

#include "../llvm_codegen.h"

inline llvm::Value* LLVMCodegen::generateNode(const Node& node) {
    switch (node.kind) {
        case Node::Kind::Number:
            return builder.getInt32(std::stoll(node.text));

        case Node::Kind::Float:
            return llvm::ConstantFP::get(
                builder.getFloatTy(),
                std::stof(node.text)
            );

        case Node::Kind::Name: {
            auto it = variables.find(node.text);

            if (it == variables.end()) {
                throw std::runtime_error(
                    "Unknown variable: " + node.text
                );
            }

            return builder.CreateLoad(
                it->second->getAllocatedType(),
                it->second,
                node.text
            );
        }

        case Node::Kind::Array: {
            if (node.children.empty()) {
                throw std::runtime_error(
                    "Empty arrays are not supported"
                );
            }

            std::vector<llvm::Value*> elements;

            for (const auto& child : node.children)
                elements.push_back(generateNode(child));

            // If any element is float, the entire array is float.
            bool isFloat = false;

            for (auto* value : elements) {
                if (value->getType()->isFloatingPointTy()) {
                    isFloat = true;
                    break;
                }
            }

            if (isFloat) {
                for (auto*& value : elements) {
                    if (value->getType()->isIntegerTy()) {
                        value = builder.CreateSIToFP(
                            value,
                            builder.getFloatTy(),
                            "int_to_float"
                        );
                    }
                }
            }

            llvm::Type* elementType =
                isFloat
                    ? builder.getFloatTy()
                    : builder.getInt32Ty();

            auto* arrayType = llvm::ArrayType::get(
                elementType,
                elements.size()
            );

            llvm::Value* result =
                llvm::UndefValue::get(arrayType);

            for (size_t i = 0; i < elements.size(); ++i) {
                result = builder.CreateInsertValue(
                    result,
                    elements[i],
                    {static_cast<unsigned>(i)},
                    "array"
                );
            }

            return result;
        }

        case Node::Kind::Binary: {
            llvm::Value* lhs = generateNode(node.children[0]);
            llvm::Value* rhs = generateNode(node.children[1]);

            bool lhsArray = lhs->getType()->isArrayTy();
            bool rhsArray = rhs->getType()->isArrayTy();

            // ------------------------------------------------------------
            // Array arithmetic
            // ------------------------------------------------------------

            if (lhsArray || rhsArray) {
                size_t count;

                if (lhsArray && rhsArray) {
                    auto* lhsType =
                        llvm::cast<llvm::ArrayType>(lhs->getType());

                    auto* rhsType =
                        llvm::cast<llvm::ArrayType>(rhs->getType());

                    if (lhsType->getNumElements() != rhsType->getNumElements()) {
                        throw std::runtime_error(
                            "Array size mismatch in binary operation"
                        );
                    }

                    count = lhsType->getNumElements();
                }
                else if (lhsArray) {
                    count = llvm::cast<llvm::ArrayType>(
                        lhs->getType()
                    )->getNumElements();
                }
                else {
                    count = llvm::cast<llvm::ArrayType>(
                        rhs->getType()
                    )->getNumElements();
                }

                // Determine the element type.
                llvm::Type* elementType = nullptr;

                if (lhsArray) {
                    elementType =
                        llvm::cast<llvm::ArrayType>(
                            lhs->getType()
                        )->getElementType();
                }
                else {
                    elementType =
                        llvm::cast<llvm::ArrayType>(
                            rhs->getType()
                        )->getElementType();
                }

                auto* resultType =
                    llvm::ArrayType::get(elementType, count);

                llvm::Value* result =
                    llvm::UndefValue::get(resultType);

                for (size_t i = 0; i < count; ++i) {
                    llvm::Value* leftElement;
                    llvm::Value* rightElement;

                    if (lhsArray) {
                        leftElement = builder.CreateExtractValue(
                            lhs,
                            {static_cast<unsigned>(i)},
                            "lhs"
                        );
                    }
                    else {
                        leftElement = lhs;
                    }

                    if (rhsArray) {
                        rightElement = builder.CreateExtractValue(
                            rhs,
                            {static_cast<unsigned>(i)},
                            "rhs"
                        );
                    }
                    else {
                        rightElement = rhs;
                    }

                    // Array float => scalar gets promoted to float.
                    if (elementType->isFloatingPointTy()) {
                        if (leftElement->getType()->isIntegerTy()) {
                            leftElement = builder.CreateSIToFP(
                                leftElement,
                                elementType,
                                "int_to_float"
                            );
                        }

                        if (rightElement->getType()->isIntegerTy()) {
                            rightElement = builder.CreateSIToFP(
                                rightElement,
                                elementType,
                                "int_to_float"
                            );
                        }
                    }

                    llvm::Value* value = nullptr;

                    if (elementType->isFloatingPointTy()) {
                        if (node.text == "+")
                            value = builder.CreateFAdd(
                                leftElement,
                                rightElement,
                                "fadd"
                            );
                        else if (node.text == "-")
                            value = builder.CreateFSub(
                                leftElement,
                                rightElement,
                                "fsub"
                            );
                        else if (node.text == "*")
                            value = builder.CreateFMul(
                                leftElement,
                                rightElement,
                                "fmul"
                            );
                        else if (node.text == "/")
                            value = builder.CreateFDiv(
                                leftElement,
                                rightElement,
                                "fdiv"
                            );
                    }
                    else {
                        if (node.text == "+")
                            value = builder.CreateAdd(
                                leftElement,
                                rightElement,
                                "add"
                            );
                        else if (node.text == "-")
                            value = builder.CreateSub(
                                leftElement,
                                rightElement,
                                "sub"
                            );
                        else if (node.text == "*")
                            value = builder.CreateMul(
                                leftElement,
                                rightElement,
                                "mul"
                            );
                        else if (node.text == "/")
                            value = builder.CreateSDiv(
                                leftElement,
                                rightElement,
                                "div"
                            );
                    }

                    if (!value) {
                        throw std::runtime_error(
                            "Unknown binary operator: " + node.text
                        );
                    }

                    result = builder.CreateInsertValue(
                        result,
                        value,
                        {static_cast<unsigned>(i)},
                        "array"
                    );
                }

                return result;
            }

            // ------------------------------------------------------------
            // Normal scalar arithmetic
            // ------------------------------------------------------------

            bool isFloat =
                lhs->getType()->isFloatingPointTy() ||
                rhs->getType()->isFloatingPointTy();

            if (isFloat) {
                if (lhs->getType()->isIntegerTy()) {
                    lhs = builder.CreateSIToFP(
                        lhs,
                        builder.getFloatTy(),
                        "int_to_float"
                    );
                }

                if (rhs->getType()->isIntegerTy()) {
                    rhs = builder.CreateSIToFP(
                        rhs,
                        builder.getFloatTy(),
                        "int_to_float"
                    );
                }

                if (node.text == "+")
                    return builder.CreateFAdd(lhs, rhs, "fadd");

                if (node.text == "-")
                    return builder.CreateFSub(lhs, rhs, "fsub");

                if (node.text == "*")
                    return builder.CreateFMul(lhs, rhs, "fmul");

                if (node.text == "/")
                    return builder.CreateFDiv(lhs, rhs, "fdiv");
            }

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

        case Node::Kind::Index: {
            const Node& arrayNode = node.children[0];
            const Node& indexNode = node.children[1];

            // a[index]
            if (arrayNode.kind != Node::Kind::Name) {
                throw std::runtime_error(
                    "Array indexing requires an array variable"
                );
            }

            auto it = variables.find(arrayNode.text);

            if (it == variables.end()) {
                throw std::runtime_error(
                    "Unknown variable: " + arrayNode.text
                );
            }

            llvm::AllocaInst* storage = it->second;
            llvm::Type* storageType = storage->getAllocatedType();

            if (!storageType->isArrayTy()) {
                throw std::runtime_error(
                    "Indexing requires an array"
                );
            }

            llvm::Value* index = generateNode(indexNode);

            // index must be an integer scalar
            if (!index->getType()->isIntegerTy()) {
                throw std::runtime_error(
                    "Array index must be an integer"
                );
            }

            auto* arrayType =
                llvm::cast<llvm::ArrayType>(storageType);

            llvm::Type* elementType =
                arrayType->getElementType();

            // array storage:
            //
            // %a = alloca [N x T]
            //
            // GEP indices:
            //   0 = enter the array object
            //   index = element index
            //
            llvm::Value* elementPtr = builder.CreateGEP(
                storageType,
                storage,
                {
                    builder.getInt32(0),
                    index
                },
                "element_ptr"
            );

            return builder.CreateLoad(
                elementType,
                elementPtr,
                "element"
            );
        }

        default:
            throw std::runtime_error(
                "Unsupported AST node"
            );
    }
}