#pragma once

#include "../parser.h"

inline Statement Parser::parseStatement() {
    Statement stmt;

    if (check(TokenKind::Identifier) &&
        peek().kind == TokenKind::Equal) {

        stmt.kind = Statement::Kind::Assignment;
        stmt.name = consume(TokenKind::Identifier).text;

        consume(TokenKind::Equal);

        stmt.value = parseExpression();

        return stmt;
    }

    // any expression can be a statement
    stmt.kind = Statement::Kind::Expression;
    stmt.value = parseExpression();

    return stmt;
}