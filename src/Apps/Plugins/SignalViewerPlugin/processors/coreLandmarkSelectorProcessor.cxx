/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreLandmarkSelectorProcessor.h"
#include "coreDataContainer.h"
#include "coreDataEntityHolder.h"
#include "coreReportExceptionMacros.h"
#include "blSignalCollective.h"
#include "coreDataEntityHelper.h"
#include "coreVTKPolyDataHolder.h"

#include "vtkPointData.h"

using namespace Core;

//!
LandmarkSelectorProcessor::LandmarkSelectorProcessor(void)
{
	SetName( "LandmarkSelectorProcessor" );
	SetNumberOfInputs(1);
	GetInputPort( 0 )->SetName( "Input" );
	SetNumberOfOutputs(1);
	GetOutputPort( 0 )->SetName( "Output" );
}

//!
LandmarkSelectorProcessor::~LandmarkSelectorProcessor(void)
{
}

//!
void LandmarkSelectorProcessor::Update( )
{
	if ( GetInputDataEntity(0).IsNull( ) ||
		 GetInputDataEntity(0)->GetType() != Core::SurfaceMeshTypeId )
	{
		throw Core::Exceptions::Exception(
			"PropagateRegionProcessor::Update",
			"You must select a Mesh Image input from the Processing browser" );
	}

	try
	{
		// Create a signal data entity with the field information 
	
		// Create a signal
		blSignal::Pointer  signal = CreateScalarSignal();
		if (signal.IsNull() )
				return;
		blSignalCollective::Pointer signalCollective = blSignalCollective::New();
		signalCollective->SetNumberOfSignals(1);
		signalCollective->SetSignal(0, signal);


				 // Create a new signaldataentity
		std::string signalName =  + "Landmark_Signal";
		Core::DataEntity::Pointer dataEntitySignal;
		dataEntitySignal = Core::DataEntity::New( Core::SignalTypeId );
		dataEntitySignal->GetMetadata( )->SetName( signalName );
		dataEntitySignal->AddTimeStep( signalCollective );
		dataEntitySignal->SetFather(GetInputDataEntity(0));

		// Set the output
		SetOutputDataEntity(0,dataEntitySignal);


		//UpdateOutput(0,signalCollective,signalName, true);

	}
	coreCatchExceptionsReportAndNoThrowMacro( 
		"corePropagateRegionProcessor::Update" );

}

void LandmarkSelectorProcessor::SetInteractor(Core::PointInteractorPointSelect::Pointer interactor)
{
	m_PointInteractor = interactor;
}

blSignal::Pointer LandmarkSelectorProcessor::CreateScalarSignal()
{
	size_t numberOfTimeSteps = GetInputDataEntity(0)->GetNumberOfTimeSteps();
	std::vector<float> scalarValues;
	scalarValues.resize(numberOfTimeSteps);
	std::vector<float> timeSteps;
	timeSteps.resize(numberOfTimeSteps);
	for (size_t i =0; i< numberOfTimeSteps; i++)
	{
		Core::vtkPolyDataPtr polyData;
		Core::DataEntityHelper::GetProcessingData( 
						GetInputDataEntityHolder(0),
						polyData,
						i );


		Core::PointInteractorPointSelect::Pointer interactor; 
		// The following cas can only take place if the interactor was created as a PointInteractorPointSelect!
		interactor=	static_cast<Core::PointInteractorPointSelect*>(m_PointInteractor.GetPointer());
		if (interactor.IsNull())
			return NULL;

		int cellId = interactor->GetCellId();
		vtkDataArray* activeScalar = polyData->GetPointData()->GetScalars();
		scalarValues.at(i) =*activeScalar->GetTuple(cellId);
		timeSteps.at(i) = i;
	}

	// Create and fill the signal
	blSignal::Pointer signal = blSignal::New( );
	signal->SetName( "Signal" );
	signal->SetXVector( timeSteps );
	signal->SetYVector( scalarValues );
	signal->SetYUnit( "scalar values" );
	return signal;
}