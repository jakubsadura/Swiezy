/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//---------------------------------------------------
// HEADERS
//---------------------------------------------------

#include "blCardiacShape.h"

// BaseLib
#include "blShapeUtils.h"
#include "blVTKHelperTools.h"

//ITK
#include "vnl/vnl_vector.h"
#include "vnl/vnl_vector_ref.h"
#include "vnl/vnl_cross.h"

//VTK
#include "vtkPolyData.h"
#include "vtkStringArray.h"
#include "vtkShortArray.h"
#include "vtkFeatureEdges.h"
#include "vtkLandmarkTransform.h"
#include "vtkSmartPointer.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkCutter.h"
#include "vtkPolyDataNormals.h"
#include "vtkPointLocator.h"

using namespace blShapeUtils;

blCardiacShape::blCardiacShape( vtkPolyData *inputShape /*= NULL*/ )
{
	SetInputShape( inputShape );
	m_strVectorSubpartName = Cardio::VECTOR_NAME_SUBPART_ID;
}

blCardiacShape::~blCardiacShape( )
{

}

vtkPolyData *blCardiacShape::ApplyNormalsFilter( )
{
	vtkPolyData* inputWithNormals = vtkPolyData::New();
	vtkPolyDataNormals* polydataNormalsFilter = vtkPolyDataNormals::New();
	polydataNormalsFilter->SetInput( m_InputShape.GetPointer() );
	polydataNormalsFilter->SetOutput(inputWithNormals);
	polydataNormalsFilter->ComputePointNormalsOn();	
	polydataNormalsFilter->SplittingOff();
	polydataNormalsFilter->ConsistencyOn();
	try 
	{
		polydataNormalsFilter->Update();
	}
	catch( vtkstd::exception& e ) 
	{ 
		throw e;
	}

	polydataNormalsFilter->Delete();
	
	return inputWithNormals;
}

vtkPolyData* blCardiacShape::GetSubpart( 
						Cardio::SubpartType subpartIDStart,
						Cardio::SubpartType subpartIDEnd )
{
	if ( subpartIDEnd == Cardio::NONE_SUBPART )
	{
		subpartIDEnd = subpartIDStart;
	}

	if ( !m_InputShape )
	{
		throw Exception( "blCardiacShape::GetCardiacModelSubpart( ):" \
							  "Input shape cannot be NULL");
	}

	vtkPolyData* subpart = ShapeUtils::GetShapeRegion(
						m_InputShape,
						subpartIDStart,
						subpartIDEnd,
						m_strVectorSubpartName.c_str() );

	if (!subpart)
	{
		throw Exception( "blCardiacShape::GetCardiacModelSubpart( ):" \
							  "Cannot get cardiac model subpart");
	}

	return subpart;
}

vtkPolyData* blCardiacShape::RemoveCoronaries( )
{
	vtkPolyData* vtkFilteredMesh = NULL;

	if ( m_InputShape->GetPointData()->GetArray( 
		Cardio::VECTOR_NAME_SUBPART_ID ) )
	{
		vtkFilteredMesh = blShapeUtils::ShapeUtils::GetShapeRegion( 
			m_InputShape, 
			Cardio::LV_ENDO_SUBPART, 
			Cardio::FS_SUBPART, 
			m_strVectorSubpartName.c_str() );
	}
	else
	{
		vtkFilteredMesh = vtkPolyData::New();
		vtkFilteredMesh->DeepCopy( m_InputShape );
	}
	
	return vtkFilteredMesh;
}

vtkPolyData* blCardiacShape::GetLVChamber( )
{
	vtkPolyData* vtkFilteredMesh = NULL;

	if ( m_InputShape->GetPointData()->GetArray( 
		Cardio::VECTOR_NAME_SUBPART_ID ) )
	{
		vtkFilteredMesh = blShapeUtils::ShapeUtils::GetShapeRegion( 
			m_InputShape, 
			Cardio::LV_ENDO_SUBPART, 
			Cardio::LV_EPI_SUBPART, 
			m_strVectorSubpartName.c_str() );
	}
	else
	{
		vtkFilteredMesh = vtkPolyData::New();
		vtkFilteredMesh->DeepCopy( m_InputShape );
	}

	return vtkFilteredMesh;
}

void blCardiacShape::GetLVEndoCentroid( double centroid[ 3 ] )
{
	vtkSmartPointer<vtkPolyData> endoSubpart;
	endoSubpart.TakeReference( GetSubpart( Cardio::LV_ENDO_SUBPART ) );

	vtkIdType numPoints = endoSubpart->GetNumberOfPoints();

	centroid[ 0 ] = 0;
	centroid[ 1 ] = 0;
	centroid[ 2 ] = 0;
	double *currentPoint;
	for(int i=0; i<numPoints; i++)
	{
		currentPoint = endoSubpart->GetPoint(i);
		centroid[ 0 ] += currentPoint[ 0 ];
		centroid[ 1 ] += currentPoint[ 1 ];
		centroid[ 2 ] += currentPoint[ 2 ];
	}

	centroid[ 0 ] /= numPoints;
	centroid[ 1 ] /= numPoints;
	centroid[ 2 ] /= numPoints;
}

vtkPolyData* blCardiacShape::GetInputShape() const
{
	return m_InputShape;
}

void blCardiacShape::SetInputShape( vtkPolyData* val )
{
	m_InputShape = val;
}

void blCardiacShape::GetSubpartRange( double range[2] )
{
	vtkShortArray *subpartIDArray;
	subpartIDArray = blVTKHelperTools::GetPointDataArray<vtkShortArray>( 
		m_InputShape, 
		m_strVectorSubpartName.c_str() );
	subpartIDArray->GetRange( range );
}

void blCardiacShape::SetVectorSubpartName( const char* vectorName )
{
	m_strVectorSubpartName = vectorName;
}



void blCardiacShape::GetLVLongAxis(
	vnl_vector<double>& apex, 
	vnl_vector<double>& mithral)
{
	//extract endocardium edges and their centroid
	m_InputShape->GetPointData()->SetActiveScalars( "subpartID" );
	vtkPolyData *endo = blVTKHelperTools::GetShapeSubSurface( m_InputShape, Cardio::LV_ENDO_SUBPART );

	vtkSmartPointer<vtkFeatureEdges> fe = vtkSmartPointer<vtkFeatureEdges>::New();
	fe->SetInput(endo);
	fe->BoundaryEdgesOn();
	fe->FeatureEdgesOff();
	fe->NonManifoldEdgesOff();
	fe->ManifoldEdgesOff();
	fe->Update();
	vtkPolyData *edges = fe->GetOutput();

	//add centroid
	mithral.set_size(3);
	mithral.fill(0);
	for(int i=0; i<edges->GetPoints()->GetNumberOfPoints(); i++)
	{
		for(int j=0; j<3; j++)
			mithral[j] += edges->GetPoints()->GetPoint(i)[j];
	}

	mithral /= edges->GetPoints()->GetNumberOfPoints();


	//find the apex as the farthest point
	const int num_of_endo_pts = endo->GetNumberOfPoints();
	vtkIdList *ordered_endo_pts = vtkIdList::New();
	vtkPointLocator *pl = vtkPointLocator::New();
	pl->SetDataSet(endo);
	pl->BuildLocator();
	pl->FindClosestNPoints(endo->GetNumberOfPoints(), mithral.begin(), ordered_endo_pts);

	//apex is the farthest
	apex.set_size(3);
	apex.copy_in( endo->GetPoint(
		ordered_endo_pts->GetId(ordered_endo_pts->GetNumberOfIds()-1)) );

	ordered_endo_pts->Delete();
	pl->Delete();	

	endo->Delete();
}




