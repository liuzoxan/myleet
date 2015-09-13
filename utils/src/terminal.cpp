#include <fcntl.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include "utils/terminal.h"

namespace utils
{

bool Terminal::m_isTTY = isatty(STDOUT_FILENO) and isatty(STDOUT_FILENO);
unsigned short int Terminal::m_col = 120;
unsigned short int Terminal::m_row = 42;

int Terminal::getTerminalWidth(int def)
{
    if (isatty(STDOUT_FILENO) and isatty(STDOUT_FILENO))
    {
#ifdef TIOCGSIZE
        struct ttysize ts;
        ioctl(STDOUT_FILENO, TIOCGSIZE, &ts);
        Terminal::m_col = ts.ts_cols;
        Terminal::m_row = ts.ts_lines;
        return ts.ts_cols;
#elif defined(TIOCGWINSZ)
        struct winsize ts;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &ts);
        Terminal::m_col = ts.ws_col;
        Terminal::m_row = ts.ws_row;
        return ts.ws_col;
#endif // TIOCGSIZE
    }
    Terminal::m_col = def;
    Terminal::m_row = 42;
    return def;
}

const char* Terminal::Normal()
{
    if (Terminal::m_isTTY)
        return "\033[0m";
    else
        return "";
}

const char* Terminal::White()
{
    if (Terminal::m_isTTY)
        return "\033[1;37m";
    else
        return "";
}

const char* Terminal::LightGray()
{
    if (Terminal::m_isTTY)
        return "\033[37m";
    else
        return "";
}

const char* Terminal::Gray()
{
    if (Terminal::m_isTTY)
        return "\033[1;30m";
    else
        return "";
}

const char* Terminal::Black()
{
    if (Terminal::m_isTTY)
        return "\033[30m";
    else
        return "";
}

const char* Terminal::Red()
{
    if (Terminal::m_isTTY)
        return "\033[31m";
    else
        return "";
}

const char* Terminal::LightRed()
{
    if (Terminal::m_isTTY)
        return "\033[1;31m";
    else
        return "";
}

const char* Terminal::Green()
{
    if (Terminal::m_isTTY)
        return "\033[32m";
    else
        return "";
}

const char* Terminal::LightGreen()
{
    if (Terminal::m_isTTY)
        return "\033[1;32m";
    else
        return "";
}

const char* Terminal::Brown()
{
    if (Terminal::m_isTTY)
        return "\033[33m";
    else
        return "";
}

const char* Terminal::Yellow()
{
    if (Terminal::m_isTTY)
        return "\033[1;33m";
    else
        return "";
}

const char* Terminal::Blue()
{
    if (Terminal::m_isTTY)
        return "\033[34m";
    else
        return "";
}

const char* Terminal::LightBlue()
{
    if (Terminal::m_isTTY)
        return "\033[1;34m";
    else
        return "";
}

const char* Terminal::Purple()
{
    if (Terminal::m_isTTY)
        return "\033[35m";
    else
        return "";
}

const char* Terminal::Pink()
{
    if (Terminal::m_isTTY)
        return "\033[1;35m";
    else
        return "";
}

const char* Terminal::Cyan()
{
    if (Terminal::m_isTTY)
        return "\033[36m";
    else
        return "";
}

const char* Terminal::LightCyan()
{
    if (Terminal::m_isTTY)
        return "\033[1;36m";
    else
        return "";
}

const char* Terminal::bgBlack()
{
    if (Terminal::m_isTTY)
        return "\033[40m";
    else
        return "";
}

const char* Terminal::bgRed()
{
    if (Terminal::m_isTTY)
        return "\033[41m";
    else
        return "";
}

const char* Terminal::bgGreen()
{
    if (Terminal::m_isTTY)
        return "\033[42m";
    else
        return "";
}

const char* Terminal::bgBrown()
{
    if (Terminal::m_isTTY)
        return "\033[43m";
    else
        return "";
}

const char* Terminal::bgBlue()
{
    if (Terminal::m_isTTY)
        return "\033[44m";
    else
        return "";
}

const char* Terminal::bgPurple()
{
    if (Terminal::m_isTTY)
        return "\033[45m";
    else
        return "";
}

const char* Terminal::bgCyan()
{
    if (Terminal::m_isTTY)
        return "\033[46m";
    else
        return "";
}

const char* Terminal::bgLightGray()
{
    if (Terminal::m_isTTY)
        return "\033[47m";
    else
        return "";
}

const char* Terminal::clearToEOL()
{
    return "\033K";
}

const char* Terminal::clearToEOS()
{
    return "\033J";
}

} /* namespace utils */
