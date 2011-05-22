/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef COREASSERT_H
#define COREASSERT_H

#include "assert.h"

/**
\brief Use this macro to assert that certain logical conditions are met.
Only use assertions to check programming logic. 
\ingroup gmCommonObjects
\sa http://www.xsi-blog.com/archives/82
*/
#define coreAssertMacro(expr) assert(expr)

#define NeedsImplementation coreAssertMacro("The requested function has not \
been implemented yet.\n It will be available in future releases\n \
Please Master, give me a body!!" && 0);
#define NotYetImplemented NeedsImplementation
#endif //COREASSERT_H
