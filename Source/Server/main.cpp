#include <iostream>
#include "Server.h"

using namespace std;

int main()
{
    try
    {
        Server server;
        server.start();
    }
    catch(const exception& e)
    {
        cerr << e.what() << '\n';
    }
    return 0;
}