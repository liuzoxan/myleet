#ifndef UTILS_LOG_H_
#define UTILS_LOG_H_

#include <iostream>

enum LogLevel
{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_PRODUCTION,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL,
    LOG_FATAL_EXIT
};

class Log
{
public:
    static LogLevel level;
private:
    static std::ofstream devnull;
    static std::ostream& devNull();
public:
    static void debug(const char* format, ...);
    static void info(const char* format, ...);
    static void warn(const char* format, ...);
    static void error(const char* format, ...);
    static void errorNo(const char* message);

    static void fatal(const char* format, ...);
    static void fatalExit(const char* format, ...) __attribute__((noreturn));


    static std::ostream& debug();
    static std::ostream& info();
    static std::ostream& warn();
    static std::ostream& error();
    static std::ostream& fatal();

    static std::ostream& endl(std::ostream& stream);

private:
    static void log(std::ostream& os, va_list args, const char* format);

};

#endif /* UTILS_LOG_H_ */
