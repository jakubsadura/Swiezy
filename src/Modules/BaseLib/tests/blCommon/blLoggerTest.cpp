// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "blLoggerTest.h"

#include <blLogger.h>

void blLoggerTest::TestLog()
{
    log4cplus::Logger logger = blLogger::getInstance("baselib");
    
    LOG4CPLUS_TRACE_METHOD(logger, "blLoggerTest::TestLog()");
    LOG4CPLUS_TRACE(logger, "This is a TRACE message.");
    LOG4CPLUS_DEBUG(logger, "This is a DEBUG message.");
    LOG4CPLUS_INFO(logger, "This is a INFO message.");
    LOG4CPLUS_WARN(logger, "This is a WARN message.");
    LOG4CPLUS_ERROR(logger, "This is a ERROR message.");
    LOG4CPLUS_FATAL(logger, "This is a FATAL message.");
}
