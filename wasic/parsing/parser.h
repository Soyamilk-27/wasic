#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "lexer.h"
#include "../types/types_parser.h"

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens)
        : source(tokens) {}

    Program parse();

private:
    const std::vector<Token>& source;
    size_t pos = 0;

    const Token& current() const;
    const Token& peek(size_t offset = 1) const;

    bool check(TokenKind kind) const;
    const Token& consume(TokenKind kind);

    void skipNewlines();

    Function parseFunction();

    Node parseUnary();
    Node parseExpression();
    Node parseTerm();
    Node parsePrimary();
    Node parseArray();

    Statement parseStatement();
};

#include "parser/core.h"
#include "parser/expression.h"
#include "parser/statement.h"