#include <iostream>
#include <memory>

#include "wasic/parsing/lexer.h"
#include "wasic/parsing/parser.h"

int main() {
    const char* script = R"(
    fn main()
        x = [1 2 3 4 5]
        y = x * 2
        print(y)
    end
    )";

    Lexer lexer(script);
    const auto tokens = lexer.tokenize();

    Parser parser(tokens);


    for (const auto& token : tokens) {
        std::cout
            << tokenName(token.kind)
            << " : "
            << token.text
            << '\n';
    }

    return 0;
}