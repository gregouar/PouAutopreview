#include <iostream>
#include "AutoPreviewer.h"

using namespace std;


int main()
{
    cout << "Hello world!" << endl;

    cout<< "Choose your config file:"<< endl;
    cout<< "Default 1 or anything"<<endl;
    cout<< "Left 2 or l"<<endl;
    cout<< "Right 3 or r"<<endl;
    //cout<< "All 4 or a"<<endl;

    char choice;
    cin >> choice;

    string configPath("config.txt");
    if(choice == '2' || choice == 'l')
        configPath = "config_left.txt";
    else if(choice == '3' || choice == 'r')
        configPath = "config_right.txt";
   /* else if(choice == '4' || choice == 'a')
    {
        {
            AutoPreviewer autoPreviewer;

            if(autoPreviewer.loadConfig("config_left.txt"))
                autoPreviewer.exploreDirectory();
        }
        {
            AutoPreviewer autoPreviewer;

            if(autoPreviewer.loadConfig("config_right.txt"))
                autoPreviewer.exploreDirectory();
        }
    }*/


    AutoPreviewer autoPreviewer;

    if(autoPreviewer.loadConfig(configPath))
        autoPreviewer.exploreDirectory();

    return 0;
}
