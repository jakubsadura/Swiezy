/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreBaseWindowFactories_H
#define _coreBaseWindowFactories_H

#include "coreObject.h"
#include "coreBaseWindow.h"
#include "coreWindowConfig.h"
#include "coreBaseWindowFactory.h"

namespace Core
{

	class FactoryData{
	public:
		//!
		BaseWindowFactory::Pointer m_Factory;
		//!
		WindowConfig m_WindowConfig;
	};


/**
\brief Repository of all BaseWindow registered to the framework

\ingroup gmKernel
\sa FrontEndPlugin
\author Xavi Planes
\date 10 April 2010
*/
class GMKERNEL_EXPORT BaseWindowFactories : public Core::SmartPointerObject
{
public:

	//! Insertion order
	typedef std::list<std::string> FactoriesListType;
	//! Map using factory name as key
	typedef std::map<std::string, FactoryData> FactoriesMapType;
	//!
	typedef Core::DataHolder<FactoriesMapType> FactoriesMapHolderType;

public:

	coreDeclareSmartPointerClassMacro(
		BaseWindowFactories, 
		Core::SmartPointerObject);

	//!
	void CleanRegisteredWidgets(  );

	//! Create a registered widget
	Core::BaseWindow* CreateWindow( 
		const std::string &factoryName, 
		wxWindow* parent,
		const wxPoint&  pos = wxDefaultPosition,
		const wxSize&  size = wxDefaultSize );

	//! Create a registered widget
	BaseWindowFactory::Pointer FindFactory( 
		const std::string &factoryName );

	//!
	bool GetWindowConfig( 
		const std::string &factoryName,
		Core::WindowConfig &config );

	//! Return a list of factory names
	std::list<std::string> GetWindowsList(
		WIDGET_TYPE type = WIDGET_TYPE_MAX,
		const std::string &caption = "",
		const std::string &category = "",
		int id = -1,
		WIDGET_STATE state = WIDGET_STATE_MAX );

	//!
	FactoriesMapHolderType::Pointer GetFactoriesHolder();

	//! Register a factory that will be used to create an instance
	void RegisterFactory( 
		BaseWindowFactory::Pointer factory,
		WindowConfig widgetConfig );

protected:
	//!
	BaseWindowFactories( );

	//!
	FactoriesMapType &GetFactoriesMap();

protected:

	//!
	FactoriesMapHolderType::Pointer m_CreatorsMapHolder;

	//!
	FactoriesListType m_CreatorsList;
};

} // namespace Core

#endif // _coreBaseWindowFactories_H

