#ifndef UTILS_TERMINAL_H_
#define UTILS_TERMINAL_H_

#include <string>

namespace utils {

class Terminal
{
public:
    static int getTerminalWidth(int def=120);

    // Reset to normal color and background
    static const char* Normal();

    // Text colors
    static const char* White();
    static const char* LightGray();
    static const char* Gray();
    static const char* Black();
    static const char* Red();
    static const char* LightRed();
    static const char* Green();
    static const char* LightGreen();
    static const char* Brown();
    static const char* Yellow();
    static const char* Blue();
    static const char* LightBlue();
    static const char* Purple();
    static const char* Pink();
    static const char* Cyan();
    static const char* LightCyan();

    // Background colors
    static const char* bgBlack();
    static const char* bgRed();
    static const char* bgGreen();
    static const char* bgBrown();
    static const char* bgBlue();
    static const char* bgPurple();
    static const char* bgCyan();
    static const char* bgLightGray();

    // Control
    // http://ascii-table.com/ansi-escape-sequences-vt-100.php
    static const char* clearToEOL();
    static const char* clearToEOS();

private:
    static bool m_isTTY;
    static unsigned short int m_col;
    static unsigned short int m_row;
};

} /* namespace utils */

#endif /* UTILS_TERMINAL_H_ */
