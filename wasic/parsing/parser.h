#pragma once
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

#include "lexer.h"
#include "../types/types_parser.h"

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens)
        : source(tokens) {}

    Program parse() {
        Program program;

        while (!check(TokenKind::Eof)) {
            skipNewlines();

            if (check(TokenKind::Eof))
                break;

            program.functions.push_back(parseFunction());
        }

        return program;
    }

private:
    const std::vector<Token>& source;
    size_t pos = 0;

    const Token& current() const {
        return source[pos];
    }

    const Token& peek(size_t offset = 1) const {
        return source[pos + offset];
    }

    bool check(TokenKind kind) const {
        return current().kind == kind;
    }

    const Token& consume(TokenKind kind) {
        if (!check(kind)) {
            throw std::runtime_error(
                "Expected " +
                std::string(tokenName(kind)) +
                ", got " +
                std::string(tokenName(current().kind))
            );
        }

        return source[pos++];
    }

    void skipNewlines() {
        while (check(TokenKind::Newline))
            ++pos;
    }

    Function parseFunction() {
        consume(TokenKind::Fn);

        const Token& name = consume(TokenKind::Identifier);

        consume(TokenKind::LParen);
        consume(TokenKind::RParen);

        skipNewlines();

        Function fn;
        fn.name = name.text;

        while (!check(TokenKind::End)) {
            fn.body.push_back(parseStatement());
            skipNewlines();
        }

        consume(TokenKind::End);

        return fn;
    }

    Statement parseStatement() {
        // what
        return {};
    }
};