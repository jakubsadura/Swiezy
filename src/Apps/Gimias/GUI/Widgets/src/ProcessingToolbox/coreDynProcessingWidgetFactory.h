/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _DynProcessingWidgetFactory_H
#define _DynProcessingWidgetFactory_H

#include "coreDynProcessingWidget.h"

namespace Core{
namespace Widgets{

/**
\brief Dynamic processing widget Factory 
Specific factory that uses internal name instead of class name
to identify it
\ingroup gmWidgets
\author Xavi Planes
\date 15 July 2010
*/
class DynProcessingWidgetFactory : public Core::BaseWindowFactory
{
public:
	coreDeclareSmartPointerTypesMacro(DynProcessingWidgetFactory,BaseWindowFactory)
	coreFactorylessNewMacro(DynProcessingWidgetFactory)
	coreClassNameMacro(DynProcessingWidgetFactory)

	static BaseWindowFactory::Pointer NewBase( const ModuleDescription &module ) 
	{ 
		DynProcessingWidgetFactory::Pointer factory = New( );
		factory->SetModule( module );
		return factory.GetPointer( );
	} 
	virtual Core::BaseWindow* CreateWindow( ) 
	{ 
		DynProcessingWidget* window = new DynProcessingWidget( GetParent(), GetWindowId( ), GetPosition( ), GetSize( ) );
		window->SetFactoryName( GetName( ) );
		if ( !GetWindowName( ).empty( ) ){ 
			wxWindowBase* windowBase = window; 
			windowBase->SetName( GetWindowName( ) ); 
		} 
		window->SetModule( &m_Module );
		return window; 
	} 
	std::string GetName() const 
	{ 
		return m_Module.GetTitle( ); 
	} 

};

}
}

#endif // DynProcessingWidgetFactory_H
