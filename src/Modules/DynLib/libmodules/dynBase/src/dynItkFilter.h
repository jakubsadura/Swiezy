/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynItkFilter_H
#define _dynItkFilter_H

#include "DynLibWin32Header.h"
#include "dynFilter.h"
#include "dynItkFilterIFace.h"

class dynItkFilterIFace;

/**
ITK dynamic Filter execution

Uses a itk::SmartPointer to the filter

\author Xavi Planes
\date 18 Sept 2010
\ingroup DynLib
*/
class DYNLIB_EXPORT dynItkFilter : public dynFilter
{
public:
	typedef dynItkFilter Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);

	//!
	virtual dynFilterFunction GetFunctionSetInput( );
	virtual dynFilterFunction GetFunctionGetOutput();

protected:
	//!
	dynItkFilter( );

	//!
	virtual ~dynItkFilter( );

	//!
	virtual dynFilterIFace* GetInstance( );

	//!
	virtual void NewInstance( );

	//!
	virtual void Delete( );

private:
	//!
	dynItkFilterIFace::Pointer m_FilterIFace;
};



#endif // _dynItkFilter_H

