#include "utils/log.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <fstream>

using std::cout;
using std::cerr;
using std::ostream;

const size_t LOG_MESSAGE_MAXLEN = 1024;
const size_t FULL_MESSAGE_MAXLEN = LOG_MESSAGE_MAXLEN + 128;

#ifdef NDEBUG
LogLevel Log::level = LOG_PRODUCTION;
#else // NDEBUG
LogLevel Log::level = LOG_DEBUG;
#endif // NDEBUG
std::ofstream Log::devnull;


void Log::log(ostream& os, va_list args, const char* format)
{
    char msg[FULL_MESSAGE_MAXLEN];
    vsnprintf(msg, LOG_MESSAGE_MAXLEN, format, args);
    os << msg << "\n";
    os.flush();
}

void Log::debug(const char* format, ...)
{
    if (level > LOG_DEBUG)
    {
        return;
    }

    va_list args;
    va_start(args, format);
    log(cout, args, format);
    va_end(args);
}

void Log::info(const char* format, ...)
{
    if (level > LOG_INFO)
    {
        return;
    }

    va_list args;
    va_start(args, format);
    log(cout, args, format);
    va_end(args);
}

void Log::warn(const char* format, ...)
{
    if (level > LOG_WARNING)
    {
        return;
    }

    va_list args;
    va_start(args, format);
    log(cout, args, format);
    va_end(args);
}

void Log::error(const char* format, ...)
{
    if (level > LOG_ERROR)
    {
        return;
    }

    va_list args;
    va_start(args, format);
    log(cerr, args, format);
    va_end(args);
}

void Log::errorNo(const char* message)
{
    char errbuf[1024];
    Log::error("%s: %s\n", message, strerror_r(errno, errbuf, sizeof(errbuf)));
}

void Log::fatal(const char* format, ...)
{
    if (level > LOG_FATAL)
    {
        return;
    }

    va_list args;
    va_start(args, format);
    log(cerr, args, format);
    va_end(args);
}

void Log::fatalExit(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    log(cerr, args, format);
    va_end(args);

#ifndef NDEBUG
    abort();
#endif // NDEBUG
    exit(EXIT_FAILURE);
}

ostream& Log::devNull()
{
    if (!devnull.good())
    {
        devnull.open("/dev/null");
    }
    return devnull;
}


ostream& Log::debug()
{
    if (level > LOG_DEBUG)
    {
        return devNull();
    }

    return cout;
}

ostream& Log::info()
{
    if (level > LOG_INFO)
    {
        return devNull();
    }

    return cout;
}

ostream& Log::warn()
{
    if (level > LOG_WARNING)
    {
        return devNull();
    }


    return cout;
}

ostream& Log::error()
{
    if (level > LOG_ERROR)
    {
        return devNull();
    }

    return cerr;
}

ostream& Log::fatal()
{
    if (level > LOG_FATAL)
    {
        return devNull();
    }


    return cerr;
}

std::ostream& Log::endl(std::ostream& stream)
{
    return stream << std::endl;
}
