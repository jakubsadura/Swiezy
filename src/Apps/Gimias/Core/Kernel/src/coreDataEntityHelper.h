/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDataEntityHelper_H
#define coreDataEntityHelper_H

#include "gmKernelWin32Header.h"
#include "coreObject.h"
#include "coreDataEntity.h"
#include <list>

namespace Core
{
class RenderingTreeManager;

/** 
\brief Provides helper functions for working with DataEntities and to 
retrieve info from them.

This class has dependency on the different data types that can be
stored inside the data entity, like vtkPolyData or itk::Image

\ingroup gmKernel
\author Juan A. Moya
\date 26 Mar 2008
*/
class GMKERNEL_EXPORT DataEntityHelper : public Core::Object
{
public:
	//! Define virtual destructor to avoid warning
	virtual ~DataEntityHelper( );

	/**
	 \brief Open a file as a data entity and add to the data entity list 
	 */
	static Core::DataEntity::Pointer
		OpenFileAsDataEntity( const std::string &pathFileName );

	//! Load data entity from disk
	static Core::DataEntity::Pointer LoadDataEntity( 
		const std::string &pathFileNames );

	//! Load data entity from disk
	static Core::DataEntity::Pointer LoadDataEntity( 
		std::vector< std::string > &pathFileName );

	//! Preload data entity from disk
	static blTagMap::Pointer PreLoadDataEntityVector( 
		std::vector< std::string > &pathFileNames );

	//! Load data entity from disk
	static std::vector<Core::DataEntity::Pointer> LoadDataEntityVector( 
		std::vector< std::string > &pathFileName,
		blTagMap::Pointer tagMap = NULL );

	//! Save data entity from disk
	static void SaveDataEntity( 
		const std::string& fileName,
		Core::DataEntity::Pointer dataEntity,
		Core::RenderingTreeManager* treeManager = NULL );


	/**
	 \brief Add data entity to list and select it
	 \note This also creates a connection between the data holders,
	 when the data entity from the list is removed, the contents
	 of the parameter holder will be set to NULL
	 */
	static void AddDataEntityToList( 
		Core::DataEntityHolder::Pointer dataEntity,
		bool bSetSelected = true );

	/**
	 \brief Add data entity to list and select it
	 */
	static void AddDataEntityToList( 
		Core::DataEntity::Pointer dataEntity,
		bool bSetSelected = true );


	//! Get input data of a data entity and throw exception if error
	template <class ProcessingDataType>
	static void GetProcessingData( 
		Core::DataEntityHolder::Pointer dataEntityHolder,
		ProcessingDataType &processingData,
		int iTimeStep = 0 );


	//! Get input data of a data entity and throw exception if error
	template <class ProcessingDataType>
	static void GetProcessingData( 
		Core::DataEntityHolder::Pointer dataEntityHolder,
		std::vector< ProcessingDataType> &processingDataVector );

				
	/**
	\brief Check that processing data is a surface mesh
	 */
	static bool IsSurfaceMesh(	
		Core::DataEntityHolder::Pointer	inputDataEntityHolder );


	/**
	\brief Check that processing data is a volume image
	 */
	static bool IsImage( 
						Core::DataEntityHolder::Pointer	inputDataEntityHolder );


	//!
	static bool CheckType( 
						Core::DataEntityHolder::Pointer	inputDataEntityHolder,
						Core::DataEntityType type );

	//! Finds a child of a data entity. Return the first match of the child data entity, NULL otherwise
	/*!
	\param fatherDataEntity [in] The data entity in where to look for the specific child
	\param type [in] The type of data entity to look for in data entity's children list
	\return The first match of the child data entity, NULL otherwise
	*/
	static Core::DataEntity::Pointer FindChild(
		DataEntity::Pointer fatherDataEntity,
		Core::DataEntityType type);

	//! Check if the dataEntity is in the list
	static bool IsDataEntityInTheList(DataEntity::Pointer dataEntity);
	
};

}

#include "coreDataEntityHelper.txx"

#endif
