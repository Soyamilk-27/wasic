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

    Node parseUnary() {
        if (check(TokenKind::Minus)) {
            consume(TokenKind::Minus);

            Node node;
            node.kind = Node::Kind::Binary;
            node.text = "-";

            // 0 - x
            Node zero;
            zero.kind = Node::Kind::Number;
            zero.text = "0";

            node.children.push_back(std::move(zero));
            node.children.push_back(parseUnary());

            return node;
        }

        if (check(TokenKind::Plus)) {
            consume(TokenKind::Plus);
            return parseUnary();
        }

        return parsePrimary();
    }

    Node parseExpression() {
        Node left = parseTerm();

        while (check(TokenKind::Plus) ||
               check(TokenKind::Minus)) {

            Node op;
            op.kind = Node::Kind::Binary;
            op.text = consume(current().kind).text;

            op.children.push_back(std::move(left));
            op.children.push_back(parseTerm());

            left = std::move(op);
        }

        return left;
    }

    Node parseTerm() {
        Node left = parseUnary();

        while (check(TokenKind::Star) ||
               check(TokenKind::Slash)) {

            Node op;
            op.kind = Node::Kind::Binary;
            op.text = consume(current().kind).text;

            op.children.push_back(std::move(left));
            op.children.push_back(parseUnary());

            left = std::move(op);
        }

        return left;
    }

    Node parsePrimary() {
        if (check(TokenKind::LParen)) {
            consume(TokenKind::LParen);

            Node node = parseExpression();

            consume(TokenKind::RParen);

            return node;
        }

        if (check(TokenKind::Number)) {
            Node node;
            node.kind = Node::Kind::Number;
            node.text = consume(TokenKind::Number).text;
            return node;
        }

        if (check(TokenKind::Identifier)) {
            std::string name = consume(TokenKind::Identifier).text;

            if (check(TokenKind::LParen)) {
                consume(TokenKind::LParen);

                Node call;
                call.kind = Node::Kind::Call;
                call.text = name;

                if (!check(TokenKind::RParen)) {
                    call.children.push_back(parseExpression());
                }

                consume(TokenKind::RParen);

                return call;
            }

            Node node;
            node.kind = Node::Kind::Name;
            node.text = name;

            return node;
        }

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
            array.children.push_back(parseExpression());
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