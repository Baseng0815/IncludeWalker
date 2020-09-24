#pragma once

#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <string_view>

struct Node;

using NodeId = unsigned int;
using NodePtr = std::unique_ptr<Node>;

enum class NodeType {
    LOCAL, SYSTEM
};

struct Node {
    NodeId nodeId;
    NodeType nodeType;
    std::string nodeFile;

    // TODO add multiple parents
    NodeId parent;
    std::vector<NodeId> children;
};

struct Graph {
    public:
        std::vector<NodePtr> nodes;

        const Node *getNodeByFile(std::string_view file) const;
        const Node *getNodeById(NodeId id) const;
};

class Parser {
    private:
        Graph m_graph;

        // each id is assigned a file
        NodeId m_idCounter = 1;

        // recursively create new nodes given a root file and return the new node's id
        NodeId parse(std::string_view file, NodeType type = NodeType::LOCAL, NodeId parent = 0);

    public:
        Parser(std::string_view file);

        Graph &getGraph();
};
