#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>

using namespace std;

size_t hash_combine(size_t hash, const unsigned char* data, size_t length)
{
    for (size_t byteIndex = 0; byteIndex < length; ++byteIndex)
    {
        hash ^= data[byteIndex];
        hash *= 0x100000001b3;
    }

    return hash;
}

size_t hash_value(const unsigned char* c, size_t length) {
    return hash_combine(0xcbf29ce484222325, c, length);
}


typedef std::unordered_map<std::string, std::string> stringmap;

stringmap merge (stringmap a, stringmap b)
{
    stringmap temp(a);
    temp.insert(b.begin(), b.end());
    return temp;
}

int main(int argc, char** argv)
{
    stringmap first;
    stringmap second({{"appler", "red"},{"lemon", "yellow"}});
    stringmap third({{"orange", "orange"},{"strawberry", "red"}});
    stringmap fourth (second);
    stringmap fifth (merge(third, fourth));
    stringmap sixth (fifth.begin(), fifth.end());

    std::cout << "sixth contains:";

    for (stringmap::const_iterator ite = sixth.begin();
            ite != sixth.end();
            ite++)
    {
        cout << " " << ite->first << ":" << ite->second;
        cout << endl;
    }

    std::hash<std::string> h;
    size_t n = h("Just fucking");
    return EXIT_SUCCESS;
}
