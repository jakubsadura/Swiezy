/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _cmguiRegionImpl_H
#define _cmguiRegionImpl_H

#include "corePluginMacros.h"
#include "coreDataEntityImplFactory.h"
#include "CmguiFactoryMacros.h"

extern "C"
{
#include "api/cmiss_region.h"
}

namespace CmguiPlugin{

/**

Data entity for CMGUI region ID

\note You need to set the name of the DataEntity before creating this object
because the name will be used to set region name

\author Xavi Planes
\date Oct 2010
\ingroup CMGUIPlugin
*/
class PLUGIN_EXPORT CmguiRegionImpl : public Core::DataEntityImpl
{
public:
	typedef Cmiss_region_id DataType;

public:
	coreDeclareSmartPointerClassMacro2Param( CmguiRegionImpl, Core::DataEntityImpl, Cmiss_region *, std::string );

	coreDefineCMGUIDataFactory( CmguiRegionImpl, DataType, Core::SurfaceMeshTypeId );


	/** Iterate over all elements and check if it's a surface mesh or
	a volumetric mesh (contains at least one volumetric element)
	*/
	static bool IsSurfaceMesh( Cmiss_region *region );

	//@{ 
	/// \name Interface
public:
	boost::any GetDataPtr() const;

private:
	virtual bool SetData( blTagMap::Pointer tagMap, Core::ImportMemoryManagementType mem = Core::gmCopyMemory );
	virtual bool GetData( blTagMap::Pointer tagMap );
	virtual void SetAnyData( boost::any val );
	virtual void ResetData( );
	//@}


protected:
	//!
	CmguiRegionImpl( Cmiss_region *rootRegion, const std::string &name );

	//!
	virtual ~CmguiRegionImpl();

	//! Not implemented
	CmguiRegionImpl(const Self&);

	//! Not implemented
   	void operator=(const Self&);

	//!
	void DestroyRegion( DataType &region );

private:

	//!
	DataType m_Data;

	//!
	Cmiss_region *m_RootRegion;

	//!
	std::string m_Name;

	//! Temporal Data
	std::vector<Point3D> m_Points;

	//! Temporal Data
	std::vector<SurfaceElement3D> m_SurfaceElements;
};


}

#endif //_cmguiRegionImpl_H
