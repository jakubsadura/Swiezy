/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataEntityHelper.h"
#include "coreReportExceptionMacros.h"
#include "coreLoggerHelper.h"
#include "coreAssert.h"
#include "coreDirectory.h"
#include "coreKernel.h"
#include "coreDataEntityWriter.h"
#include "coreDataEntityReader.h"
#include "coreRenderingTreeManager.h"

#include <sstream>

using namespace Core;

Core::DataEntityHelper::~DataEntityHelper()
{

}


Core::DataEntity::Pointer
DataEntityHelper::OpenFileAsDataEntity( 
						const std::string &pathFileName )
{
	Core::DataEntity::Pointer dataEntity;

	try
	{
		// Load data entity
		dataEntity = Core::DataEntityHelper::LoadDataEntity( pathFileName );

		// Add to data entity list
		if ( dataEntity.IsNotNull() )
		{
			Core::DataEntityHelper::AddDataEntityToList( dataEntity );

			Core::LoggerHelper::ShowMessage(
				"File imported successfully", 
				Core::LoggerHelper::MESSAGE_TYPE_INFO, false);
		}
		
	}
	coreCatchExceptionsReportAndNoThrowMacro(DataEntityHelper::OpenFileAsDataEntity)

	return dataEntity;
}


/**
*/
bool DataEntityHelper::IsSurfaceMesh( 
						Core::DataEntityHolder::Pointer	inputDataEntityHolder )
{
	bool bInputIsASurfaceMesh = false;

	if ( inputDataEntityHolder->GetSubject( ) )
	{
		bInputIsASurfaceMesh = inputDataEntityHolder->GetSubject( )->IsSurfaceMesh();
	}

	return bInputIsASurfaceMesh;
}

/**
*/
bool DataEntityHelper::IsImage( 
						Core::DataEntityHolder::Pointer	inputDataEntityHolder )
{
	bool bInputIsAVolumeImage = false;

	if ( inputDataEntityHolder->GetSubject( ) )
	{
		bInputIsAVolumeImage = inputDataEntityHolder->GetSubject( )->IsImage();
	}

	return bInputIsAVolumeImage;
}


/**
*/
bool DataEntityHelper::CheckType( 
						Core::DataEntityHolder::Pointer	inputDataEntityHolder,
						Core::DataEntityType type )
{
	bool bTypeOk = false;

	if ( inputDataEntityHolder->GetSubject( ) )
	{
		bTypeOk = inputDataEntityHolder->GetSubject( )->GetType() == type;
	}

	return bTypeOk;
}

/**
 */
Core::DataEntity::Pointer
DataEntityHelper::LoadDataEntity( const std::string &pathFileName )
{

	std::vector< std::string > pathFileNames;
	pathFileNames.push_back( pathFileName );
	
	return LoadDataEntity( pathFileNames );
}

/**
 */
Core::DataEntity::Pointer
DataEntityHelper::LoadDataEntity( 
								 std::vector< std::string > &pathFileNames )
{
	Core::DataEntity::Pointer dataEntity;

	// Load the file
	Core::IO::DataEntityReader::Pointer dataEntityReader;
	dataEntityReader = Core::IO::DataEntityReader::New( );

	dataEntityReader->SetFileNames( pathFileNames );
	dataEntityReader->Update( );
	dataEntity = dataEntityReader->GetOutputDataEntity( );

	if(dataEntity.IsNull())
	{
		Core::Exceptions::CannotOpenFileException e("DataEntityHelper::LoadDataEntity");
		e.Append("File was: ");
		e.Append(pathFileNames[0].c_str());
		throw e;
	}
	
	// Set file name
	if ( dataEntity->GetMetadata( )->GetName().empty() )
	{
		dataEntity->GetMetadata( )->SetName(Core::IO::File::GetFilenameName(pathFileNames[0]));
	}

	return dataEntity;
}


blTagMap::Pointer DataEntityHelper::PreLoadDataEntityVector( 
	std::vector< std::string > &pathFileNames )
{
	// Preload the file
	Core::IO::DataEntityReader::Pointer dataEntityReader;
	dataEntityReader = Core::IO::DataEntityReader::New( );
	dataEntityReader->SetFileNames( pathFileNames );
	dataEntityReader->ReadHeader( );
	return dataEntityReader->GetTagMap();
}


/**
*/
std::vector<Core::DataEntity::Pointer>
DataEntityHelper::LoadDataEntityVector( 
	std::vector< std::string > &pathFileNames,
	blTagMap::Pointer tagMap )
{
	std::vector<Core::DataEntity::Pointer> dataEntityVector;

	// Load the file
	Core::IO::DataEntityReader::Pointer dataEntityReader;
	dataEntityReader = Core::IO::DataEntityReader::New( );
	dataEntityReader->SetFileNames( pathFileNames );
	dataEntityReader->SetTagMap( tagMap );
	dataEntityReader->Update( );

	for ( size_t i = 0 ; i < dataEntityReader->GetNumberOfOutputs() ; i++ )
	{
		if( dataEntityReader->GetOutputDataEntity( i ).IsNull() )
		{
			Core::Exceptions::CannotOpenFileException e("DataEntityHelper::LoadDataEntity");
			e.Append("File was: ");
			e.Append(pathFileNames[0].c_str());
			throw e;
		}

		// Set file name
		if ( dataEntityReader->GetOutputDataEntity( i )->GetMetadata( )->GetName().empty() )
		{
			dataEntityReader->GetOutputDataEntity( i )->GetMetadata( )->SetName(
				Core::IO::File::GetFilenameName(pathFileNames[0]));
		}

		dataEntityVector.push_back( dataEntityReader->GetOutputDataEntity( i ) );
	}


	return dataEntityVector;
}

/**
 */
void DataEntityHelper::AddDataEntityToList( 
						Core::DataEntityHolder::Pointer dataEntityHolder,
						bool bSetSelected /*= true*/ )
{
	try
	{
		Core::DataEntity::Pointer dataEntity = dataEntityHolder->GetSubject( );

		if ( dataEntity.IsNotNull() )
		{
			Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
			coreAssertMacro(dataContainer.IsNotNull());
			Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();
			list->Add(dataEntity);

			// Connect the subject to notify when is removed from the data list
			list->ConnectOutputHolder( dataEntityHolder );

			// If properly loaded, auto-select it 
			if ( bSetSelected )
			{
				Core::DataEntityHolder::Pointer holder;
				holder = dataContainer->GetDataEntityList()->GetSelectedDataEntityHolder();
				coreAssertMacro(holder.IsNotNull());

				// In CardiacSegmentationPlugin we need to notify the 
				// QuantificationWidget in order to compute the volume
				// each time the output mesh is modified
				holder->SetSubject( dataEntity, true );
			}
		}
		
	}
	coreCatchExceptionsReportAndNoThrowMacro(DataEntityHelper::AddDataEntityToList)
}


/**
 */
void DataEntityHelper::AddDataEntityToList( 
						Core::DataEntity::Pointer dataEntity,
						bool bSetSelected /*= true*/ )
{
	try
	{
		if ( dataEntity.IsNotNull() )
		{
			Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
			coreAssertMacro(dataContainer.IsNotNull());
			Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();
			list->Add(dataEntity);

			// If properly loaded, auto-select it 
			if ( bSetSelected )
			{
				Core::DataEntityHolder::Pointer holder;
				holder = dataContainer->GetDataEntityList( )->GetSelectedDataEntityHolder();
				coreAssertMacro(holder.IsNotNull());
				holder->SetSubject(dataEntity);
			}
		}
		
	}
	coreCatchExceptionsReportAndNoThrowMacro(DataEntityHelper::AddDataEntityToList)
}


/**
*/
Core::DataEntity::Pointer Core::DataEntityHelper::FindChild(
	DataEntity::Pointer fatherDataEntity,
	Core::DataEntityType type)
{
	try
	{
		Core::DataEntity::Pointer childDataEntity = NULL;
		if(fatherDataEntity.IsNotNull())
		{
			Core::DataEntity::ChildrenListType list = fatherDataEntity->GetChildrenList();
			if (list.size()==0)
				return childDataEntity;
			else
			{
				Core::DataEntity::ChildrenListType::iterator it = list.begin();
				while (it != list.end() )
				{
					if ((*it)->GetType() == type)
					{
						childDataEntity = (*it);
						return childDataEntity;
					}// end if
					it++;
				}//end while
			}//end else

		return childDataEntity;
		} // end if (fatherDataEntity.IsNotNull)
	} // end try
	catch( ... )
	{
		std::cerr << "CoreDataEntityHelper::FindChild exception caught!" << std::endl; 
		return NULL;
	}

	return NULL;
}

void Core::DataEntityHelper::SaveDataEntity( 
	const std::string& fileName, 
	Core::DataEntity::Pointer dataEntity,
	Core::RenderingTreeManager* treeManager )
{
	// Pass rendering data to MetaData before saving the XML
	if ( treeManager )
	{
		std::list<RenderingTree::Pointer> treeList = treeManager->GetTreeList( );
		std::list<RenderingTree::Pointer>::iterator it;
		for ( it = treeList.begin() ; it != treeList.end() ; it++ )
		{
			(*it)->UpdateMetadata( dataEntity );
		}
	}

	Core::IO::DataEntityWriter::Pointer dataEntityWriter;
	dataEntityWriter = Core::IO::DataEntityWriter::New( );
	dataEntityWriter->SetFileName( fileName );
	dataEntityWriter->SetInputDataEntity( 0, dataEntity );
	dataEntityWriter->Update();

}

 bool Core::DataEntityHelper::IsDataEntityInTheList( 
	 DataEntity::Pointer dataEntity ) 
{
	Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
	coreAssertMacro(dataContainer.IsNotNull());
	Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();
	return list->IsInTheList(dataEntity->GetId());
}
