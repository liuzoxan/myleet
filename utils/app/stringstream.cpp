#include <stdlib.h>
#include <sstream>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    string str = "lord?\nhello\nworld lord";
    stringstream ss(str);

    for (int ind = 0; ind < 5; ind++)
    {
        string tmp;
        ss >> tmp;
        cout << tmp << endl;
    }

    // numbers
    int number = 0;
    stringstream ssi;
    ssi << 100 << ' ' << 200 << ' ' << 300;
    for (int ind = 0; ind < 3; ind++)
    {
        ssi >> number;
        cout << number << endl;
    }

    cout << endl;

    // test input
    stringstream ssc;
    ssc << "lord?\nhello\nworld lord";
    for (int ind = 0; ind < 5; ind++)
    {
        string tmp;
        ssc >> tmp;
        cout << tmp << endl;
    }
    return EXIT_SUCCESS;
}

