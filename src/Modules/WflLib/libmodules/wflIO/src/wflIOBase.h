/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _wflIOBase_H
#define _wflIOBase_H

#include "WflLibWin32Header.h"
#include "blLightObject.h"
#include "wflWorkflow.h"

/**
Base reader/writer for all Workflow reader/writers. 

\author Xavi Planes
\date 22 Dec 2010
\ingroup WflLib
*/
class WFLLIB_EXPORT wflIOBase : public blLightObject
{
public:
	typedef wflIOBase Self;
	typedef blSmartPointer<Self> Pointer;
	
public:

	//! Set the complete filename 
	void SetFilename( const char* );

	//!
	void SetInput( wflWorkflow::Pointer workflow );

	//! execute
	virtual void Update( );

	//! Returns the output of the reader
	wflWorkflow::Pointer GetOutput( );


protected:
	wflIOBase( );

	~wflIOBase( );

	//!
	virtual void InternalUpdate( ) = 0;

protected:

	//!
	wflWorkflow::Pointer m_Data;

	//!
	std::string m_Filename;

	//!
	unsigned int m_Version;
};

#endif //_wflIOBase_H
