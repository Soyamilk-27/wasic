#include <iostream>

#include "wasic/parsing/lexer.h"
#include "wasic/parsing/parser.h"
#include "wasic/codegen/llvm_codegen.h"

int main() {
    const char* script = R"(
    fn main()
        x = 10
        y = x * 2
    end
    )";

    Lexer lexer(script);
    const auto tokens = lexer.tokenize();

    Parser parser(tokens);
    Program program = parser.parse();

    LLVMCodegen codegen;
    codegen.generate(program);
    codegen.dump();

    return 0;
}