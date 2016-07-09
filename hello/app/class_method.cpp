/*
 * class_method.cpp
 *
 *  Created on: Jul 9, 2016
 *      Author: zxliu
 */

#include <cstring>
#include <iostream>

using namespace std;

namespace
{

class Temp
{
public:
    Temp(void) :
            m_pData(nullptr)
    {
        cout << "default construct" << endl;
    }

    explicit Temp(const char* str)
    {
        cout << "param construct" << endl;
        m_pData = strdup(str);
    }

    Temp(const Temp& t)
    {
        cout << "copy construct" << endl;
        m_pData = strdup(t.m_pData);
    }

    Temp& operator=(const Temp& t)
    {
        cout << "assign construct" << endl;
        if (m_pData == nullptr)
        {
            delete m_pData;
        }
        m_pData = strdup(t.m_pData);
        return *this;
    }

    Temp(Temp&& t)
    {
        cout << "clone" << endl;
        m_pData = t.m_pData;
        t.m_pData = nullptr;
    }

    Temp& operator=(Temp&& t)
    {
        cout << "clone assign" << endl;
        m_pData = t.m_pData;
        t.m_pData = nullptr;
        return *this;
    }

    ~Temp()
    {
        cout << "destruct" << endl;
        if (m_pData != nullptr)
        {
            cout << "free" << endl;
            delete m_pData;
            m_pData = nullptr;
        }
    }

public:
    const char* getData(void) const
    {
        return m_pData;
    }

private:
    char *m_pData;
};

} // namespace

std::ostream& operator<<(std::ostream& os, const Temp& t)
{
    if (t.getData() != nullptr)
    {
        os << t.getData();
    }
    else
    {
        os << "empty";
    }
    return os;
}

int main(int argc, char **argv)
{
    Temp p1; // default
    Temp p2("temp2 string"); // param
    Temp p3 = p2; // copy
    Temp p4("temp4 string"); // param
    Temp p5 = Temp("temp5 string"); // param && assign
    cout << p5 << endl; // 5
    p5 = p4; // assign 4

    Temp &&p6 = Temp("temp6 string");
    cout << p6 << endl; // 6

    Temp p7 = move(p5);
    cout << p5 << endl; // empty
    cout << p7 << endl; // 4

    p5 = move(p7);
    cout << p5 << endl; // empty
    cout << p7 << endl; // 4
    return EXIT_SUCCESS;
}
