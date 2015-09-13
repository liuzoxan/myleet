#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* mymemmove(void* dst, void* src, size_t n)
{
    // overlap
    if ( src < dst and dst < (char*)src + n)
    {
        void* tdst = malloc(n);
        memcpy(tdst, src, n);
        memcpy(dst, tdst, n);
        free(tdst);
    }
    else
    {
        memcpy(dst, src, n);
    }

    return dst;
}

int main(int argc, char** argv)
{
    char mem[] = "1234567890";
    assert(sizeof(mem) == 11);
    printf("move before: %s", mem);
    mymemmove(mem+2,mem, 5);
    printf("move after: %s", mem);
    return EXIT_SUCCESS;
}
