/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreNetgenTimeStepImpl_H
#define _coreNetgenTimeStepImpl_H

#include "corePluginMacros.h"
#include "coreDataEntityImplFactory.h"
#include "meNetgenMesh.h"

namespace Core{


/**
Surface implementation for Netgen class

- "Points": std::vector<Point3D>*
- "SurfaceElements": std::vector<SurfaceElement3D>*

\author Xavi Planes
\date 03 nov 2009
\ingroup MeshEditorPlugin
*/
class PLUGIN_EXPORT NetgenImpl : public DataEntityImpl
{
public:
	coreDeclareSmartPointerClassMacro( NetgenImpl, DataEntityImpl )

	coreDefineSingleDataFactory( NetgenImpl, meNetgenMesh::Pointer, SurfaceMeshTypeId )
	//@{ 
	/// \name Interface
public:
	boost::any GetDataPtr() const;
	virtual void CleanTemporalData( );
private:
	virtual void SetAnyData( boost::any val );
	virtual bool SetData( blTagMap::Pointer tagMap, ImportMemoryManagementType mem = gmCopyMemory );
	virtual bool GetData( blTagMap::Pointer tagMap );
	virtual void ResetData( );
	//@}

private:
	//!
	NetgenImpl( );

	//!
	virtual ~NetgenImpl( );

private:

	//!
	meNetgenMesh::Pointer m_NetgenMesh;

	//! Temporal Data
	std::vector<Point3D> m_Points;

	//! Temporal Data
	std::vector<SurfaceElement3D> m_SurfaceElements;

};

} // end namespace Core

#endif // _coreNetgenTimeStepImpl_H

