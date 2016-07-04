/*
 * example.cpp
 *
 *  Created on: Jul 3, 2016
 *      Author: zxliu
 */

#include <gflags/gflags.h>
#include <iostream>

#include "utils/log.h"

bool ValidatePort(const char* flagname, int32_t value)
{
    if (value > 0 && value < 32768)   // value is ok
        return true;
    printf("Invalid value for --%s: %d\n", flagname, (int) value);
    return false;
}

/*
 DEFINE_bool: boolean
 DEFINE_int32: 32-bit integer
 DEFINE_int64: 64-bit integer
 DEFINE_uint64: unsigned 64-bit integer
 DEFINE_double: double
 DEFINE_string: C++ string

 example:
 DEFINE_bool(big_menu, true, "Include 'advanced' options in the menu listing");
 */

DEFINE_int32(port, 0, "What port to listen on");
DEFINE_bool(verbose, true, "Turn on verbose");

int main(int argc, char*argv[])
{
    std::string usage("This program does nothing.  Sample usage:\n");
    usage += std::string(argv[0]) + " --port 1234 \n or :\n -flagfile=foo.conf";

    gflags::SetUsageMessage(usage);
    bool port_dummy = gflags::RegisterFlagValidator(&FLAGS_port, &ValidatePort);
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    std::cout << "port :" << FLAGS_port << std::endl;
    Log::info() << "verbose" << FLAGS_verbose << Log::endl;

    return EXIT_SUCCESS;
}
