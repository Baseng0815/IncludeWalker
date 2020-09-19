#include "graph.hpp"

const Node &Graph::getNodeByFile(std::string_view file) {
    auto it = std::find_if(nodes.begin(), nodes.end(), [&](const Node &node) {
        return node.nodeFile == file;
    });

    if (it == nodes.end()) {
        std::cout << "WARNING: node file " << file << " not found\n";
        return nodes[0];
    } else {
        return *it;
    }
}

const Node &Graph::getNodeById(NodeId id) {
    auto it = std::find_if(nodes.begin(), nodes.end(), [&](const Node &node) {
        return node.nodeId == id;
    });

    if (it == nodes.end()) { std::cout << "WARNING: node id " << id << " not found\n"; return nodes[0]; } else { return *it;
    }
}

// recursively create new nodes given a root file and return the new node's id
NodeId Parser::parse(std::string_view file, NodeType type)
{
    Node newNode { m_idCounter++, type, std::string {file} };

    if (!(std::count_if(m_graph.nodes.begin(), m_graph.nodes.end(), [&](const Node &node) { return node.nodeFile == file; }) || type == NodeType::SYSTEM)) {
        size_t end = file.find_last_of('/');
        if (end != std::string_view::npos) {
            std::filesystem::current_path(file.substr(0, file.find_last_of('/')));
        }

        std::ifstream stream { std::string {file.substr(file.find_last_of('/') + 1)} };
        if (!stream.good()) {
            std::cout << "WARNING: file " << file << " could not be opened.\n";
            return 0;
        }

        // only parse includes of local files
        if (type == NodeType::LOCAL) {
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
                            newNode.children.push_back(parse(line, NodeType::SYSTEM));
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
                                newNode.children.push_back(parse(line, NodeType::LOCAL));
                            }
                        }
                    }
                }
            }
        }
    }

    m_graph.nodes.emplace_back(newNode);
    return newNode.nodeId;
}

Parser::Parser(std::string_view file) {
    parse(file);
}

Graph Parser::getGraph()
{ return m_graph; }
