// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "coreMeshStatisticsProcessor.h"

#include "coreLogger.h"
#include "coreEnvironment.h"
#include "coreDataEntityHelper.h"
#include "coreVTKPolyDataHolder.h"
#include "coreVTKUnstructuredGridHolder.h"

#include "meMeshStatistics.h"
#include "blSignalWriter.h"

using namespace Core;

MeshStatisticsProcessor::MeshStatisticsProcessor() 
{
	SetNumberOfInputs( 1 );
	GetInputPort( 0 )->SetName( "Input data entity" );
	GetInputPort( 0 )->SetDataEntityType( Core::VolumeMeshTypeId );
	SetNumberOfOutputs( 1 );
	GetOutputPort( 0 )->SetDataEntityType( Core::SignalTypeId );
	GetOutputPort( 0 )->SetReuseOutput( false );
	GetOutputPort( 0 )->SetDataEntityName( "Statistics Histograms" );

	statisticsData = StatisticsDataPtr(new StatisticsData);
	histogramData = NULL;
	numberofbins = 15;
// 	qualityStringMap.insert( QualityStringPair( "Area", new Quality( me::MeshStatistics::Area ) ) );
// 	qualityStringMap.insert( QualityStringPair( "Aspect Ratio", new Quality( me::MeshStatistics::AspectRatio ) ) );
// 	qualityStringMap.insert( QualityStringPair( "Edge Ratio", new Quality( me::MeshStatistics::EdgeRatio ) ) );
// 	qualityStringMap.insert( QualityStringPair( "Distortion", new Quality( me::MeshStatistics::Distortion ) ) );
// 	qualityStringMap.insert( QualityStringPair( "Radius Ratio", new Quality( me::MeshStatistics::RadiusRatio ) ) );
	qualityStringMap.insert( QualityStringPair( "Tetra Min Angle", new Quality( me::MeshStatistics::TetraMinAngle ) ) );
	qualityStringMap.insert( QualityStringPair( "Tetra Aspect Ratio", new Quality( me::MeshStatistics::TetraAspectRatio ) ) );
	qualityStringMap.insert( QualityStringPair( "Tetra Edge Ratio", new Quality( me::MeshStatistics::TetraEdgeRatio ) ) );

	
}

void MeshStatisticsProcessor::Update()
{
	try
	{
		SetState(Core::Runtime::APP_STATE_PROCESSING);
		Core::vtkPolyDataPtr inputMesh;
		Core::vtkUnstructuredGridPtr inputVolMesh;
		Core::DataEntityHolder::Pointer inputData = GetInputDataEntityHolder(0);
		me::MeshStatistics::Pointer meshStatistics;

		if(inputData.GetPointer() == NULL)
		{
			return;
		}
		
		if(inputData->GetSubject()->IsSurfaceMesh())
		{
			
			Core::DataEntityHelper::GetProcessingData( 
				GetInputDataEntityHolder( 0 ),
				inputMesh );
			meshStatistics = me::MeshStatistics::New( inputMesh );
		}
		else if(inputData->GetSubject()->IsVolumeMesh())
		{
			Core::DataEntityHelper::GetProcessingData( 
			GetInputDataEntityHolder( 0 ),inputVolMesh );
			meshStatistics = me::MeshStatistics::New( inputVolMesh );
		}
		else
		{
			return;
		}
		blSignalCollective::Pointer signal = blSignalCollective::New();
		QualityStringMap::iterator it;
		unsigned int param_numberofbins = GetNumberofBins();
		for( it = qualityStringMap.begin(); it != qualityStringMap.end(); it++ )
		{
			Quality* q = (*it).second;
			if(q->m_selected == true)
			{
				q->m_averageValue = meshStatistics->getAverageValueOfQuality(q->m_qualityType);
				//q->m_histogramData = meshStatistics->computeHistogram(15, q->m_qualityType);
				q->m_histogramData = meshStatistics->computeHistogram(param_numberofbins, q->m_qualityType);
				blSignal::Pointer histogram =  meshStatistics->GetSignal();
				histogram->SetName((*it).first);
				signal->AddSignal(histogram);
			}
		}

		UpdateOutput( 0,
 						signal,
 						"Histograms statistics",
						false,
						1,
 						GetInputDataEntity(0));
		

		SetState(Core::Runtime::APP_STATE_IDLE);
	}
	coreCatchExceptionsReportAndNoThrowMacro( "Processor::Update()" );
}

unsigned int MeshStatisticsProcessor::GetNumberofBins(void)
{
	return numberofbins;
}

void MeshStatisticsProcessor::SetNumberofBins(unsigned int textnumberofbins )
{
	numberofbins = textnumberofbins;
}
