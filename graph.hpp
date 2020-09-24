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
using NodeRef = const Node*;

enum class NodeType {
    LOCAL, SYSTEM
};

struct Node {
    NodeId nodeId;
    NodeType nodeType;
    std::string nodeFile;

    std::vector<NodeRef> children;
    std::vector<NodeRef> parents;
};

struct Graph {
    public:
        std::vector<NodePtr> nodes;

        NodeRef getNodeByFile(std::string_view file) const;
        NodeRef getNodeById(NodeId id) const;
};

class Parser {
    private:
        Graph m_graph;

        // each id is assigned a file
        NodeId m_idCounter = 1;

        // recursively create new nodes given a root file and return the new node
        NodeRef parse(std::string_view file, NodeType type = NodeType::LOCAL, NodeRef parent = nullptr);

    public:
        Parser(std::string_view file);

        Graph &getGraph();
};
