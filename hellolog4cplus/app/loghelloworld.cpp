/**
 * @file       logtest.cpp
 * @brief      /mythrift/hellolog4cplus/app/logtest.cpp
 *
 * @date       Sep 14, 2015
 * @author     lzx_bupt@126.com
 * @note       create
 */

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/initializer.h>

#include <iomanip>

int main()
{
    log4cplus::Initializer initializer;
    log4cplus::BasicConfigurator config;
    config.configure();

    log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("main"));
    LOG4CPLUS_WARN(logger, LOG4CPLUS_TEXT("Hello, World!"));
    return 0;
}

