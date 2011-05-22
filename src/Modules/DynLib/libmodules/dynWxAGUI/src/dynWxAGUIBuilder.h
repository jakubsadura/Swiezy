/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynWxAGUIBuilder_H
#define _dynWxAGUIBuilder_H

#include "DynWxAGUILibWin32Header.h"
#include "ModuleDescription.h"
#include "dynWxGUIUpdater.h"
#include "dynBasePanel.h"
#include "dynWxControlFactory.h"

class wxPanel;
class wxWindow;
class wxSizer;

/**
Automated GUI Builder for wxWidgets

\author Xavi Planes
\date 15 July 2010
\ingroup DynLib
*/
class DYNWXAGUILIB_EXPORT dynWxAGUIBuilder
{
public:
	//!
	dynWxAGUIBuilder( );

	//!
	virtual ~dynWxAGUIBuilder( );

	//!
	ModuleDescription *GetModule() const;
	void SetModule(ModuleDescription *val);

	//!
	wxWindow* GetParentWindow() const;
	void SetParentWindow(wxWindow* val);

	//!
	void Update( );

	//!
	dynBasePanel* GetPanel() const;

	//!
	void SetControlFactory(dynWxControlFactoryBase::Pointer val);

	//!
	void RemoveEmptyGroups( );

protected:

	//!
	void AddHelp( );

	//!
	void AddParameterGroups( );

	//!
	void AddApplyButton( );

	//!
	void AddAdvancedOptions( );

	//!
	void AddParameterGroup( const ModuleParameterGroup &parameterGroup );

	//!
	void AddParameter( 
		wxWindow* parent, 
		const ModuleParameter& moduleParameter );

private:

	//!
	ModuleDescription *m_Module;

	//!
	dynBasePanel* m_Panel;

	//!
	wxWindow* m_ParentWindow;

	//!
	dynWxControlFactoryBase::Pointer m_ControlFactory;
};



#endif // _dynWxAGUIBuilder_H

