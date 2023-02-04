#include <iostream>
#include "Router.h"

using namespace std;

int main()
{
    try
    {
        Router router;
        router.start();
    }
    catch(const exception& e)
    {
        cerr << e.what() << '\n';
    }
    return 0;
}