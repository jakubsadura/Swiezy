/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDataEntityBuilder_H
#define coreDataEntityBuilder_H

#include "gmDataHandlingWin32Header.h"
#include "coreObject.h"
#include "coreDataEntity.h"
#include "coreException.h"

coreCreateRuntimeExceptionClassMacro(
	CannotBuild3DTDataEntity, 
	"3D+T data entity cannot be created",
	GMDATAHANDLING_EXPORT );


namespace Core
{
/** 
\brief DataEntityBuilder is an abstract class. All DataEntity builders 
classes for different data types have to inherit from this class and
implement appropriate build method 

\ingroup gmDataHandling
\author: Jakub Lyko
\date: 5.06.2008
*/

class GMDATAHANDLING_EXPORT DataEntityBuilder : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerTypesMacro(DataEntityBuilder, Core::SmartPointerObject);

	/**
	\brief Build a DataEntity with iTimeSteps

	
	\param [in] processingData Input processing data. If this is NULL, empty
		processing data will be created
	\param [in] iTimeSteps Number of time steps for the DataEntity. It this is
		-1, then the size of the data will be used
	\param [in] bCopyProcessingData Create a copy of the processing data
	\note If data size is < iTimeSteps, then new empty processing data will
		be created to reach the iTimeSteps
	*/
	virtual Core::DataEntity::Pointer Build(
		boost::any& processingData,
		int iTimeSteps = -1,
		bool bCopyProcessingData = false ) = 0;

	//! Get output data entity type that this builder can build
	virtual void GetOutputDataEntityTypes( 
		std::list<Core::DataEntityType> &list ) = 0;

};

}

#endif
