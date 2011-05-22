/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _TavernaPluginModuleConverter_H
#define _TavernaPluginModuleConverter_H

#include "coreObject.h"
#include "wflWorkflow.h"
#include "ModuleDescription.h"

namespace TavernaPlugin{

/** 

Convert wflWorkflow to ModuleDescription

\ingroup TavernaPlugin
\author Xavi Planes
\date 23 Dec 2010
*/
class ModuleConverter : public Core::SmartPointerObject
{
public:
	//!
	coreDeclareSmartPointerClassMacro(ModuleConverter, Core::SmartPointerObject)
	
	//! 
	void Update( );

	//!
	void SetWorkflow(wflWorkflow::Pointer val);

	//!
	ModuleDescription GetModule() const;

private:
	//!
	ModuleConverter();

	//!
	~ModuleConverter();

	//! Purposely not implemented
	ModuleConverter( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );

private:
	//!
	wflWorkflow::Pointer m_Workflow;

	//!
	ModuleDescription m_Module;
};
    
} // namespace TavernaPlugin{

#endif //_TavernaPluginModuleConverter_H
