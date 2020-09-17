#include <ncurses.h>
#include <array>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>
#include <memory>
#include <iostream>

enum class NodeType {
    LOCAL, SYSTEM
};

typedef unsigned int NodeId;
struct Node {
    NodeType nodeType;
    NodeId nodeId;

    std::vector<Node*> children;
};

class Graph {
    private:
        std::map<std::string, NodeId> m_localMap;
        std::map<std::string, NodeId> m_systemMap;

    public:
        // main.cpp is seen as the parent
        Node rootNode;

        void parse(const std::string &file)
        {
            Node *currentNode =

            std::vector<NodeId> localNodes, systemNodes;
            std::ifstream stream { file };
            if (!stream.good()) {
                std::cout << "WARNING: file " << file << " could not be opened.\n";
                return;
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
                            if (m_systemMap.count(line) == 0) {
                                NodeId newId = m_systemMap.emplace(std::move(line), m_systemMap.size()).first->second;
                                systemNodes.push_back(newId);
                            }
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
                                if (m_localMap.count(line) == 0) {
                                    NodeId newId = m_localMap.emplace(std::move(line), m_localMap.size()).first->second;
                                    localNodes.push_back(newId);
                                }
                            }
                        }
                    }
                }
            }
        }
};


void add_string(std::vector<std::string> &lines)
{
    std::string str(100, ' ');
    echo();
    mvgetnstr(lines.size(), 0, &str[0], str.size());
    noecho();
    move(lines.size(), 0);
    clrtoeol();
    lines.emplace_back(std::move(str));
}

int main(int argc, char **argv)
{
    Graph graph;
    graph.parse("test/src/main.cpp");

    return 0;

    initscr();
    noecho();
    curs_set(1);
    noecho();

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    color_set(1, 0);

    std::vector<std::string> lines = { "Hallo", "das hier", "sind", "Reihen" };
    int curline = 0;
    bool running = true;

    while (running) {
        for (size_t line = 0; line < lines.size(); line++) {
            if (line == curline) {
                color_set(1, 0);
            } else {
                color_set(0, 0);
            }

            mvaddstr(line, 0, lines[line].c_str());
        }

        color_set(0, 0);
        move(curline, 0);

        char c = getch();

        switch (c) {
            case 'q':
                running = false;
                break;
            case 'a':
                add_string(lines);
                break;
            case 'j':
                if (curline < lines.size() - 1) curline++;
                break;
            case 'k':
                if (curline > 0) curline--;
                break;
        }
    }

    endwin();
    return 0;
}
