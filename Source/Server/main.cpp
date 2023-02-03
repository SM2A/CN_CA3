#include <iostream>
#include "Server.h"

using namespace std;

int main()
{
    try
    {
        Server server;
        server.start();
    cerr<<"QQQQQQQQQQQQ44"<<endl;
    }
    catch(const exception& e)
    {
        cerr << e.what() << '\n';
    }
    cerr<<"QQQQQQQQQQQQ55"<<endl;
    return 0;
}