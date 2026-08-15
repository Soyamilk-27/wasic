#pragma once

#include <iostream>

#include "types/types_parser.h"

inline void dumpNode(const Node& node, int indent = 0) {
    const std::string pad(indent, ' ');

    switch (node.kind) {
        case Node::Kind::Number:
            std::cout << pad << "Number: " << node.text << '\n';
            break;

        case Node::Kind::Float:
            std::cout << pad << "Float: " << node.text << '\n';
            break;

        case Node::Kind::Name:
            std::cout << pad << "Name: " << node.text << '\n';
            break;

        case Node::Kind::Array:
            std::cout << pad << "Array\n";

            for (const auto& child : node.children)
                dumpNode(child, indent + 2);

            break;

        case Node::Kind::Binary:
            std::cout << pad << "Binary: " << node.text << '\n';

            for (const auto& child : node.children)
                dumpNode(child, indent + 2);

            break;

        case Node::Kind::Call:
            std::cout << pad << "Call: " << node.text << '\n';

            for (const auto& child : node.children)
                dumpNode(child, indent + 2);

            break;

        case Node::Kind::Index:
            std::cout << pad << "Index\n";

            std::cout << pad << "  Array\n";
            dumpNode(node.children[0], indent + 4);

            std::cout << pad << "  Index\n";
            dumpNode(node.children[1], indent + 4);

            break;
    }
}

inline void dumpProgram(const Program& program) {
    std::cout << "Program\n";

    for (const auto& fn : program.functions) {
        std::cout << "  Function: " << fn.name << '\n';

        for (const auto& stmt : fn.body) {
            if (stmt.kind == Statement::Kind::Assignment) {
                std::cout << "    Assignment\n";

                std::cout << "      Target:\n";
                dumpNode(stmt.target, 8);

                std::cout << "      Value:\n";
                dumpNode(stmt.value, 8);
            }
            else {
                std::cout << "    Expression\n";
                dumpNode(stmt.value, 6);
            }
        }
    }
}