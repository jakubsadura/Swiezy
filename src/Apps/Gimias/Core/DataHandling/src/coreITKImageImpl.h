/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreITKImageImpl_H
#define _coreITKImageImpl_H

#include "gmDataHandlingWin32Header.h"
#include "coreDataEntityImplFactory.h"
#include "itkImage.h"
#include <typeinfo>

namespace Core
{
/** 
ITKImageDataEntityBuilder inherits from abstract class DataEntityBuilder 
and enables building DataEntity from ITKImageData 

- "Origin": std::vector<double>
- "Spacing": std::vector<double>
- "Dimensions": std::vector<int>
- "Buffer": void*

\ingroup gmDataHandling
\author: Jakub Lyko
\date: 5.06.2008
*/
template <class ImageType>
class ITKImageImpl : public DataEntityImpl
{
public:

	coreDeclareSmartPointerClassMacro( ITKImageImpl, DataEntityImpl)

	/**
	*/
	class Factory : public DataEntityImplFactory 
	{ 
	public: 
		coreDeclareSmartPointerClassMacro(Factory, DataEntityImplFactory)  
		static DataEntityImplFactory::Pointer NewBase( )
		{
			Pointer p = New( );
			return p.GetPointer( );
		}
		const std::type_info& GetDataType( )
		{
			return typeid( ImageType );
		}
	private: 
		DataEntityImpl::Pointer NewData( )
		{
			typename ITKImageImpl::Pointer data = ITKImageImpl<ImageType>::New( );
			return data.GetPointer( );
		}
		Factory( ) 
		{ 
			m_Properties->AddTag( typeid( typename ImageType::Pointer ).name( ), true );
			m_Properties->AddTag( typeid( ImageType* ).name( ), true );
			m_Properties->AddTag( "single", true );
		}
	};

	//@{ 
	/// \name Interface
public:
	boost::any GetDataPtr() const;
	virtual bool IsValidType( const std::type_info &type );
	virtual bool SetDataPtr( boost::any val );
	virtual bool SetData( blTagMap::Pointer tagMap, ImportMemoryManagementType mem = gmCopyMemory );
	virtual bool GetData( blTagMap::Pointer tagMap );

private:
	virtual void SetAnyData( boost::any val );
	virtual void ResetData( );
	//@}

protected:
	//!
	ITKImageImpl();
	//!
	~ITKImageImpl();

private:

	//!
	typename ImageType::Pointer m_Data;
	//!
	std::string m_ComponentType;
};

}

#include "coreITKImageImpl.txx"

#endif // _coreITKImageImpl_H
