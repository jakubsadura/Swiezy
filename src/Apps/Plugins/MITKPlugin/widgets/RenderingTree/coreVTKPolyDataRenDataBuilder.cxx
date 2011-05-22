/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreVTKPolyDataRenDataBuilder.h"
#include "blVTKHelperTools.h"
#include <vtkPolyData.h>
#include <mitkSurface.h>
#include "coreVTKPolyDataHolder.h"
#include "coreDataEntity.h"
#include "coreVTKPolyDataImpl.h"

using namespace Core;

//!
VTKPolyDataRenDataBuilder::VTKPolyDataRenDataBuilder(void)
{
}

//!
VTKPolyDataRenDataBuilder::~VTKPolyDataRenDataBuilder(void)
{
}

//!
void VTKPolyDataRenDataBuilder::Update( )
{

	//! Check processing data
	Core::DataEntity::Pointer dataEntity = GetInputDataEntity( 0 );

	if ( !CheckInput( ) )
	{
		return;
	}


	mitk::BaseData::Pointer mitkData;
	boost::any anyData = dataEntity->GetRenderingData( typeid( mitkData ) );
	Core::CastAnyProcessingData( anyData, mitkData );
	mitk::Surface::Pointer mitkSurface = dynamic_cast<mitk::Surface*> ( mitkData.GetPointer() );
	

	// Create rendering data
	if ( mitkSurface.IsNull() )
	{
		mitkSurface = mitk::Surface::New();
	}
	// commented out to reflect the changes of the new version of mitk 0.12.2
	//mitkSurface->Resize( dataEntity->GetNumberOfTimeSteps() );
	mitkSurface->Expand( dataEntity->GetNumberOfTimeSteps() );

	// For all time steps
	for ( unsigned iTimeStep = 0 ; iTimeStep < dataEntity->GetNumberOfTimeSteps() ; iTimeStep++ )
	{
		// Be careful with this code. If you create a new rendering data
		// The observers to the data entity needs to update the node in the
		// rendering tree with SetData function each time this function is called
		vtkPolyDataPtr meshPtr;
		dataEntity->GetProcessingData( meshPtr, iTimeStep, false );

		// If the implementation is not VtkPolyData
		if ( meshPtr.GetPointer() == NULL )
		{
			VtkPolyDataImpl::Pointer polyDataTimeStep;
			polyDataTimeStep = VtkPolyDataImpl::New( );
			polyDataTimeStep->DeepCopy( dataEntity->GetTimeStep( iTimeStep ) );
			CastAnyProcessingData( polyDataTimeStep->GetDataPtr( ), meshPtr );
		}

		mitkSurface->SetVtkPolyData( meshPtr, iTimeStep );

	}
		
	dataEntity->SetRenderingData( mitk::BaseData::Pointer(mitkSurface) );
}

bool Core::VTKPolyDataRenDataBuilder::CheckProcessingDataIsOk( 
	const Core::DataEntityImpl::Pointer timeStep )
{
	vtkPolyDataPtr meshPtr;
	bool worked = CastAnyProcessingData( timeStep->GetDataPtr(), meshPtr ); 
	return worked;
}
