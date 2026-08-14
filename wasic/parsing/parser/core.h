#pragma once

#include "../parser.h"

inline const Token& Parser::current() const {
    return source[pos];
}

inline const Token& Parser::peek(size_t offset) const {
    return source[pos + offset];
}

inline bool Parser::check(TokenKind kind) const {
    return current().kind == kind;
}

inline const Token& Parser::consume(TokenKind kind) {
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

inline void Parser::skipNewlines() {
    while (check(TokenKind::Newline))
        ++pos;
}

inline Program Parser::parse() {
    Program program;

    while (!check(TokenKind::Eof)) {
        skipNewlines();

        if (check(TokenKind::Eof))
            break;

        program.functions.push_back(parseFunction());
    }

    return program;
}

inline Function Parser::parseFunction() {
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