#ifndef baseLibPCH_h
#define baseLibPCH_h

/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "BaseLibWin32Header.h"

#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>

#include "boost/format.hpp"
#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/signals.hpp>

#ifdef WIN32
  #include <TCHAR.H>
  #include <conio.h>
  #include <windows.h>
#endif

#endif //baseLibPCH_h

