#pragma once

#include "../parser.h"

inline Node Parser::parseUnary() {
    if (check(TokenKind::Minus)) {
        consume(TokenKind::Minus);

        Node node;
        node.kind = Node::Kind::Binary;
        node.text = "-";

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

inline Node Parser::parseExpression() {
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

inline Node Parser::parseTerm() {
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

inline Node Parser::parsePrimary() {
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

    if (check(TokenKind::Float)) {
        Node node;
        node.kind = Node::Kind::Float;
        node.text = consume(TokenKind::Float).text;

        return node;
    }

    if (check(TokenKind::Identifier)) {
        std::string name = consume(TokenKind::Identifier).text;

        if (check(TokenKind::LParen)) {
            consume(TokenKind::LParen);

            Node call;
            call.kind = Node::Kind::Call;
            call.text = name;

            if (!check(TokenKind::RParen))
                call.children.push_back(parseExpression());

            consume(TokenKind::RParen);

            return call;
        }

        Node node;
        node.kind = Node::Kind::Name;
        node.text = name;

        return node;
    }

    if (check(TokenKind::LBracket))
        return parseArray();

    throw std::runtime_error(
        "Expected expression, got: " + current().text
    );
}

inline Node Parser::parseArray() {
    consume(TokenKind::LBracket);

    Node array;
    array.kind = Node::Kind::Array;

    while (!check(TokenKind::RBracket))
        array.children.push_back(parseExpression());

    consume(TokenKind::RBracket);

    return array;
}