/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef CORECOMMONMACROS_H
#define CORECOMMONMACROS_H


#ifdef coreClassNameMacro
#undef coreClassNameMacro
#endif

//! Preserved for legacy reasons.
#define coreClassNameMacro(classname) \
	virtual const char* GetNameOfClass(void) const { return #classname; }

#endif //CORECOMMONMACROS_H
