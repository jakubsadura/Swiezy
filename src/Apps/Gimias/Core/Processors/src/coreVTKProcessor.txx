/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreVTKProcessor.h"
#include "coreDataEntityHelper.h"
#include "vtkInformation.h"
#include "coreVTKPolyDataHolder.h"
#include "coreVTKImageDataHolder.h"
#include "vtkAlgorithm.h"

template<class T>
T* Core::VTKProcessor<T>::GetFilter( ) {
    return m_Filter;
}


template < class T >
Core::VTKProcessor<T>::VTKProcessor( )
{
	m_Filter = vtkSmartPointer<T>::New();

	SetName( typeid(T).name() );

	SetNumberOfInputs( m_Filter->GetNumberOfInputPorts() );
	for ( size_t i = 0 ; i < GetNumberOfInputs() ; i++ )
	{
		vtkInformation* ipi = m_Filter->GetInputPortInformation( i );
		const char* requiredDataType = ipi->Get(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE());
		if ( strcmp( requiredDataType, "vtkPolyData" ) == 0 )
		{
			GetInputPort( i )->SetName( "Input surface" );
			GetInputPort( i )->SetDataEntityType( Core::SurfaceMeshTypeId);
		}
		else if ( strcmp( requiredDataType, "vtkImageData" ) == 0 )
		{
			GetInputPort( i )->SetName( "Input image" );
			GetInputPort( i )->SetDataEntityType( Core::ImageTypeId);
		}
	}

	SetNumberOfOutputs( m_Filter->GetNumberOfOutputPorts() );
	for ( size_t i = 0 ; i < GetNumberOfOutputs() ; i++ )
	{
		vtkInformation* opi = m_Filter->GetOutputPortInformation( i );
		const char* providedDataType = opi->Get(vtkDataObject::DATA_TYPE_NAME());
		if ( strcmp( providedDataType, "vtkPolyData" ) == 0 )
		{
			SetOutputDataName( i, "Output surface" );
			GetOutputPort( i )->SetDataEntityType( Core::SurfaceMeshTypeId );
			GetOutputPort( i )->SetReuseOutput( false );
			GetOutputPort( i )->SetDataEntityName( typeid(T).name() );
		}
		if ( strcmp( providedDataType, "vtkImageData" ) == 0 )
		{
			SetOutputDataName( i, "Output image" );
			GetOutputPort( i )->SetDataEntityType( Core::ImageTypeId );
			GetOutputPort( i )->SetReuseOutput( false );
			GetOutputPort( i )->SetDataEntityName( typeid(T).name() );
		}
	}

}

template < class T >
void Core::VTKProcessor<T>::Update()
{
	int timeSteps = ComputeNumberOfTimeSteps( );

	for ( int timeStep = 0 ; timeStep < timeSteps ; timeStep++ )
	{
		for ( size_t num = 0 ; num < GetNumberOfInputs() ; num++ )
		{

			switch( GetInputPort( num )->GetDataEntityType( ) )
			{
			case Core::SurfaceMeshTypeId: 
				PassInputData<Core::vtkPolyDataPtr>( num, timeStep );
				break;
			case Core::ImageTypeId: 
				PassInputData<Core::vtkImageDataPtr>( num, timeStep );
				break;
			}

		}

		m_Filter->Update();

		for ( size_t num = 0 ; num < GetNumberOfOutputs() ; num++ )
		{
			UpdateOutput( 
				num, 
				m_Filter->GetOutput( num ), 
				GetOutputPort( num )->GetDataEntityName( ), 
				GetOutputPort( num )->GetReuseOutput( ),
				1,
				GetInputDataEntity( 0 ) );
		}
	}
}

template < class T >
int Core::VTKProcessor<T>::ComputeNumberOfTimeSteps()
{
	size_t timeSteps = 0;

	for ( size_t num = 0 ; num < GetNumberOfInputs() ; num++ )
	{
		Core::DataEntity::Pointer	dataEntity;
		dataEntity = this->GetInputDataEntity( num );

		if ( dataEntity.IsNull( ) )
		{
			throw Core::Exceptions::Exception("DataEntityHelper::GetProcessingData", 
				"You must select an input data from the Processing Browser and set it as input" 
				);
		}

		timeSteps = std::max( timeSteps, dataEntity->GetNumberOfTimeSteps( ) );

	}

	return timeSteps;
}

template<class T>
template<class DataType>
void Core::VTKProcessor<T>::PassInputData( int portNum, int timeStep )
{
	Core::DataEntity::Pointer	dataEntity;
	dataEntity = this->GetInputDataEntity( portNum );
	if ( dataEntity.IsNull( ) )
	{
		throw Core::Exceptions::Exception("DataEntityHelper::GetProcessingData", 
			"You must select an input data from the Processing Browser and set it as input" 
			);
	}

	DataType processingData;
	dataEntity->GetProcessingData( processingData, timeStep, true );
	if( !processingData )
	{
		throw Core::Exceptions::Exception("DataEntityHelper::GetProcessingData", 
			"You must select an input data from the Processing Browser and set it as input" 
			);
	}
	m_Filter->SetInput( processingData );	
}
