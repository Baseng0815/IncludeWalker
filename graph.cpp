#include "graph.hpp"

const Node *Graph::getNodeByFile(std::string_view file) {
    auto it = std::find_if(nodes.begin(), nodes.end(), [&](const NodePtr &node) {
        return node->nodeFile == file;
    });

    if (it == nodes.end()) {
        std::cout << "WARNING: node file " << file << " not found\n";
        return nodes[0].get();
    } else {
        return it->get();
    }
}

const Node *Graph::getNodeById(NodeId id) {
    auto it = std::find_if(nodes.begin(), nodes.end(), [&](const NodePtr &node) {
        return node->nodeId == id;
    });

    if (it == nodes.end()) { std::cout << "WARNING: node id " << id << " not found\n";
        return nodes[0].get();
    } else {
        return it->get();
    }
}

// recursively create new nodes given a root file and return the new node
const Node *Parser::parse(std::string_view file, NodeType type)
{
    // node already existing? return it
    const auto node = std::find_if(m_graph.nodes.begin(), m_graph.nodes.end(), [&](const NodePtr &node) {
        return node->nodeFile == file && node->nodeType == type; });

    if (node != m_graph.nodes.end()) {
        return node->get();

    // else, create new node
    } else {
        NodePtr newNode = std::make_unique<Node>();
        newNode->nodeFile = std::string {file};
        newNode->nodeType = type;
        newNode->nodeId = m_idCounter++;

        // only parse child includes of local files
        if (type == NodeType::LOCAL) {
            size_t end = file.find_last_of('/');
            if (end != std::string_view::npos) {
                std::filesystem::current_path(file.substr(0, file.find_last_of('/')));
            }

            // open file
            std::ifstream stream { std::string {file.substr(file.find_last_of('/') + 1)} };
            if (!stream.good()) {
                std::cout << "WARNING: file " << file << " could not be opened.\n";
                return 0;
            }

            std::string line;
            while (std::getline(stream, line)) {
                size_t index = line.find("#include");
                if (index != std::string::npos) {
                    size_t start = line.find('<');
                    if (start != std::string::npos) {
                        size_t end = line.find('>');
                        if (end <= start) {
                            std::cout << "ERROR: file " << file << "; line " << line << " badly formatted\n";
                            continue;
                        } else {
                            start++;
                            line = line.substr(start, end - start);
                            newNode->children.emplace_back(parse(line, NodeType::SYSTEM));
                        }
                    } else {
                        start = line.find('"') + 1;
                        if (start != std::string::npos) {
                            line = line.substr(start);
                            size_t end = line.find('"');
                            if (end == std::string::npos) {
                                std::cout << "ERROR: file " << file << "; line " << line << " badly formatted\n";
                                continue;
                            } else {
                                start++;
                                line = line.substr(0, end);
                                newNode->children.emplace_back(parse(line, NodeType::LOCAL));
                            }
                        }
                    }
                }
            }
        }

        const Node *newNodeCPtr = newNode.get();
        m_graph.nodes.emplace_back(std::move(newNode));
        return newNodeCPtr;
    }
}

Parser::Parser(std::string_view file) {
    parse(file);
}

Graph &Parser::getGraph()
{ return m_graph; }
