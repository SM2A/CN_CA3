#include <iomanip>
#include <iostream>
#include "Network.h"
#include <algorithm>
//#include <chrono>

#define INF 99999
#define NO_LINK -1
#define SAME_NODE 0
#define NUM_WIDTH 5
#define SEPARATOR ' '
#define SAME_NODE_ERROR "Source and Destination cannot be the same"

using namespace std;

Network *Network::instance = nullptr;

Network *Network::getInstance() {
    if (instance == nullptr) instance = new Network();
    return instance;
}

void Network::show() {

    cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << "      ";

    for (int i = 0; i < links.size(); ++i) cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << i + 1;
    cout << endl;

    for (int i = 0; i < ((links.size() * NUM_WIDTH) + NUM_WIDTH); ++i) cout << '_';
    cout << endl;

    for (int i = 0; i < links.size(); ++i) {
        cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << i + 1 << '|';
        for (int j : links[i]) cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << j;
        cout << endl;
    }

    cout << endl;
}

void Network::addLink(int source, int destination, int cost) {

    if (source == destination) {
        cerr << SAME_NODE_ERROR << endl;
        return;
    }

    for (int i = 0; links.size() < max(source, destination); ++i) {
        vector<int> row;
        for (int j = 0; row.size() < max(source, destination); ++j) row.push_back(NO_LINK);
        links.push_back(row);
    }

    for (auto &link : links) for (int j = 0; link.size() < max(source, destination); ++j) link.push_back(NO_LINK);

    for (int i = 0; i < links.size(); ++i) links[i][i] = SAME_NODE;

    source -= 1;
    destination -= 1;

    links[source][destination] = cost;
    links[destination][source] = cost;
    edges.emplace_back(pair<int, int>(source, destination), cost);
    edges.emplace_back(pair<int, int>(destination, source), cost);

    if (findNode(source) == nullptr) nodes.push_back(new Node(source));
    if (findNode(destination) == nullptr) nodes.push_back(new Node(destination));
}

void Network::modifyLink(int source, int destination, int cost) {

    if (source == destination) {
        cerr << SAME_NODE_ERROR << endl;
        return;
    }

    source -= 1;
    destination -= 1;

    links[source][destination] = cost;
    links[destination][source] = cost;

    bool sd = false, ds = false;

    for (auto i : edges) {
        if ((i.first.first == source) && (i.first.second == destination)) {
            i.second = cost;
            sd = true;
        }
        if ((i.first.second == source) && (i.first.first == destination)) {
            i.second = cost;
            ds = true;
        }
    }

    if (!sd && !ds) {
        edges.emplace_back(pair<int, int>(source, destination), cost);
        edges.emplace_back(pair<int, int>(destination, source), cost);
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

void Network::lsrp() {
    copyLinks();
    for (int i = 0; i < links.size(); ++i) {
        cout << "Node " << i + 1 << ":" << endl;
        dijkstra(i);
    }
}

void Network::lsrp(int src) {
    copyLinks();
    dijkstra(src - 1);
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

void Network::dijkstra(int src) {

    Node *node = findNode(src);
    node->init(links.size());
    node->distance[src] = 0;

    vector<int> visited(links.size(), 0);
    /*vector<int> lastCost = copyDistance(node->distance);*/

    /*auto begin = chrono::high_resolution_clock::now();*/

    for (int j = 0; j < links.size() - 1; j++) {

        int min = INF, u;

        for (int i = 0; i < links.size(); i++) {
            if (visited[i] == 0 && node->distance[i] <= min) {
                min = node->distance[i];
                u = i;
            }
        }

        visited[u] = 1;

        for (int v = 0; v < links.size(); v++) {
            if (algorithmsLinks[u][v] > 0 && node->distance[u] != INF) {
                int alt = node->distance[u] + algorithmsLinks[u][v];
                if (alt < node->distance[v]) {
                    node->distance[v] = alt;
                    node->prev[v] = u;
                }
            }
        }

        cout << "Iter " << j + 1 << ":" << endl;

        cout << left << setw(NUM_WIDTH * 2) << setfill(SEPARATOR) << "Dest";

        for (int k = 0; k < links.size(); k++) cout << left << setw(NUM_WIDTH * 2) << setfill(SEPARATOR) << k + 1;
        cout << endl;

        for (int k = 0; k < (links.size() * (NUM_WIDTH * 2) + NUM_WIDTH); ++k) cout << '_';
        cout << endl;

        cout << left << setw(NUM_WIDTH * 2) << setfill(SEPARATOR) << "Cost";

        for (int k = 0; k < links.size(); k++) {
            if (node->distance[k] == INF) cout << left << setw(NUM_WIDTH * 2) << setfill(SEPARATOR) << NO_LINK;
            else cout << left << setw(NUM_WIDTH * 2) << setfill(SEPARATOR) << node->distance[k];
        }
        cout << endl;

        /*if (compareCost(lastCost, node->distance)) break;*/ // To end algorithm when two iterations are the same
        /*lastCost = copyDistance(node->distance);*/ // To end algorithm when two iterations are the same

        for (int k = 0; k < (links.size() * (NUM_WIDTH * 2) + NUM_WIDTH); ++k) cout << '_';
        cout << endl << endl;
    }

    /*auto end = chrono::high_resolution_clock::now();*/

    cout << "Path: [s]->[d]   Min-Cost   Shortest Path" << endl;
    cout << "      ---------  ---------  --------------" << endl;
    for (int k = 0; k < links.size(); k++) {
        if (k == src) continue;

        cout << "       " << "[" << src + 1 << "]"
             << "->" << "[" << k + 1 << "]   "
             << node->distance[k] << "        "
             << src + 1;

        recvPrintPath(node->prev, k);
        cout << endl;
    }

    /*auto duration = chrono::duration_cast<chrono::nanoseconds>(end - begin);
    cout << duration.count() << " nanosecond" << endl;*/

    cout << endl;

    node->clear();
}

void Network::recvPrintPath(vector<int> prev, int visited) {
    if (prev[visited] == -1) return;
    recvPrintPath(prev, prev[visited]);
    cout << "->" << visited + 1;
}

void Network::copyLinks() {

    algorithmsLinks.clear();

    for (int i = 0; i < links.size(); ++i) {
        algorithmsLinks.emplace_back();
        for (int j = 0; j < links[i].size(); ++j) {
            algorithmsLinks[i].push_back(links[i][j]);
            if (links[i][j] == -1) algorithmsLinks[i][j] = INF;
        }
    }
}

bool Network::compareCost(vector<int> prev, vector<int> now) {
    for (int i = 0; i < min(prev.size(), now.size()); ++i) if (prev[i] != now[i]) return false;
    return true;
}

vector<int> Network::copyDistance(const vector<int> &distance) {
    vector<int> newDistance;
    for (int i : distance) newDistance.push_back(i);
    return newDistance;
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
