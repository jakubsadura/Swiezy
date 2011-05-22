/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CmguiMaterial_H
#define _CmguiMaterial_H

#include "coreObject.h"
#include "CmguiPluginCmgui.h"

extern "C"
{
#include "api/cmiss_material.h"
}

namespace CmguiPlugin{

/**
Several rendering trees can reuse the same material. We need to count
the number of references and destroy it when it's 0

\author Xavi Planes
\date Nov 2010
\ingroup CMGUIPlugin
*/
class CmguiMaterial : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro3Param(CmguiMaterial, Core::SmartPointerObject, 
		Cmgui *, const std::string &, struct Cmgui_image *);

	typedef std::map<std::string, CmguiMaterial*> MaterialMapType;

	//!
	static CmguiMaterial* FindMaterial( const std::string &regionName );

	//! Increase reference count by 1
	static void RegisterMaterial( const std::string &regionName, CmguiMaterial* material );

	//! Decrease reference count by 1. And destroy if it's 0
	static void UnRegisterMaterial( const std::string &regionName );

	//! This function doesn't increase access_count of Cmiss_material_id
	Cmiss_material_id GetMaterialId() const;

private:
	//!
	CmguiMaterial( 
		Cmgui *cmgui, const std::string &regionName, struct Cmgui_image *cmgui_image );

	//!
	~CmguiMaterial( );

private:
	//! region name and material Id
	static MaterialMapType m_MaterialMap;
	//!
	Cmgui *m_cmgui;
	//!
	std::string m_RegionName;
	//!
	Cmiss_material_id m_MaterialId;
};

}

#endif //_CmguiMaterial_H
