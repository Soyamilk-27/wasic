#pragma once

#include "../parser.h"

inline Statement Parser::parseStatement() {
    Statement stmt;

    // Parse the left side first.
    Node lhs = parseExpression();

    // Assignment
    if (check(TokenKind::Equal)) {
        consume(TokenKind::Equal);

        if (lhs.kind != Node::Kind::Name &&
            lhs.kind != Node::Kind::Index) {
            throw std::runtime_error(
                "Invalid assignment target"
            );
            }

        stmt.kind = Statement::Kind::Assignment;
        stmt.target = std::move(lhs);
        stmt.value = parseExpression();

        return stmt;
    }

    // Normal expression statement
    stmt.kind = Statement::Kind::Expression;
    stmt.value = std::move(lhs);

    return stmt;
}