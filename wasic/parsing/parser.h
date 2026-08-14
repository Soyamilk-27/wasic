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

    Node parseExpression() {
        if (check(TokenKind::LBracket)) {
            return parseArray();
        }

        throw std::runtime_error(
            "Expected expression, got: " + current().text
        );
    }

    Node parseArray() {
        consume(TokenKind::LBracket);

        Node array;
        array.kind = Node::Kind::Array;

        while (!check(TokenKind::RBracket)) {
            if (!check(TokenKind::Number)) {
                throw std::runtime_error(
                    "Expected number in array, got: " + current().text
                );
            }

            Node element;
            element.kind = Node::Kind::Number;
            element.text = consume(TokenKind::Number).text;

            array.children.push_back(std::move(element));
        }

        consume(TokenKind::RBracket);

        return array;
    }

    Statement parseStatement() {
        Statement stmt;

        if (check(TokenKind::Identifier) &&
            peek().kind == TokenKind::Equal) {

            stmt.kind = Statement::Kind::Assignment;
            stmt.name = consume(TokenKind::Identifier).text;
            consume(TokenKind::Equal);
            stmt.value = parseExpression();

            return stmt;
        }

        throw std::runtime_error(
            "Expected statement, got: " + current().text
        );
    }
};