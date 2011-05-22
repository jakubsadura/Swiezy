/**
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef BLLOGGER_H
#define BLLOGGER_H

// log4cplus
#include <log4cplus/logger.h>

/**
* blLogger
* Class used to initialise the log4cplus.
* Uses a text based configuration from the "resource/logging.conf" file.
*/
class blLogger
{

public:

    /**
    * Get a log4cplus::Logger object.
    * @param name The name of the logger (can be used in the logging configuration to filter messages).
    */
    static log4cplus::Logger getInstance(const char* name);

private:

    /**
    * Private constructor, don't create instances of this class.
    */
    blLogger() {}
    
}; // class blLogger

#endif // BLLOGGER_H

