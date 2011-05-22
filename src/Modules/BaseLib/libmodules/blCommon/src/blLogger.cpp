/**
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#include "blLogger.h"

// log4cplus
#include <log4cplus/configurator.h>

/**
* Configure logging.
* Uses a text based configuration from the "resource/logging.conf" file.
*/
static void configureLogging()
{
    // only configure once
    static int initialized = 0;
    if (initialized)
        return;
    initialized = 1;

	// configure from config file
    log4cplus::PropertyConfigurator config("resource/logging.conf");
    config.configure();
}

log4cplus::Logger blLogger::getInstance(const char* name)
{
    configureLogging();
    // logger objects have a reference counting copy-constructor, so returning by-value is cheap
    return log4cplus::Logger::getInstance(name);
}

