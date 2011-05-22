/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "mlrProcessor.h"
#include "coreDataContainer.h"
#include "coreDataEntityHolder.h"
#include "coreReportExceptionMacros.h"
#include "coreRenderingTree.h"
#include "coreDataEntityList.h"
#include "coreDataEntityHelper.h"
#include "coreDataTreeHelper.h"

#include "itkBinaryThresholdImageFilter.h"
#include "itkAddImageFilter.h"
#include "itkNaryAddImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkMultiplyByConstantImageFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkImageToVTKImageFilter.h"
#include "itkVTKImageToImageFilter.h"

#include "vtkImageMathematics.h"
#include "vtkImageThreshold.h"
#include "vtkImageCast.h"
#include "vtkImageWriter.h"

#include "coreVTKImageDataHolder.h"
#include "mitkLevelWindowProperty.h"

#include "itkVector.h"

#include <sstream>

#include "blSignalCollective.h"

MultiLevelROIProcessor::MultiLevelROIProcessor() 
{
	SetNumberOfInputs( NUMBER_OF_INPUTS );
	SetNumberOfOutputs( NUMBER_OF_OUTPUTS );

	GetInputPort( INPUT_ROI )->SetName( "ROI image" );
	GetInputPort( INPUT_ROI )->SetDataEntityType( Core::ROITypeId );
}


MultiLevelROIProcessor::~MultiLevelROIProcessor() 
{

}

void MultiLevelROIProcessor::SetRange(int left, int right)
{
	m_left = left;
	m_right = right;
}

void MultiLevelROIProcessor::Propagate( ) 
{
	try {
		// Set state to processing (dialog box)
		Core::Runtime::Kernel::GetApplicationRuntime( )->SetAppState( 
			Core::Runtime::APP_STATE_PROCESSING );
		
		int nTotalTimeSteps = GetInputDataEntity( INPUT_ROI )->GetFather( )->GetNumberOfTimeSteps( );
		int currentTimeStep = GetTimeStep( );

		int firstTimeStep;
		int lastTimeStep;

		if ( std::abs(m_left) + std::abs(m_right) >= nTotalTimeSteps ) //then use all time steps
		{
			firstTimeStep = 0;
			lastTimeStep = nTotalTimeSteps-1;
		}
		else 
		{
			firstTimeStep = ( currentTimeStep + m_left + nTotalTimeSteps ) % nTotalTimeSteps;
			lastTimeStep = ( currentTimeStep + m_right ) % nTotalTimeSteps; 
		}


		bool finished = false;

		Core::vtkImageDataPtr roi;
		Core::DataEntityHelper::GetProcessingData(GetInputDataEntityHolder( INPUT_ROI ),
			roi,
			currentTimeStep );

		for ( int t = firstTimeStep; !finished ; t = (t + 1) % nTotalTimeSteps )
		{
			Core::vtkImageDataPtr roiTemp = Core::vtkImageDataPtr::New( );
			roiTemp->DeepCopy( roi );
			GetInputDataEntityHolder( INPUT_ROI )->GetSubject()->SetTimeStep( roiTemp, t  );
			
			finished = (t == lastTimeStep);
		}

		if ( GetInputDataEntity( INPUT_ROI )->GetFather( ) )
		{
			GetInputDataEntityHolder( INPUT_ROI )->GetSubject()->SetFather( GetInputDataEntity( INPUT_ROI )->GetFather( ) );
		}

		SetOutputDataEntity( PROPAGATED_ROI, GetInputDataEntityHolder( INPUT_ROI )->GetSubject() );

	} catch(...) {
		Core::Runtime::Kernel::GetApplicationRuntime( )->SetAppState( 
			Core::Runtime::APP_STATE_IDLE );
		throw;
	}

	Core::Runtime::Kernel::GetApplicationRuntime( )->SetAppState( 
		Core::Runtime::APP_STATE_IDLE );
}





void MultiLevelROIProcessor::CreateSignal( ) 
{
	try {
		// Set state to processing (dialog box)
		Core::Runtime::Kernel::GetApplicationRuntime( )->SetAppState( 
			Core::Runtime::APP_STATE_PROCESSING );

		// The output data entity that holds the signal
		Core::DataEntity::Pointer dataEntityIntensityMeasures;

		// Compute the intensity measures
		std::vector<blSignal::Pointer> intensityMeasures = ComputeIntensityMeasures();


		// Create a signal collective. This collective holds all the signals 
		blSignalCollective::Pointer signalCollective = blSignalCollective::New();
		signalCollective->SetNumberOfSignals(intensityMeasures.size());

		for ( int i = 0; i < intensityMeasures.size(); i++ )
		{
			signalCollective->SetSignal(i,intensityMeasures[i]);  
		}

		// build the data entity 
		blTagMap::Pointer ROIlevels;
		blTag::Pointer tag = GetInputDataEntity(INPUT_ROI)->GetMetadata()->FindTagByName( "MultiROILevel" );
		if ( tag.IsNotNull( ) )
		{
			tag->GetValue( ROIlevels );
		}

		dataEntityIntensityMeasures = Core::DataEntity::New( Core::SignalTypeId );
		dataEntityIntensityMeasures->AddTimeStep( signalCollective );

		std::string name;
		// Selected pixel level value starts from 1
		// and the tag map starts from 0
		tag = ROIlevels->GetTag( m_selectedLevel - 1 );
		if ( tag.IsNotNull() )
		{
			name = tag->GetName();
		}
		dataEntityIntensityMeasures->GetMetadata()->SetName( 
			name + " Intensity Measures" );

		dataEntityIntensityMeasures->SetFather( GetInputDataEntity(INPUT_ROI)->GetFather() );
		SetOutputDataEntity(SIGNAL, dataEntityIntensityMeasures);

	} catch(std::exception &e) {
		std::cout << e.what() << std::endl;
		Core::Runtime::Kernel::GetApplicationRuntime( )->SetAppState( 
			Core::Runtime::APP_STATE_IDLE );
		throw;
	}

	Core::Runtime::Kernel::GetApplicationRuntime( )->SetAppState( 
		Core::Runtime::APP_STATE_IDLE );

}

std::vector<blSignal::Pointer> MultiLevelROIProcessor::ComputeIntensityMeasures( ) 
{
	std::vector<float> meanIntensity;
	std::vector<float> stdIntensity;
	std::vector<float> minIntensity;
	std::vector<float> maxIntensity;

	std::vector<float> timeVector;
	timeVector.resize(GetInputDataEntity( INPUT_ROI )->GetNumberOfTimeSteps());
	meanIntensity.resize(GetInputDataEntity( INPUT_ROI )->GetNumberOfTimeSteps());
	stdIntensity.resize(GetInputDataEntity( INPUT_ROI )->GetNumberOfTimeSteps());
	minIntensity.resize(GetInputDataEntity( INPUT_ROI )->GetNumberOfTimeSteps());
	maxIntensity.resize(GetInputDataEntity( INPUT_ROI )->GetNumberOfTimeSteps());

	for ( int timeStep = 0; timeStep < GetInputDataEntity( INPUT_ROI )->GetNumberOfTimeSteps(); timeStep++ )
	{
		Core::vtkImageDataPtr parentImage;
		GetInputDataEntity( INPUT_ROI )->GetFather()->GetProcessingData( parentImage, timeStep );

		Core::vtkImageDataPtr roi;
		Core::DataEntityHelper::GetProcessingData( GetInputDataEntityHolder( INPUT_ROI ), roi, timeStep );

		int dimensions[3];
		parentImage->GetDimensions(dimensions);

		int numOfPixels = 0;
		int intensitySum = 0;
		int intensitySquareSum = 0;

		double tsMax,tsMin,tsStd,tsMean;

		tsMax = DBL_MIN;
		tsMin = DBL_MAX;

		int xBegin;
		int xEnd;
		int yBegin;
		int yEnd;
		int zBegin;
		int zEnd;

		switch (m_Domain)
		{
		case 1: //3d
			{
				xBegin = 0;
				xEnd = dimensions[0];
				yBegin = 0;
				yEnd = dimensions[1];
				zBegin = 0;
				zEnd = dimensions[2];
			}
			break;
		case 2: //current slice x
			{
				xBegin = m_Pos;
				xEnd = m_Pos+1;
				yBegin = 0;
				yEnd = dimensions[1];
				zBegin = 0;
				zEnd = dimensions[2];
			}
			break;
		case 3: //current slice y
			{
				xBegin = 0;
				xEnd = dimensions[0];
				yBegin = m_Pos;
				yEnd = m_Pos+1;
				zBegin = 0;
				zEnd = dimensions[2];
			}
			break;
		case 4: //current slice z
			{
				xBegin = 0;
				xEnd = dimensions[0];
				yBegin = 0;
				yEnd = dimensions[1];
				zBegin = m_Pos;
				zEnd = m_Pos+1;
			}
			break;
		default:
			{
				xBegin = 0;
				xEnd = dimensions[0];
				yBegin = 0;
				yEnd = dimensions[1];
				zBegin = 0;
				zEnd = dimensions[2];
			}
			break;
		}

		for(int x = xBegin; x < xEnd; x++ )
		{
			for( int y = yBegin; y < yEnd; y++ )
			{
				for( int z = zBegin; z < zEnd; z++ )
				{
					short* voxel1 = (short*)roi->GetScalarPointer(x,y,z);

					if ( (*voxel1) != (short) m_selectedLevel ) continue;

					short* voxel2 = (short*)parentImage->GetScalarPointer(x,y,z);

					if ( (*voxel1) * (*voxel2) != 0)
					{
						numOfPixels++;
						intensitySum += ( (*voxel2) );
						intensitySquareSum += ( (*voxel2) ) * ( (*voxel2) );
						if ( (*voxel2) > tsMax )
						{
							tsMax = (*voxel2);
						}

						if ( (*voxel2) < tsMin )
						{
							tsMin = (*voxel2);
						}
					}
				}
			}
		}

		if ( numOfPixels > 0 )
		{
			tsMean = (double)intensitySum / (double)numOfPixels;
			double stdAux = 0.0;

			stdAux = -2.0 * tsMean * intensitySum;
			stdAux += intensitySquareSum;
			stdAux += numOfPixels * tsMean * tsMean;
			stdAux /= (double) numOfPixels;

			tsStd = std::sqrt(stdAux);
		}
		else
		{
			tsMean = 0.0;
			tsStd = 0.0;
			tsMin = 0.0;
			tsMax = 0.0;
		}

		timeVector[timeStep]= timeStep;
		meanIntensity[timeStep] = tsMean;
		stdIntensity[timeStep] = tsStd;
		minIntensity[timeStep] = tsMin;
		maxIntensity[timeStep] = tsMax;

		if ( timeStep == GetTimeStep() )
		{
			m_measuresMap["mean"] = tsMean;
			m_measuresMap["std"] = tsStd;
			m_measuresMap["min"] = tsMin;
			m_measuresMap["max"] = tsMax;
		}
	}


	// Create the mean intensity signal 
	blSignal::Pointer signalMean = blSignal::New();
	signalMean->SetName("Mean Intensity Inside ROI");
	signalMean->SetXVector( timeVector );
	signalMean->SetYVector( meanIntensity );
	signalMean->SetXUnit("time step");
	signalMean->SetYUnit("Intensity");

	blSignal::Pointer signalStd = blSignal::New();
	signalStd->SetName("Intensity STD Inside ROI");
	signalStd->SetXVector( timeVector );
	signalStd->SetYVector( stdIntensity );
	signalStd->SetXUnit("time step");
	signalStd->SetYUnit("Intensity");

	blSignal::Pointer signalMin = blSignal::New();
	signalMin->SetName("Minimum Intensity Inside ROI");
	signalMin->SetXVector( timeVector );
	signalMin->SetYVector( minIntensity );
	signalMin->SetXUnit("time step");
	signalMin->SetYUnit("Intensity");

	blSignal::Pointer signalMax = blSignal::New();
	signalMax->SetName("Maximum Intensity Inside ROI");
	signalMax->SetXVector( timeVector );
	signalMax->SetYVector( maxIntensity );
	signalMax->SetXUnit("time step");
	signalMax->SetYUnit("Intensity");

	std::vector<blSignal::Pointer> signals;
	signals.push_back( signalMean );
	signals.push_back( signalStd );
	signals.push_back( signalMin );
	signals.push_back( signalMax );

	return signals;	

}


void MultiLevelROIProcessor::SetSelectedLevel(int selectedLevel)
{
	m_selectedLevel = selectedLevel;
}

void MultiLevelROIProcessor::SetDomain(int domain)
{
	m_Domain = domain;
}

void MultiLevelROIProcessor::SetPos(int pos)
{
	m_Pos = pos;
}

void MultiLevelROIProcessor::CalculateIntensityMeasures( int timeStep ) 
{
	try {
		// Set state to processing (dialog box)
		Core::Runtime::Kernel::GetApplicationRuntime( )->SetAppState( 
			Core::Runtime::APP_STATE_PROCESSING );

		double _mean;
		double _std;
		double _min;
		double _max;

		Core::vtkImageDataPtr parentImage;
		GetInputDataEntity( INPUT_ROI )->GetFather()->GetProcessingData( parentImage, timeStep );

		Core::vtkImageDataPtr roi;
		Core::DataEntityHelper::GetProcessingData( GetInputDataEntityHolder( INPUT_ROI ), roi, timeStep );

		int dimensions[3];
		parentImage->GetDimensions(dimensions);

		int numOfPixels = 0;
		int intensitySum = 0;
		int intensitySquareSum = 0;

		_max = DBL_MIN;
		_min = DBL_MAX;

		int xBegin;
		int xEnd;
		int yBegin;
		int yEnd;
		int zBegin;
		int zEnd;

		switch (m_Domain)
		{
			case 1: //3d
				{
					xBegin = 0;
					xEnd = dimensions[0];
					yBegin = 0;
					yEnd = dimensions[1];
					zBegin = 0;
					zEnd = dimensions[2];
				}
				break;
			case 2: //current slice x
				{
					xBegin = m_Pos;
					xEnd = m_Pos+1;
					yBegin = 0;
					yEnd = dimensions[1];
					zBegin = 0;
					zEnd = dimensions[2];
				}
				break;
			case 3: //current slice y
				{
					xBegin = 0;
					xEnd = dimensions[0];
					yBegin = m_Pos;
					yEnd = m_Pos+1;
					zBegin = 0;
					zEnd = dimensions[2];
				}
				break;
			case 4: //current slice z
				{
					xBegin = 0;
					xEnd = dimensions[0];
					yBegin = 0;
					yEnd = dimensions[1];
					zBegin = m_Pos;
					zEnd = m_Pos+1;
				}
				break;
			default:
				{
					xBegin = 0;
					xEnd = dimensions[0];
					yBegin = 0;
					yEnd = dimensions[1];
					zBegin = 0;
					zEnd = dimensions[2];
				}
				break;
		}

		for(int x = xBegin; x < xEnd; x++ )
		{
			for( int y = yBegin; y < yEnd; y++ )
			{
				for( int z = zBegin; z < zEnd; z++ )
				{
					short* voxel1 = (short*)roi->GetScalarPointer(x,y,z);
					if ( (*voxel1) != (short) m_selectedLevel ) continue;
					short* voxel2 = (short*)parentImage->GetScalarPointer(x,y,z);
					if ( (*voxel1) * (*voxel2) != 0)
					{
						numOfPixels++;
						intensitySum += ( (*voxel2) );
						intensitySquareSum += ( (*voxel2) ) * ( (*voxel2) );
						if ( (*voxel2) > _max )
						{
							_max = (*voxel2);
						}

						if ( (*voxel2) < _min )
						{
							_min = (*voxel2);
						}
					}
				}
			}
		}

		if ( numOfPixels > 0 )
		{
			_mean = (double)intensitySum / (double)numOfPixels;
			double measureSTDaux = 0.0;

			//we can compute the std in the following way 
			measureSTDaux = -2.0 * _mean * intensitySum;
			measureSTDaux += intensitySquareSum;
			measureSTDaux += numOfPixels * _mean * _mean;
			measureSTDaux /= (double) numOfPixels;

			_std = std::sqrt(measureSTDaux);
		}
		else
		{
			_mean = 0.0;
			_std = 0.0;
			_min = 0.0;
			_max = 0.0;
		}

		m_measuresMap["mean"] = _mean;
		m_measuresMap["std"] = _std;
		m_measuresMap["min"] = _min;
		m_measuresMap["max"] = _max;

	} catch(std::exception &e) {
		std::cout << e.what() << std::endl;
		Core::Runtime::Kernel::GetApplicationRuntime( )->SetAppState( 
			Core::Runtime::APP_STATE_IDLE );
		throw;
	}

	Core::Runtime::Kernel::GetApplicationRuntime( )->SetAppState( 
		Core::Runtime::APP_STATE_IDLE );

}

std::map<std::string,double> MultiLevelROIProcessor::GetIntensityMeasures( )
{
	return m_measuresMap;
}