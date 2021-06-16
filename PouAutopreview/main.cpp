#include <iostream>

#include "AutoPreviewer.h"

using namespace std;

int main()
{
    cout << "Hello world!" << endl;

    AutoPreviewer autoPreviewer;

    if(autoPreviewer.loadConfig("config.txt"))
        autoPreviewer.exploreDirectory();

    return 0;
}
