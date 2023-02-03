#include <iomanip>
#include <fstream>
#include <iostream>
#include "Network.h"
#include <algorithm>
//#include <chrono>

#define INF 99999
#define NO_LINK (-1)
#define SAME_NODE 0
#define NUM_WIDTH 5
#define SEPARATOR ' '
#define HOSTS "hosts"
#define ROUTERS "routers"
#define SAME_NODE_ERROR "Source and Destination cannot be the same"

using namespace std;

Network *Network::instance = nullptr;

Network *Network::getInstance() {
    if (instance == nullptr) instance = new Network();
    return instance;
}

void Network::draw() {

    ofstream file("graph.txt");

    cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << "      ";

    for (int i = 0; i < links.size(); ++i) cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << i + 1;
    cout << endl;

    for (int i = 0; i < ((links.size() * NUM_WIDTH) + NUM_WIDTH); ++i) cout << '_';
    cout << endl;

    for (int i = 0; i < links.size(); ++i) {
        cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << i + 1 << '|';
        for (int j : links[i]) {
            cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << j;
            file << j << " ";
        }
        file << endl;
        cout << endl;
    }

    file.close();
    cout << endl;

    system("python3 Source/Routing/graph.py");
}

void Network::addLink(string source, string destination, int cost) {

    if (source == destination) {
        cerr << SAME_NODE_ERROR << endl;
        return;
    }

    Node* srcNode = findNode(source);
    Node* dstNode = findNode(destination);

    if ((srcNode == nullptr) || (dstNode == nullptr)) {
        cerr << "Node not found" << endl;
        return;
    }

    int srcNum = srcNode->num;
    int dstNum = dstNode->num;

    for (int i = 0; links.size() <= max(srcNum, dstNum); ++i) {
        vector<int> row;
        for (int j = 0; row.size() <= max(srcNum, dstNum); ++j) row.push_back(NO_LINK);
        links.push_back(row);
    }

    for (auto &link : links) for (int j = 0; link.size() <= max(srcNum, dstNum); ++j) link.push_back(NO_LINK);

    for (int i = 0; i < links.size(); ++i) links[i][i] = SAME_NODE;

    links[srcNum][dstNum] = cost;
    links[dstNum][srcNum] = cost;
    edges.emplace_back(pair<int, int>(srcNum, dstNum), cost);
    edges.emplace_back(pair<int, int>(dstNum, srcNum), cost);

}

void Network::modifyLink(string source, string destination, int cost) {

    if (source == destination) {
        cerr << SAME_NODE_ERROR << endl;
        return;
    }

    Node* srcNode = findNode(source);
    Node* dstNode = findNode(destination);

    if ((srcNode == nullptr) || (dstNode == nullptr)) {
        cerr << "Node not found" << endl;
        return;
    }

    int srcNum = srcNode->num;
    int dstNum = dstNode->num;

    links[srcNum][dstNum] = cost;
    links[dstNum][srcNum] = cost;

    bool sd = false, ds = false;

    for (auto i : edges) {
        if ((i.first.first == srcNum) && (i.first.second == dstNum)) {
            i.second = cost;
            sd = true;
        }
        if ((i.first.second == srcNum) && (i.first.first == dstNum)) {
            i.second = cost;
            ds = true;
        }
    }

    if (!sd && !ds) {
        edges.emplace_back(pair<int, int>(srcNum, dstNum), cost);
        edges.emplace_back(pair<int, int>(dstNum, srcNum), cost);
    }
}

void Network::removeLink(int source, int destination) {
    source -= 1;
    destination -= 1;

    links[source][destination] = NO_LINK;
    links[destination][source] = NO_LINK;

    for (int i = 0; i < edges.size(); ++i) {
        if ((edges[i].first.first == source) && (edges[i].first.second == destination)) {
            edges.erase(edges.begin() + i);
            i--;
        }
        if ((edges[i].first.second == source) && (edges[i].first.first == destination)) {
            edges.erase(edges.begin() + i);
            i--;
        }
    }
}

void Network::dvrp() {
    for (int i = 0; i < links.size(); ++i) {
        cout << "Node " << i + 1 << ":" << endl;
        bellmanFord(i);
    }
}

void Network::dvrp(int src) {
    bellmanFord(src - 1);
}

Node *Network::findNode(int num) {
    auto iter = find_if(nodes.begin(), nodes.end(), [num](Node *node) -> bool { return node->num == num; });
    if (iter != nodes.end()) return iter[0];
    else return nullptr;
}

Node *Network::findNode(string ip) {
    auto iter = find_if(nodes.begin(), nodes.end(), [ip](Node *node) -> bool { return node->ip == ip; });
    if (iter != nodes.end()) return iter[0];
    else return nullptr;
}

void Network::recvPrintPath(vector<int> prev, int visited) {
    if (prev[visited] == -1) return;
    recvPrintPath(prev, prev[visited]);
    cout << "->" << visited + 1;
}

void Network::recvPrintPath(vector<int> const &parent, int vertex, int source) {
    if (vertex < 0) return;
    recvPrintPath(parent, parent[vertex], source);
    if (vertex != source) cout << " -> ";
    cout << vertex + 1;
}

void Network::bellmanFord(int src) {

    Node *node = findNode(src);
    node->init(links.size());

    node->distance[src] = SAME_NODE;

    /*auto begin = chrono::high_resolution_clock::now();*/

    for (int i = 0; i < links.size(); i++) {
        for (auto &edge : edges) {
            int u = edge.first.first;
            int v = edge.first.second;
            int cost = edge.second;
            if (((node->distance[u] + cost) < node->distance[v]) && (node->distance[u] != INF)) {
                node->path[v] = u;
                node->distance[v] = node->distance[u] + cost;
            }
        }
    }

    /*auto end = chrono::high_resolution_clock::now();*/

    cout << "Dest    Next Hop    Dist    Shortest Path" << endl;
    cout << "-----------------------------------------" << endl;
    for (int k = 0; k < links.size(); k++) {
        cout << k + 1
             << "         " << k + 1
             << "          " << node->distance[k]
             << "        [";
        recvPrintPath(node->path, k, src);
        cout << "]" << endl;
    }

    /*auto duration = chrono::duration_cast<chrono::nanoseconds>(end - begin);
    cout << duration.count() << " nanosecond" << endl;*/

    cout << endl;

    node->clear();
}

void Network::addNode(string type, string ip) {
    Type nodeType;
    if (type == HOSTS) nodeType = HOST;
    else if (type == ROUTERS) nodeType = ROUTER;
    else {
        cerr << "Node type error" << endl;
        return;
    }
    nodes.push_back(new Node(lastNodeNum, nodeType, ip));
    lastNodeNum++;
}
