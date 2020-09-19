#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <string_view>

using NodeId = unsigned int;

enum class NodeType {
    LOCAL, SYSTEM
};

struct Node {
    NodeId nodeId;
    NodeType nodeType;
    std::string nodeFile;

    std::vector<NodeId> children;
};

struct Graph {
    public:
        std::vector<Node> nodes;

        const Node &getNodeByFile(std::string_view file);
        const Node &getNodeById(NodeId id);
};

class Parser {
    private:
        Graph m_graph;

        // each id is assigned a file
        NodeId m_idCounter = 0;

        // recursively create new nodes given a root file and return the new node's id
        NodeId parse(std::string_view file, NodeType type = NodeType::LOCAL);

    public:
        Parser(std::string_view file);

        Graph getGraph();
};
