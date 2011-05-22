/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynLibPCH_H
#define _dynLibPCH_H

#include "ModuleDescription.h"

#if defined(__hpux)
  #include <dl.h>
#elif defined(_WIN32) && !defined(__CYGWIN__)
  #include <windows.h>
#elif defined(__APPLE__)
  #include <AvailabilityMacros.h>
  #if MAC_OS_X_VERSION_MAX_ALLOWED < 1030
    #include <mach-o/dyld.h>
  #endif
#elif defined(__BEOS__)
  #include <be/kernel/image.h>
#endif

#include <list>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>

#endif //_dynLibPCH_H
