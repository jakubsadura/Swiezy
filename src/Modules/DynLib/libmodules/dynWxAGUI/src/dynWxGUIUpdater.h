/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynWxGUIUpdater_H
#define _dynWxGUIUpdater_H

#include "DynWxAGUILibWin32Header.h"
#include "ModuleDescription.h"

#include <list>

#include "gblWxBridgeLib.h"

#include "blLightObject.h"
#include "blSmartPointer.h"

class wxPanel;
class wxWindow;
class wxSizer;


/**
GUI Updater from ModuleDescription to GUI and vice versa

\author Xavi Planes
\date 15 July 2010
\ingroup DynLib
*/
class DYNWXAGUILIB_EXPORT dynWxGUIUpdater : public blLightObject
{
public:
	typedef dynWxGUIUpdater Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	typedef std::map<std::string,gbl::ControlUpdaterBase*> ControlUpdaterMapType;

	//!
	ModuleDescription *GetModule() const;
	void SetModule(ModuleDescription* val);

	//!
	void UpdateData( );

	//!
	void UpdateWidget( );

	//!
	void AddControlUpdater( gbl::ControlUpdaterBase* controlUpdater );

	//!
	gbl::ControlUpdaterBase* FindUpdater( const std::string &name );

	//!
	ControlUpdaterMapType &GetControlUpdaters();

protected:

	//!
	dynWxGUIUpdater( );

	//!
	virtual ~dynWxGUIUpdater( );


private:

	//!
	ModuleDescription *m_Module;

	/// List of wrapper allowing to update the Module
	ControlUpdaterMapType m_ControlUpdaters;
};



#endif // _dynWxGUIUpdater_H

