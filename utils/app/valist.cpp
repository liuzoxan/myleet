#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void print(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

int main(int argc, char** argv)
{
    print("Lord. This is %s %s :)\n", "va_list", "sample");
    return EXIT_SUCCESS;
}
