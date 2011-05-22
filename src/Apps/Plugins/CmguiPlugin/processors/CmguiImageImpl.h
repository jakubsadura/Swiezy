/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CmguiImageImpl_H
#define _CmguiImageImpl_H

#include "corePluginMacros.h"
#include "coreDataEntityImplFactory.h"
#include "CmguiImage3D.h"
#include "CmguiFactoryMacros.h"

namespace CmguiPlugin{

/**

Data entity for CMGUI 3D+T Image

\note You need to set the name of the DataEntity before creating this object
because the name will be used to set region name

\author Xavi Planes
\date Nov 2010
\ingroup CMGUIPlugin
*/
class PLUGIN_EXPORT CmguiImageImpl : public Core::DataEntityImpl
{
public:
	typedef CmguiImage3D::Pointer DataType;

public:
	coreDeclareSmartPointerClassMacro2Param( CmguiImageImpl, Core::DataEntityImpl, Cmiss_region *, std::string );

	coreDefineCMGUIDataFactory( CmguiImageImpl, DataType, Core::ImageTypeId );

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

	//!
	bool SetDataFromSliceImage( blTagMap::Pointer tagMap, Core::ImportMemoryManagementType mem );
	
	//!
	bool SetDataFrom3DImage( blTagMap::Pointer tagMap, Core::ImportMemoryManagementType mem );

protected:
	//!
	CmguiImageImpl( Cmiss_region *rootRegion, const std::string &name );

	//!
	virtual ~CmguiImageImpl();

	//! Not implemented
	CmguiImageImpl(const Self&);

	//! Not implemented
   	void operator=(const Self&);

	//!
	void DestroyImage3D( CmguiImage3D::Pointer image );

	//!
	void DestroyImage2D( CmguiImage2D::Pointer image );

protected:

	//!
	DataType m_Data;

	//!
	Cmiss_region *m_RootRegion;

	//!
	std::string m_Name;
};


}

#endif //_CmguiImageImpl_H
