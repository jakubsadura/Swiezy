/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreVtkUnstructuredGridImpl_H
#define _coreVtkUnstructuredGridImpl_H

#include "gmDataHandlingWin32Header.h"
#include "coreDataEntityImplFactory.h"
#include "coreVTKUnstructuredGridHolder.h"

namespace Core{

/**
Surface time steps for DataEntity

- "Points"
- "SurfaceElements"

\author Xavi Planes
\date 03 nov 2009
\ingroup gmDataHandling
*/
class GMDATAHANDLING_EXPORT VtkUnstructuredGridImpl : public DataEntityImpl
{
public:

	typedef vtkUnstructuredGridPtr DataType;

	coreDeclareSmartPointerClassMacro( VtkUnstructuredGridImpl, DataEntityImpl )
	
	coreDefineSingleDataFactory2Types( VtkUnstructuredGridImpl, DataType, vtkUnstructuredGrid*, VolumeMeshTypeId )

	//@{ 
	/// \name Interface
public:
	bool SetDataPtr( boost::any val );
	boost::any GetDataPtr() const;
	virtual void* GetVoidPtr( ) const;
	virtual void SetVoidPtr( void* ptr );

private:
	virtual bool SetData( blTagMap::Pointer tagMap, ImportMemoryManagementType mem = gmCopyMemory );
	virtual bool GetData( blTagMap::Pointer tagMap );
	void SetAnyData( boost::any val );
	bool IsValidType( const std::type_info &type );
	virtual void ResetData( );
	//@}

private:
	//!
	VtkUnstructuredGridImpl( );

	//!
	virtual ~VtkUnstructuredGridImpl( );

private:

	//!
	DataType m_Data;

};

} // end namespace Core

#endif // _coreVtkUnstructuredGridImpl_H

