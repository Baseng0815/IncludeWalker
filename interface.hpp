#include "graph.hpp"

struct InterfaceNode {
    Node graphNode;
    bool isCollapsed;
};

class Interface {
    private:
        std::vector<InterfaceNode> m_interfaceNodes;
        Graph m_graph;

        bool m_isOpen = true;

        void recalculateInterfaceNodes();

    public:
        Interface(Graph graph);
        ~Interface();

        void charInput(char c);

        bool isOpen() const;

        void draw();
};
