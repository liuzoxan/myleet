#include <stdlib.h>
#include <iostream>

using namespace std;

class Point
{
public:
    Point(int x)
    {
        this->x = x;
    }

    friend ostream& operator<<(ostream& os, const Point& p)
    {
        os << p.x;
        return os;
    }
private:
    int x;
};

class A
{

};

class B:public A
{

};

class C:public B
{

};

template <typename T>
void func(const T& t, A)
{
    cout << "A object" << endl;
}

template <typename T>
void func(const T&t, C)
{
    cout << "C object" << endl;
}

int main(int argc, char** argv)
{
    func(2, A());
    func(2, B());
    func(2, C());
    return EXIT_SUCCESS;
}
