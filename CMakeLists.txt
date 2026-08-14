cmake_minimum_required(VERSION 4.2)

project(wasic LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "LLVM version: ${LLVM_VERSION}")
message(STATUS "LLVM config: ${LLVM_DIR}")

add_executable(wasic main.cpp
        wasic/types/types_parser.h
        wasic/parsing/lexer.h
        wasic/parsing/parser.h
        wasic/parsing/parser/core.h
        wasic/parsing/parser/expression.h
        wasic/parsing/parser/statement.h
        wasic/ast_dump.h
        wasic/codegen/llvm_codegen.h
)

target_include_directories(wasic PRIVATE
        ${LLVM_INCLUDE_DIRS}
)

target_compile_definitions(wasic PRIVATE
        ${LLVM_DEFINITIONS}
)

llvm_map_components_to_libnames(LLVM_LIBS
        core
        support
)

target_link_libraries(wasic PRIVATE
        ${LLVM_LIBS}
)