/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreVtkImageDataImpl_H
#define _coreVtkImageDataImpl_H

#include "gmDataHandlingWin32Header.h"
#include "coreDataEntityImplFactory.h"
#include "coreVTKImageDataHolder.h"

namespace Core{

/**
Stores a vtkSmartPointer<vtkImageData>

- "Origin": std::vector<double>
- "Spacing": std::vector<double>
- "Dimensions": std::vector<int>
- "ScalarType": std::string
- "ScalarPointer": void*
- "ScalarSizeInBytes": size_t
- "NumberOfComponents": int

\author Xavi Planes
\date 08 Sept 2010
\ingroup gmDataHandling
*/
class GMDATAHANDLING_EXPORT VtkImageDataImpl : public DataEntityImpl
{
public:
	typedef vtkSmartPointer<vtkImageData> DataType;

	coreDeclareSmartPointerClassMacro( VtkImageDataImpl, DataEntityImpl )
	
	coreDefineSingleDataFactory2Types( VtkImageDataImpl, DataType, vtkImageData*, ImageTypeId )

	//@{ 
	/// \name Interface
public:
	bool SetDataPtr( boost::any val );
	boost::any GetDataPtr() const;
	virtual void* GetVoidPtr( ) const;
	virtual void SetVoidPtr( void* ptr );

private:
	virtual void SetAnyData( boost::any val );
	virtual bool SetData( blTagMap::Pointer tagMap, ImportMemoryManagementType mem = gmCopyMemory );
	virtual bool GetData( blTagMap::Pointer tagMap );
	bool IsValidType( const std::type_info &type );
	virtual void ResetData( );
	//@}

private:
	//!
	VtkImageDataImpl( );

	//!
	virtual ~VtkImageDataImpl( );

private:

	DataType m_Data;

};

} // end namespace Core

#endif // _coreVtkImageDataImpl_H

