#include <assert.h>
#include <stdlib.h>

#include "utils/log.h"

struct UnpackedStruct
{
    char c1;
    int n;
    char c2;
};

struct PackedStruct
{
    char c1;
    int n;
    char c2;
}__attribute__((__packed__));

struct UnpackedStructLong
{
    char c1;
    int n;
    char c2;
    long l;
};

struct PackedStructLong
{
    char c1;
    int n;
    char c2;
    long l;
}__attribute__((__packed__));

class UnpakcedClass
{
private:
    char c1;
    int n;
    char c2;
};

class PakcedClass
{
private:
    char c1;
    int n;
    char c2;
}__attribute__((__packed__));

int main(int argc, char** argv)
{
    Log::debug("sizof(UnpackedStruct):%d", sizeof(UnpackedStruct));
    Log::debug("sizof(PackedStruct):%d", sizeof(PackedStruct));
    Log::debug("sizof(UnpackedStructLong):%d", sizeof(UnpackedStructLong));
    Log::debug("sizof(PackedStructLong):%d", sizeof(PackedStructLong));
    Log::debug("sizof(UnpakcedClass):%d", sizeof(UnpakcedClass));
    Log::debug("sizof(PakcedClass):%d", sizeof(PakcedClass));
    return EXIT_SUCCESS;
}


