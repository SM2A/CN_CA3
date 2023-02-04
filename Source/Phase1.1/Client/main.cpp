#include <iostream>
#include "Client.h"

using namespace std;

int main()
{
    try
    {
        Client client;
        client.start();
    }
    catch(const exception& e)
    {
        cerr << e.what() << '\n';
    }
    return 0;
}