/*=========================================================================
This file was copied from EvoLib/EvoToolsVTK module in order to
remove dependencies between the threshold segmentation and EvoLib.
Authors: jlyko and sordas. Date: 2009-04-22$
=========================================================================*/

#include "meRemoveInnerSurfaceFilter.h"
#include "meCloseHoles.h"

#include "itkLinearInterpolateImageFunction.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPolyDataConnectivityFilter.h"
#include "vtkPolyDataNormals.h"
#include "vtkPointData.h"
#include "vtkCleanPolydata.h"


//vtkStandardNewMacro(RemoveInnerSurfaceFilter<ItkImageType>);

//thisClass* thisClass::New() 
//{ 
//	vtkObject* ret = vtkObjectFactory::CreateInstance(#thisClass);
//	if(ret)
//	{
//		return static_cast<thisClass*>(ret);
//	}
//	return new thisClass;
//}
//vtkInstantiatorNewMacro(thisClass)


template <class ItkImageType>
RemoveInnerSurfaceFilter<ItkImageType>* RemoveInnerSurfaceFilter<ItkImageType>::New()
{
	return new RemoveInnerSurfaceFilter<ItkImageType>;
}

template <class ItkImageType>
RemoveInnerSurfaceFilter<ItkImageType>::RemoveInnerSurfaceFilter()
:
m_distanceMap(NULL)
{

}


template <class ItkImageType>
RemoveInnerSurfaceFilter<ItkImageType>::~RemoveInnerSurfaceFilter()
{

}

template <class ItkImageType>
double RemoveInnerSurfaceFilter<ItkImageType>::GetClosedSurfaceVolume(vtkSmartPointer< vtkPolyData > normals)
{
	double volume=0, points[3][3];

	vtkSmartPointer<vtkIdList> pointIds = vtkSmartPointer< vtkIdList >::New();

	for (int cellId = 0; cellId < normals->GetNumberOfPolys(); cellId++)
	{

		normals->GetCellPoints( cellId, pointIds );
		if (pointIds->GetNumberOfIds() == 3) {	// avoid cases with degeneric triangles

			for(int i = 0; i < 3 ; i++)
			{
				normals->GetPoints()->GetPoint( pointIds->GetId( i ), points[i] );
			}

			volume += points[1][1]*(points[2][0]*points[0][2]-points[2][2]*points[0][0])+
				points[2][1]*(points[0][0]*points[1][2]-points[0][2]*points[1][0])+
				points[0][1]*(points[1][0]*points[2][2]-points[1][2]*points[2][0]);
		}		
	}
	return volume;
}

template <class ItkImageType>
int RemoveInnerSurfaceFilter<ItkImageType>::RequestData(
	vtkInformation *vtkNotUsed(request),
	vtkInformationVector **inputVector,
	vtkInformationVector *outputVector)
{

	//check if distance map is valid
	if(m_distanceMap.IsNull())
	{
		vtkErrorMacro("Distance map is not set");
		return 1;
	}

	// get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	// get the input and output
	vtkPolyData *inputSurface = vtkPolyData::SafeDownCast(
		inInfo->Get(vtkDataObject::DATA_OBJECT()));
	vtkPolyData *outputSurface = vtkPolyData::SafeDownCast(
		outInfo->Get(vtkDataObject::DATA_OBJECT()));

	//create list of connected components
	vtkSmartPointer< vtkPolyDataConnectivityFilter > dataConnectivityFilter = vtkSmartPointer< vtkPolyDataConnectivityFilter >::New();
	dataConnectivityFilter->SetInput(inputSurface);
	dataConnectivityFilter->SetExtractionModeToSpecifiedRegions();
	dataConnectivityFilter->Update();

	int numRegions = dataConnectivityFilter->GetNumberOfExtractedRegions();
	vtkSmartPointer< vtkCleanPolyData > cleanPolyData = vtkSmartPointer< vtkCleanPolyData >::New();

	bool normalsPointOutwards = true;
	
	//keep the original list
	std::vector < vtkSmartPointer<vtkPolyData> > originalRegionList;

	for(int idRegion = 0; idRegion < numRegions; idRegion++ )
	{
		//add region
		dataConnectivityFilter->AddSpecifiedRegion(idRegion);
		dataConnectivityFilter->Update();

		//clean poly data
		cleanPolyData->SetInput(dataConnectivityFilter->GetOutput());
		cleanPolyData->ConvertPolysToLinesOn();
		cleanPolyData->SetTolerance(1.0e-10);
		cleanPolyData->SetAbsoluteTolerance(1.0e-10);
		cleanPolyData->Update();  

		vtkSmartPointer< vtkPolyData > regionCopy = vtkSmartPointer< vtkPolyData >::New();
		regionCopy->DeepCopy( cleanPolyData->GetOutput() );
		originalRegionList.push_back( regionCopy );
		dataConnectivityFilter->DeleteSpecifiedRegion(idRegion);
	}

	for(int idRegion = 0; idRegion < numRegions; idRegion++ )
	{
		//put the region to the original list, after cleanPolyData filter is applied
		//vtkSmartPointer< vtkPolyData > regionCopy = vtkSmartPointer< vtkPolyData >::New();
		vtkPolyData* region = originalRegionList.at(idRegion).GetPointer();
	
		//close holes
		meCloseHoles::Pointer meshcloser = meCloseHoles::New();
		meshcloser->SetInput(region);
		meshcloser->Update();
		
		//compute normal to a selected point
		//first compute normals for all component
		vtkSmartPointer< vtkPolyDataNormals > surfaceWithComputedNormals = vtkSmartPointer< vtkPolyDataNormals >::New();
		surfaceWithComputedNormals->SetInput( meshcloser->GetOutput() );
		surfaceWithComputedNormals->ComputePointNormalsOn();
		surfaceWithComputedNormals->SetFeatureAngle(180.0); // nothing is considered as an edge. Otherwise mesh splitting might occur.
		surfaceWithComputedNormals->ConsistencyOn();
		surfaceWithComputedNormals->AutoOrientNormalsOn(); //the normals in the output will point outwards. (it implies ConsistencyOn)	
		surfaceWithComputedNormals->Update();

		// Needed only if noted that AutoOrientNormals is not working well -> some normals start pointing inwards.
		if ( this->GetClosedSurfaceVolume( surfaceWithComputedNormals->GetOutput() ) > 0 )
			normalsPointOutwards = false;
		else
			normalsPointOutwards = true;

		//get a point
		typedef itk::LinearInterpolateImageFunction< ItkImageType > InterpolatorType;
		InterpolatorType::PointType translatedPoint;
		int pointId = meshcloser->GetOutput()->GetNumberOfPoints()/2;	// start from something in the middle -> less probable that will have border problems.
		bool pointInTheBoundary = true;
		while ( pointInTheBoundary && pointId < meshcloser->GetOutput()->GetNumberOfPoints() )
		{
			double point[3];
			meshcloser->GetOutput()->GetPoint(pointId, point);

			double pointNormal[3];
			surfaceWithComputedNormals->GetOutput()->GetPointData()->GetNormals()->GetTuple(pointId, pointNormal);

			//do a translation of selected point
			ItkImageType::SpacingType spacing = m_distanceMap->GetSpacing();
			if (normalsPointOutwards) {
				translatedPoint[0] = point[0] - spacing[0]/2 * pointNormal[0];
				translatedPoint[1] = point[1] - spacing[0]/2 * pointNormal[1];
				translatedPoint[2] = point[2] - spacing[0]/2 * pointNormal[2];
			} else { // normals pointing inwards
				translatedPoint[0] = point[0] + spacing[0]/2 * pointNormal[0];
				translatedPoint[1] = point[1] + spacing[0]/2 * pointNormal[1];
				translatedPoint[2] = point[2] + spacing[0]/2 * pointNormal[2];
			}

			InterpolatorType::ContinuousIndexType index;
			m_distanceMap->TransformPhysicalPointToContinuousIndex( translatedPoint, index );
			pointInTheBoundary = !m_distanceMap->GetLargestPossibleRegion( ).IsInside( index );

			pointId++;
		}
		
		if ( !pointInTheBoundary )
		{
			//evaluate the value of the translated point
			InterpolatorType::Pointer interpolator = InterpolatorType::New();
			interpolator->SetInputImage(m_distanceMap);
			double pointValue = interpolator->Evaluate(translatedPoint);
			
			//check if we want this region in the output
			if(pointValue < 0)
				dataConnectivityFilter->AddSpecifiedRegion(idRegion);
		}
	}

	//connect desired regions
	dataConnectivityFilter->Update();

	//clean poly data
	cleanPolyData->SetInput(dataConnectivityFilter->GetOutput());
	cleanPolyData->Update();  
	
	//final output
	//outputSurface->DeepCopy(dataConnectivityFilter->GetOutput());
	outputSurface->DeepCopy(cleanPolyData->GetOutput());

	return 1;
}
