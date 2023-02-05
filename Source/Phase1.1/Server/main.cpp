#include <iostream>
#include <chrono>
#include "Server.h"

using namespace std;

int main()
{
    try
    {
        Server server;
        auto start = chrono::steady_clock::now();
        server.start();
        auto end = chrono::steady_clock::now();
        cout<<"Duartion time: "<<chrono::duration_cast<chrono::seconds>(end - start).count() - 2<<" seconds"<<endl;
    }
    catch(const exception& e)
    {
        cerr << e.what() << '\n';
    }
    return 0;
}