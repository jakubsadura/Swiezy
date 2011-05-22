/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _WorkingAreaFactory_H
#define _WorkingAreaFactory_H

#include "coreRenderWindowContainer.h"

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
class WorkingAreaFactory : public Core::BaseWindowFactory
{
public:
	coreDeclareSmartPointerTypesMacro(WorkingAreaFactory,BaseWindowFactory)
	coreFactorylessNewMacro(WorkingAreaFactory)
	coreClassNameMacro(WorkingAreaFactory)

	static BaseWindowFactory::Pointer NewBase( const std::string &name, blTagMap::Pointer tagMap ) 
	{ 
		WorkingAreaFactory::Pointer factory = New( );
		factory->SetWindowClassname( name );
		factory->SetProperties( tagMap );
		return factory.GetPointer( );
	} 
	virtual Core::BaseWindow* CreateWindow( ) 
	{ 
		RenderWindowContainer* window = new RenderWindowContainer( GetParent(), GetWindowId( ), GetPosition( ), GetSize( ) );
		window->SetFactoryName( GetName( ) );
		window->SetBitmapFilename( GetBitmapFilename( ) );
		if ( !GetWindowName( ).empty( ) ){ 
			wxWindowBase* windowBase = window; 
			windowBase->SetName( GetWindowName( ) ); 
		} 
		window->SetProperties( m_Properties );
		return window; 
	} 
	std::string GetName() const 
	{ 
		return m_Name; 
	} 
	void SetWindowClassname( const std::string &name) 
	{ 
		m_Name = name;
	} 
	blTagMap::Pointer GetProperties() const { return m_Properties; }
	void SetProperties(blTagMap::Pointer val) { m_Properties = val; }

	std::string m_Name;
	blTagMap::Pointer m_Properties;
};

}
}

#endif // WorkingAreaFactory_H
