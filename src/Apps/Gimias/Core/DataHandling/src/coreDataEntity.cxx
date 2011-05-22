/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifdef WIN32 
#pragma warning(push)
#pragma warning(disable: 4675)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#endif

#include "coreDataEntity.h"
#include "coreException.h"
#include "coreAssert.h"
#include "coreMultipleDataEntityImpl.h"

using namespace Core;

// ---------------------------------------------------------------------------
unsigned int DataEntity::dataEntityIdCount = 0;

const bool ENABLE_DATAENTITY_DEBUG = false;

// ---------------------------------------------------------------------------


/** Initializer for the class DataEntity 
*/
DataEntity::DataEntity(
	DataEntityType type /*= UnknownTypeId*/, 
	ModalityType modality /*= UnknownModality*/)
{
	m_Id = ++DataEntity::dataEntityIdCount;
	m_Metadata = DataEntityMetadata::New( m_Id );
	m_Metadata->SetName( "Unnamed" );
	m_Metadata->SetModality( modality );
	SetType( type );
	m_Father = NULL;
	m_ChildrenList.clear();
	m_DataEntityImpl = MultipleDataEntityImpl::New();
}

/** Destructor for the class DataEntity */
DataEntity::~DataEntity(void)
{
	NotifyObserversOfDestruction();

	if ( ENABLE_DATAENTITY_DEBUG )
	{
		std::cout << "Destroying: " << GetId( );
	}

	//If has a father, remove the reference. If there are children, delete them
	if (GetFather().IsNotNull())
	{
		GetFather()->RemoveChildFromList(GetId());
	}
	if ( ENABLE_DATAENTITY_DEBUG )
	{
		std::cout << std::endl;
	}


	// For each children, remove the father
	// All the elements of the vector are dropped: their destructors are called,
	// and then they are removed from the vector container
	while (m_ChildrenList.size())
	{
		m_ChildrenList.at( 0 )->SetFather( NULL );
	}
    
	// first destroy the rendering data because it can reuse the processing 
	// data buffer
	m_RenderingDataList.clear();

	// Processing data
	ResetTimeSteps(); 
}

/** Notifies observers OnDestroy signal, being cast when this dataentity is forced to unregister and destroy */
void DataEntity::NotifyObserversOfDestruction(void) const
{
	// Calling the function call operator may invoke undefined behavior if no slots are connected
	// to the signal, depending on the combiner used, so check it was connected to a slot beforehand
	if(!m_OnDestroyedSignal.empty())
		m_OnDestroyedSignal(const_cast<Core::DataEntity*>(this));
}

bool DataEntity::IsImage() const
{
	return ( GetType() & ImageTypeId ) != 0;
}

bool Core::DataEntity::IsSurfaceMesh() const
{
	return ( GetType() & SurfaceMeshTypeId ) != 0;
}

bool Core::DataEntity::IsVolumeMesh() const
{
	return ( GetType() & VolumeMeshTypeId ) != 0;
}

bool Core::DataEntity::IsSkeletonMesh() const
{
	return ( GetType() & SkeletonTypeId ) != 0;
}

bool Core::DataEntity::IsROI() const
{
	return ( GetType() & ROITypeId ) != 0;
}

bool Core::DataEntity::IsMeasurement() const
{
	return ( GetType() & MeasurementTypeId ) != 0;
}

bool Core::DataEntity::IsPointSet() const
{
	return ( GetType() & PointSetTypeId ) != 0;
}

bool Core::DataEntity::IsSignal() const
{
	return ( GetType() & SignalTypeId ) != 0;
}

bool Core::DataEntity::IsContour() const
{
	return ( GetType() & ContourTypeId ) != 0;
}

bool Core::DataEntity::IsNumericData() const
{
	return ( GetType() & NumericDataTypeId ) != 0;
}

bool Core::DataEntity::IsVectorField() const
{
	return ( GetType() & VectorFieldTypeId ) != 0;
}

bool Core::DataEntity::IsTransform() const
{
	return ( GetType() & TransformId ) != 0;
}

bool Core::DataEntity::IsMITKData() const
{
	return ( GetType() & MITKDataTypeId ) != 0;
}

bool Core::DataEntity::IsTensor() const
{
	return ( GetType() & TensorTypeId ) != 0;
}

/**
*/
void DataEntity::SetTimeForAllTimeSteps (std::vector <float>& timesteps)
{
	for (unsigned i=0; i<m_DataEntityImpl->GetSize(); i++)
	{
		SetTimeAtTimeStep(i, timesteps.at(i));
	}
}

/**
*/
int Core::DataEntity::GetId() const
{
	return m_Id;
}

/**
*/
Core::DataEntityType Core::DataEntity::GetType() const
{
	return m_Type;
}

/**
*/
void Core::DataEntity::SetType(Core::DataEntityType type)
{
	m_Type = type;
	GetMetadata()->AddTag( "Type", int( type ) );
}

/**
*/
boost::any Core::DataEntity::GetRenderingData( const std::type_info& type )
{
	std::list<boost::any>::iterator it = FindRenderingData( type );
	if ( it != m_RenderingDataList.end() )
	{
		return *it;
	}

	return NULL;
}

/**
*/
void Core::DataEntity::SetRenderingData( boost::any x )
{
	std::list<boost::any>::iterator it = FindRenderingData( x.type() );
	if ( it == m_RenderingDataList.end() )
	{
		m_RenderingDataList.push_back( x );
	}
	else
	{
		*it = x;
	}
}

/**
*/
size_t Core::DataEntity::GetNumberOfTimeSteps() const
{
	return m_DataEntityImpl->GetSize();
}

/**
*/
double Core::DataEntity::GetTimeAtTimeStep( TimeStepIndex timeStep ) const
{
	return m_DataEntityImpl->GetAt( timeStep )->GetRealTime();
}

/**
*/
void Core::DataEntity::SetTimeAtTimeStep( TimeStepIndex timeStep, double timeVal )
{
	m_DataEntityImpl->GetAt(timeStep)->SetRealTime(timeVal);
}

/**
*/
void Core::DataEntity::RegisterChild(Core::DataEntity* child) 
{
	if ( ENABLE_DATAENTITY_DEBUG )
	{
		std::cout 
			<< "DataEntity: " << GetId()
			<< "RegisterChild: " << child->GetId()
			<< std::endl;
	}

	// Find if it is yet inside the list
	ChildrenListType::iterator it;
	it = std::find( m_ChildrenList.begin(), m_ChildrenList.end(), child );
	if ( it != m_ChildrenList.end() )
	{
		throw Core::Exceptions::Exception( 
			"RegisterChild", 
			"Children is already in the list" );
	}

	// Check that the child is not it self
	if ( child->GetId() == this->GetId() )
	{
		throw Core::Exceptions::Exception( 
			"RegisterChild", 
			"A child cannot be itself" );
	}

	// Add to the list
	m_ChildrenList.push_back( child );
}

/**
*/
bool Core::DataEntity::SetFather(Core::DataEntity* father)
{
	// Remove the children from the previous father
	if (m_Father != NULL )
	{
		m_Father->RemoveChildFromList(this->GetId());
	}
	
	m_Father = father;

	// Register the children
	if ( m_Father != NULL )
	{
		m_Father->RegisterChild(this);
	}

	return true;
}

/**
*/
Core::DataEntity::Pointer Core::DataEntity::GetFather()
{
	return m_Father;
}

/**
*/
bool Core::DataEntity::RemoveChildFromList( int coreDataEntityId )
{
	ChildrenListType::iterator it = m_ChildrenList.begin();
	
	if ( ENABLE_DATAENTITY_DEBUG )
	{
		std::cout << " RemoveChildFromList."
			<< " ID: " << GetId( )
			<< " Size: " << m_ChildrenList.size();
	}

	while (it != m_ChildrenList.end())
	{
		if ((*it)->GetId() == coreDataEntityId )
		{
			m_ChildrenList.erase(it);
			if ( ENABLE_DATAENTITY_DEBUG )
			{
				std::cout << " Removed " << coreDataEntityId;
				std::cout << " Size: " << m_ChildrenList.size();
			}
			return true;
		}
		it++;
	}
	return false;
}

/**
*/
Core::DataEntity::ChildrenListType Core::DataEntity::GetChildrenList()
{
	return m_ChildrenList;
}

Core::DataEntityMetadata::Pointer Core::DataEntity::GetMetadata() const
{
	return m_Metadata;
}

std::string Core::DataEntity::operator*( void )
{
	return GetMetadata()->GetName();
}

bool DataEntity::AddTimeStep(boost::any processingDataObj, RealTime realTime)
{
	DataEntityImplFactory::Pointer factory;
	factory = DataEntityImplFactory::FindFactory( processingDataObj.type() );
	if ( factory.IsNull() )
	{
		return false;
	}

	// Add tag "Name" that is used by CMGUI impl
	factory->GetProperties()->AddTags( GetMetadata().GetPointer() );

	// Create new DataEntityImpl
	DataEntityImpl::Pointer newData = factory->NewData();
	newData->SetDataPtr( processingDataObj );
	newData->SetRealTime( realTime );

	// Try to add a single time step
	bool success = false;
	if ( factory->GetProperties()->FindTagByName( "single" ) )
	{
		success = m_DataEntityImpl->SetAt( m_DataEntityImpl->GetSize(), newData );
	}

	// Failed because it's multiple or the type is not correct
	if ( !success )
	{
		// Add a multiple data type only if it's empty
		if ( factory->GetProperties()->FindTagByName( "multiple" ) && 
			GetNumberOfTimeSteps() == 0 )
		{
			m_DataEntityImpl = newData;
			success = true;
		}
	}

	if ( !success )
	{
		return false;
	}

	// Data entity modified to update rendering representation
	Modified();

	return true;
}

/**
*/
void DataEntity::SetTimeStep(
							 boost::any processingDataObj, 
							 TimeStepIndex timeStep,
							 RealTime realTime )
{
	DataEntityImplFactory::Pointer factory;
	factory = DataEntityImplFactory::FindFactory( processingDataObj.type() );
	if ( factory.IsNull() )
	{
		return;
	}

	// Add tag "Name" that is used by CMGUI impl
	factory->GetProperties()->AddTags( GetMetadata().GetPointer() );

	DataEntityImpl::Pointer data = factory->NewData();
	data->SetDataPtr( processingDataObj );
	data->SetRealTime( realTime );
	m_DataEntityImpl->SetAt( timeStep, data );

	Modified();
}

bool Core::DataEntity::SwitchImplementation( const std::type_info &type )
{
	if ( m_DataEntityImpl->IsValidType( type ) )
	{
		return true;
	}

	DataEntityImplFactory::Pointer factory;
	factory = DataEntityImplFactory::FindFactory( type );
	if ( factory.IsNull() )
	{
		return false;
	}

	// Add tag "Name" that is used by CMGUI impl
	factory->GetProperties()->AddTags( GetMetadata().GetPointer() );

	// Create new DataEntityImpl
	if ( factory->GetProperties()->FindTagByName( "multiple" ) )
	{
		DataEntityImpl::Pointer newData = factory->NewData();
		newData->DeepCopy( m_DataEntityImpl );
		m_DataEntityImpl = newData;
		m_RenderingDataList.clear();
		Modified();
		return true;
	}
	else if ( factory->GetProperties()->FindTagByName( "single" ) )
	{
		// Type matches for the time steps ?
		if ( m_DataEntityImpl->GetSize() &&
			 m_DataEntityImpl->GetAt( 0 )->GetDataPtr().type() == type )
		{
			return true;
		}
		else 
		{
			// Switch to multiple
			MultipleDataEntityImpl::Pointer multiple;
			multiple = dynamic_cast<MultipleDataEntityImpl*> ( m_DataEntityImpl.GetPointer() );
			if ( multiple.IsNull() )
			{
				multiple = MultipleDataEntityImpl::New();
				multiple->SwitchImplementation( type );
				multiple->DeepCopy( m_DataEntityImpl );
			}
			else
			{
				multiple->SwitchImplementation( type );
			}
			m_DataEntityImpl = multiple;
			m_RenderingDataList.clear();
			Modified();
			return true;
		}

	}

	return false;
}

/**
*/
boost::any DataEntity::GetProcessingData(TimeStepIndex timeStep)
{
	DataEntityImpl::Pointer data = m_DataEntityImpl->GetAt( timeStep );
	return data->GetDataPtr();
}

Core::DataEntityImpl::Pointer Core::DataEntity::GetTimeStep(
	int timeStepIndex /*= -1*/ )
{
	if ( timeStepIndex == -1 )
	{
		return m_DataEntityImpl;
	}
	return m_DataEntityImpl->GetAt( timeStepIndex );
}


Core::TimeStepIndex Core::DataEntity::FindTimeStep( Core::RealTime time )
{
	Core::TimeStepIndex timeStep = 0;
	bool found = false;
	while ( !found && timeStep < GetNumberOfTimeSteps() )
	{
		if ( time < GetTimeAtTimeStep( timeStep ) )
		{
			found = true;
		}
		else
		{
			timeStep++;
		}
	}

	return timeStep;
}

bool Core::DataEntity::Resize( int size, const std::type_info &type )
{
	DataEntityImplFactory::Pointer factory;
	factory = DataEntityImplFactory::FindFactory( type );
	if ( factory.IsNull() )
	{
		return false;
	}

	// Add tag "Name" that is used by CMGUI impl
	factory->GetProperties()->AddTags( GetMetadata().GetPointer() );

	if ( factory->GetProperties()->FindTagByName( "single" ) )
	{
		MultipleDataEntityImpl::Pointer multiple = MultipleDataEntityImpl::New();
		for ( int i = 0 ; i < size ; i++ )
		{
			multiple->SetAt( i, factory->NewData() );
		}
		m_DataEntityImpl = multiple;
	}
	else if ( factory->GetProperties()->FindTagByName( "multiple" ) )
	{
		DataEntityImpl::Pointer multiple = factory->NewData();
		multiple->SetSize( size );
		m_DataEntityImpl = multiple;
	}

	return true;
}

std::list<boost::any>::iterator Core::DataEntity::FindRenderingData( const std::type_info& type )
{
	std::list<boost::any>::iterator it;
	for ( it = m_RenderingDataList.begin() ; it != m_RenderingDataList.end() ; it++ )
	{
		if ( it->type() == type )
		{
			return it;
		}
		else if ( it->type( ) == typeid( DataEntity::Pointer ) )
		{
			DataEntity::Pointer dataEntity;
			CastAnyProcessingData( *it, dataEntity );

			DataEntityImpl::Pointer impl;
			impl = dataEntity->GetTimeStep( 0 );

			if ( impl.IsNotNull() && impl->GetDataPtr( ).type() == type )
			{
				return it;
			}
		}
	}

	return it;
}

/**
*/
void DataEntity::ResetTimeSteps()
{
	m_DataEntityImpl->ResetData();
}

bool DataEntity::Copy( 
	const DataEntity::Pointer& de, ImportMemoryManagementType mem /*= gmCopyMemory*/ )
{

	bool success = m_DataEntityImpl->DeepCopy( de->m_DataEntityImpl, mem );

	// Data entity modified to update rendering representation
	if ( success )
	{
		Modified();
	}

	return success;
}



#ifdef WIN32 
#pragma warning(pop)
#endif

/**
*/
namespace TestCompilation
{
	void Test()
	{
		double processingData;
		Core::DataEntity::Pointer p;
		p->GetProcessingData(processingData, 0);
	}
}

