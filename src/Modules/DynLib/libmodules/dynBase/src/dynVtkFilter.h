/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynVtkFilter_H
#define _dynVtkFilter_H

#include "DynLibWin32Header.h"

#include <string>
#include <list>

#include "dynFilter.h"

class dynVtkFilterIFace;

/**
VTK dynamic Filter execution

Uses a raw pointer to the filter

\author Xavi Planes
\date 20 Sept 2010
\ingroup DynLib
*/
class DYNLIB_EXPORT dynVtkFilter : public dynFilter
{
public:
	typedef dynVtkFilter Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);

	//!
	virtual dynFilterFunction GetFunctionSetInput( );
	virtual dynFilterFunction GetFunctionGetOutput();


protected:
	//!
	dynVtkFilter( );

	//!
	virtual ~dynVtkFilter( );;

	//!
	virtual void NewInstance( );

	//!
	virtual void Delete( );

	//!
	virtual dynFilterIFace* GetInstance( );

protected:
	//!
	dynVtkFilterIFace* m_FilterIFace;
};



#endif // _dynVtkFilter_H

