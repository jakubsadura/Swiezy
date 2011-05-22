/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynWxControlFactoryBase_H
#define _dynWxControlFactoryBase_H

#include "DynWxAGUILibWin32Header.h"
#include "ModuleDescription.h"
#include "dynBasePanel.h"

/**
Base class for Automated GUI controls Builder

\author Xavi Planes
\date 04 August 2010
\ingroup DynLib
*/
class DYNWXAGUILIB_EXPORT dynWxControlFactoryBase : public blLightObject
{
public:
	typedef dynWxControlFactoryBase Self;
	typedef blSmartPointer<Self> Pointer;

	//!
	virtual wxSizer* CreateTagWidget(
		dynBasePanel* panel,
		wxWindow* parent, 
		const ModuleParameter& moduleParameter) = 0;

	//! This function is called after creating all widgets
	virtual void LayoutAllControls( 
		dynBasePanel* panel,
		ModuleDescription *module ){}

	//! Create help widget
	virtual wxSizer* CreateHelpWidget( wxWindow* parent, ModuleDescription *module ) = 0;

protected:
	//!
	dynWxControlFactoryBase( ){}

	//!
	virtual ~dynWxControlFactoryBase( ){}

};



#endif // _dynWxControlFactoryBase_H

