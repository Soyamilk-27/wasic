#include <iostream>

#include "wasic/ast_dump.h"
#include "wasic/parsing/lexer.h"
#include "wasic/parsing/parser.h"
#include "wasic/codegen/llvm_codegen.h"

int main() {
    const char* script = R"(
    fn main()
        a = [3.4 4.5 2+1 1.1]
        x = a[3]
        a[2] = 4

        print(53)
    end
    )";

    Lexer lexer(script);
    const auto tokens = lexer.tokenize();

    Parser parser(tokens);
    Program program = parser.parse();

    dumpProgram(program);

    LLVMCodegen codegen;
    codegen.generate(program);
    codegen.dump();

    return 0;
}