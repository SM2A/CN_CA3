#include<ctime>
#include<cstdio>
#include <cmath>
#include<vector>
#include<utility>
#include<iostream>
#include <graphics.h>

#define PI 3.14159265

using namespace std;

void addEdge(vector<pair<int, int> > [], int, int, int);
void displayGraph(vector<pair<int, int> > [], int);
vector<pair<int, int> > getCoords(int);

/*int main() {
    int n, e;
    cout << "Enter the no. of Vertices: ";
    cin >> n;
    cout << "Enter the no. of Edges: ";
    cin >> e;
    vector<pair<int, int> > arr[n];
    int c1, c2, weight;
    cout << "Enter connection as in ' a,b,weight ' and remember node starts from 0:-" << endl;
    for (int i = 0; i < e; i++) {
        scanf("%d,%d,%d", &c1, &c2, &weight);
        addEdge(arr, c1, c2, weight);
    }

    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");
    displayGraph(arr, n);
    getch();
    closegraph();
    return 0;
}*/

void addEdge(vector<pair<int, int> > arr[], int u, int v, int weight) {
    arr[u].emplace_back(v, weight);
    arr[v].emplace_back(u, weight);
}

void displayGraph(vector<pair<int, int> > arr[], int n) {
    vector<pair<int, int> > coords = getCoords(n);
    char str[5];
    for (int f = 0; f < n; f++) {
        for (auto i = arr[f].begin(); i < arr[f].end(); i++) {
            line(coords[f].first, coords[f].second, coords[(*i).first].first, coords[(*i).first].second);
            sprintf(str, "%d", (*i).second);
            outtextxy((coords[(*i).first].first + coords[f].first) / 2 - 5,
                      (coords[(*i).first].second + coords[f].second) / 2 - 7,
                      str);
        }
    }

    char symbol[1];
    strcpy(symbol, "A");
    setbkcolor(WHITE);
//    setfillstyle(SOLID_FILL, WHITE);
    for (auto & coord : coords) {
        setcolor(RED);
        circle(coord.first, coord.second,15);
        floodfill(coord.first + 1, coord.second + 1,LIGHTGRAY);
        setcolor(BLACK);
        outtextxy(coord.first - 5, coord.second - 7,symbol);
        *symbol += 1;
    }
}

vector<pair<int, int> > getCoords(int n) {
    int cenX = getmaxx() / 2, cenY = getmaxy() / 2;
    vector<pair<int, int> > coords;
    int x, y;
    x = 0;
    y = (25 * n);
    coords.emplace_back(x + cenX, cenY + y);
    for (int i = 1; i < n; i++) {
        int newX = int((x * cos(2 * PI / n) - y * sin(2 * PI / n)));
        y = int((y * cos(2 * PI / n) + x * sin(2 * PI / n)));
        x = newX;
        coords.emplace_back(x + cenX, cenY + y);
    }
    return coords;
}
