#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include "Node.h"

class Network {

private:

    static Network *instance;

    int lastNodeNum = 0;

    std::vector<Node *> nodes;
    std::vector<std::vector<int>> links;
    std::vector<std::pair<std::pair<int,int>,int>> edges;
    std::vector<std::vector<int>> algorithmsLinks;

    Network() {

    }

    Node* findNode(int num);
    Node* findNode(std::string ip);
    void recvPrintPath(std::vector<int> prev, int visited);
    void recvPrintPath(std::vector<int> const &parent, int vertex, int source);
    void bellmanFord(int src);

public:

    static Network *getInstance();

    void draw();

    void addNode(std::string type, std::string ip);
    void addLink(std::string source, std::string destination, int cost);
    void modifyLink(std::string source, std::string destination, int cost);
    void removeLink(int source, int destination);

    void dvrp();
    void dvrp(int src);

};

#endif
