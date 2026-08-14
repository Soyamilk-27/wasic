#include <iostream>

#include "wasic/ast_dump.h"
#include "wasic/parsing/lexer.h"
#include "wasic/parsing/parser.h"
#include "wasic/codegen/llvm_codegen.h"

int main() {
    const char* script = R"(
    fn chick()
        b = 3
    end

    fn main()
        a = [3.4 4.5 2+1 1.1]
        x = 10
        y = 2.5
        z = x + y
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