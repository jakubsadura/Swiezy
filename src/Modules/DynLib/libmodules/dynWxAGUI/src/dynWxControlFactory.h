/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynWxControlFactory_H
#define _dynWxControlFactory_H

#include "DynWxAGUILibWin32Header.h"
#include "dynWxControlFactoryBase.h"

/**
Automated GUI Builder for wxWidgets

\author Xavi Planes
\date 15 July 2010
\ingroup DynLib
*/
class DYNWXAGUILIB_EXPORT dynWxControlFactory : public dynWxControlFactoryBase
{
public:
	typedef dynWxControlFactory Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);

	//!
	virtual wxSizer* CreateTagWidget(
		dynBasePanel* panel,
		wxWindow* parent, 
		const ModuleParameter& moduleParameter);

	//!
	virtual wxSizer* CreateHelpWidget( wxWindow* parent, ModuleDescription *module );

private:
	//!
	dynWxControlFactory( );

	//!
	virtual ~dynWxControlFactory( );

	//!
	wxSizer* CreateNumericControlTagWidget( 
		dynBasePanel* panel,
		wxWindow* parent, 
		const ModuleParameter& moduleParameter );

	//!
	wxSizer* CreateTextControlTagWidget( 
		dynBasePanel* panel,
		wxWindow* parent, 
		const ModuleParameter& moduleParameter );

	//!
	wxSizer* CreateSliderControlTagWidget( 
		dynBasePanel* panel,
		wxWindow* parent, 
		const ModuleParameter& moduleParameter );

	//!
	wxSizer* CreateCheckBoxControlTagWidget( 
		dynBasePanel* panel,
		wxWindow* parent, 
		const ModuleParameter& moduleParameter );

	//!
	wxSizer* CreateEnumerationTagWidget( 
		dynBasePanel* panel,
		wxWindow* parent, 
		const ModuleParameter& moduleParameter );

	//!
	wxSizer* CreateFileTagWidget( 
		dynBasePanel* panel,
		wxWindow* parent, 
		const ModuleParameter& moduleParameter );
	
};



#endif // _dynWxControlFactory_H

