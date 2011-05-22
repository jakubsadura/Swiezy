/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreSelectionToolWidget_H
#define _coreSelectionToolWidget_H

#include "gmWidgetsWin32Header.h"
#include "coreBaseWindow.h"

namespace Core{
namespace Widgets{

/**
\brief Selection tool base widget
\ingroup gmWidgets
\author Xavi Planes
\date 25 April 2010
*/

class GMWIDGETS_EXPORT SelectionToolWidget: public BaseWindow {
public:
    SelectionToolWidget( );
	 
	virtual ~SelectionToolWidget( );

	//!
	void Init( );

	//!
	virtual void StartInteractor( ) = 0;

	//!
	virtual void StopInteraction( ) = 0;

	//!
	virtual bool IsSelectionEnabled( ) = 0;

	//! Used by DynProcessingWidget
	virtual boost::any GetData( );

	//! Get DataEntity used to put the new selected DataEntity
	virtual Core::DataEntity::Pointer GetDataEntity( );

protected:
	//!
	virtual void OnInit();

private:

};

}
}

#endif // _coreSelectionToolWidget_H
