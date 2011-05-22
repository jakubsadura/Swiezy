/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreVtkPolyDataImpl_H
#define _coreVtkPolyDataImpl_H

#include "gmDataHandlingWin32Header.h"
#include "coreDataEntityImplFactory.h"
#include "coreVTKPolyDataHolder.h"

namespace Core{

/**
Surface time steps for DataEntity

- "Points": std::vector<Point3D>*
- "SurfaceElements": std::vector<SurfaceElement3D>*

\author Xavi Planes
\date 03 nov 2009
\ingroup gmDataHandling
*/
class GMDATAHANDLING_EXPORT VtkPolyDataImpl : public DataEntityImpl
{
public:
	coreDeclareSmartPointerClassMacro( VtkPolyDataImpl, DataEntityImpl )
	
	coreDefineSingleDataFactory2Types( VtkPolyDataImpl, vtkPolyDataPtr, vtkPolyData*, SurfaceMeshTypeId )

	//@{ 
	/// \name Interface
public:
	bool SetDataPtr( boost::any val );
	boost::any GetDataPtr() const;
	virtual void* GetVoidPtr( ) const;
	virtual void SetVoidPtr( void* ptr );
	virtual void CleanTemporalData( );

private:
	virtual void SetAnyData( boost::any val );
	virtual bool SetData( blTagMap::Pointer tagMap, ImportMemoryManagementType mem = gmCopyMemory );
	virtual bool GetData( blTagMap::Pointer tagMap );
	bool IsValidType( const std::type_info &type );
	virtual void ResetData( );
	//@}

private:
	//!
	VtkPolyDataImpl( );

	//!
	virtual ~VtkPolyDataImpl( );

private:

	vtkPolyDataPtr m_Data;

	//! Temporal Data
	std::vector<Point3D> m_Points;

	//! Temporal Data
	std::vector<SurfaceElement3D> m_SurfaceElements;
};

} // end namespace Core

#endif // _coreVtkPolyDataImpl_H

