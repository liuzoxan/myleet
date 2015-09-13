#include <iostream>
#include <stdlib.h>

using namespace std;

class Point
{
public:
    Point(int x, int y) :
        x(x), y(y)
    {

    }

    friend ostream& operator<<(ostream& os, const Point& point)
    {
        return os << "x " << point.x << " y " << point.y;
    }

    int x;
    int y;
};

int main(int argc, char** argv)
{
    Point* pointer = static_cast<Point*>(malloc(sizeof(Point)));
    new (pointer) Point(1,2);

    cout << *pointer << endl;
    pointer->~Point();

    free(pointer);
    return EXIT_SUCCESS;
}

