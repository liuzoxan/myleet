/*
 * logtest.cpp
 *
 *  Created on: Jul 11, 2015
 *      Author: zxliu
 */

#include <iostream>
#include <fstream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils/log.h"

void log(std::ostream& os, const char* format ,va_list args)
{
    char buff[1024];
    vsnprintf(buff, sizeof(buff), format, args);
    os << buff;
    os.flush();
}

void print(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    log(std::cout, format, args);
    va_end(args);

}

int main(int argc, char** argv)
{
    print("%s %s, from origin\n", "hello", "my lord");
    Log::debug("%s %s, from Log", "hello", "my lord");

    std::ofstream ofs;
    if (not ofs.good())
    {
        Log::error("ofstream is not good!");
    }
    else
    {
        ofs.open("/dev/null");
        if (ofs.good())
        {
            Log::debug("ofstream is good now :)");
        }
        ofs << "" << std::endl;
    }

    Log::debug() << "debug" << Log::endl;
    Log::fatal() << "fatal" << Log::endl;
    Log::error() << "error" << std::endl;

    Log::level = LOG_FATAL;
    Log::error("error, should not see this message");
    Log::error() << "error, should not see this message" << Log::endl;
    return EXIT_SUCCESS;
}

