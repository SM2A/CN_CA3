#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include "Node.h"

enum LinkStatus { UP, DOWN };

class Network {

private:

    static Network *instance;

    int lastNodeNum = 0;

    std::vector<Node *> nodes;
    std::vector<std::vector<std::pair<int, LinkStatus>>> links;
    std::vector<std::pair<std::pair<std::pair<int,int>,int>, LinkStatus>> edges;

    Network() {

    }

    Node* findNode(int num);
    Node* findNode(std::string ip);
    std::vector<int> recvPrintPath(std::vector<int> const &parent, int vertex, int source, std::vector<int>);
    void bellmanFord(int src);
    LinkStatus edgeStatus(int src, int dst);
    void changeEdgeStatus(int src, int dst, LinkStatus status);

public:

    static Network *getInstance();

    void draw();

    void addNode(std::string type, std::string ip);
    void addLink(std::string source, std::string destination, int cost);
    void modifyLink(std::string source, std::string destination, int cost);
    void removeLink(std::string source, std::string destination);
    void linkStatus(std::string source, std::string destination);
    void showTable(std::string src);
    void downLink(std::string source, std::string destination, int start, int duration);

    void dvrp(bool logTime = false);

};

#endif
