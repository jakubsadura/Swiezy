/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef WXID_H
#define WXID_H

#include <string>
#include "TpExtLibWxWidgetsWin32Header.h"

/**
This function converts a stringId into a number id. The mapping between stringId and number id is one-to-one.

\ingroup TpExtLibWxWidgets
\author Maarten Nieber
\date 21 may 2008
*/

int TPEXTLIBWXWIDGETS_EXPORT wxID(const std::string& stringId);

#endif //WXID_H
