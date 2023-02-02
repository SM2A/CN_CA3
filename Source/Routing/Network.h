#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include "Node.h"

class Network {

private:

    static Network *instance;

    std::vector<Node *> nodes;
    std::vector<std::vector<int>> links;
    std::vector<std::pair<std::pair<int,int>,int>> edges;
    std::vector<std::vector<int>> algorithmsLinks;

    Network() {

    }

    Node* findNode(int num);
    void copyLinks();
    void dijkstra(int src);
    void recvPrintPath(std::vector<int> prev, int visited);
    void recvPrintPath(std::vector<int> const &parent, int vertex, int source);
    std::vector<int> copyDistance(const std::vector<int>& distance);
    bool compareCost(std::vector<int> prev, std::vector<int> now);
    void bellmanFord(int src);

public:

    static Network *getInstance();

    void show();

    void addLink(int source, int destination, int cost);
    void modifyLink(int source, int destination, int cost);
    void removeLink(int source, int destination);

    void lsrp();
    void lsrp(int src);

    void dvrp();
    void dvrp(int src);

};

#endif
