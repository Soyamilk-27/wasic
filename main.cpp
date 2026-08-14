#include <iostream>

#include "wasic/parsing/lexer.h"
#include "wasic/parsing/parser.h"
#include "wasic/ast_dump.h"

int main() {
    const char* script = R"(
    fn main()
        x = (2 + 3) * -4
        y = [1 -2 3 * 4]
        print(x + y)
    end
    )";

    Lexer lexer(script);
    const auto tokens = lexer.tokenize();

    Parser parser(tokens);
    Program program = parser.parse();

    dumpProgram(program);

    return 0;
}