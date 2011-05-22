/*
* Copyright (c) 2008,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//Standard
# include <math.h>

//VTK
#include <vtkClipPolyData.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkCellLocator.h>
#include <vtkPlane.h>
#include <vtkCutter.h>


//BASELIB
#include <blLightObject.h>
#include <blSmartPointer.h>
#include <blMacro.h>
#include <blvtkFinitCylinder.h>

//GeoLib

#include <meNormalPlaneToVessel.h>

#include "meRingCut.h"
#include <meCloseHoles.h>

	//--------------------------------------------------
	meRingCut::meRingCut()
	//--------------------------------------------------
	{
		this->m_Input = vtkSmartPointer<vtkPolyData>::New();
		this->m_ClosedMesh = vtkSmartPointer<vtkPolyData>::New();
		this->m_Skeleton =vtkSmartPointer< vtkPolyData>::New();
		this->m_FirstOutput = vtkSmartPointer<vtkPolyData>::New();
		this->m_SecondOutput =vtkSmartPointer< vtkPolyData>::New();
		this->m_Intersection =vtkSmartPointer< vtkPolyData>::New();
		this->m_PointCoordinates[ 0 ] = 0;
		this->m_PointCoordinates[ 1 ] = 0;
		this->m_PointCoordinates[ 2 ] = 0;
		this->m_Normal[ 0 ] = 0;
		this->m_Normal[ 1 ] = 0;
		this->m_Normal[ 2 ] = 0;
		this->m_Radius = 0;
		this->m_PointId = -1;
		this->m_PointAndTangentComputed = false;
		this->meshPointUsed = false;
		this->meshSecondPointUsed = false;
	}

	//--------------------------------------------------
	meRingCut::~meRingCut()
	//--------------------------------------------------
	{
	}

	//--------------------------------------------------
	void meRingCut::Update()
	//--------------------------------------------------
	{
	  this->GetRadius();

	  vtkSmartPointer<blvtkFinitCylinder>  CircularPlaneClip = 
		  vtkSmartPointer<blvtkFinitCylinder>::New();
	  CircularPlaneClip->SetOrigin(this->m_PointCoordinates);
	  CircularPlaneClip->SetNormal(this->m_Normal);
	//CircularPlaneClip->SetRadius(this->m_Radius*1.4);
	  CircularPlaneClip->SetRadius(this->m_Radius*1.1);
	  CircularPlaneClip->SetWidth(this->m_Radius*2);

	  vtkSmartPointer<vtkClipPolyData>  clip = vtkSmartPointer<vtkClipPolyData>::New();
	  clip->SetClipFunction(CircularPlaneClip);
	  clip->SetInput(this->m_Input);
	  clip->Update();

 	 vtkSmartPointer<vtkPolyData>  mesh = vtkSmartPointer<vtkPolyData>::New();
 	  mesh->DeepCopy(clip->GetOutput());
	  
	  vtkSmartPointer<vtkPolyDataConnectivityFilter> connec = 
		  vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	  connec->SetInput(mesh);
	 // connec->SetInput(clip->GetOutput());
	  connec->SetExtractionModeToClosestPointRegion();
	  connec->SetClosestPoint(m_PointCoordinates);
	  connec->Update();
	  this->m_FirstOutput->DeepCopy(connec->GetOutput());

	  double invertedNormal[3];
	  for(int i=0; i<3; i++) invertedNormal[i] = -this->m_Normal[i];
	  CircularPlaneClip->SetOrigin(this->m_PointCoordinates);
	  CircularPlaneClip->SetNormal(invertedNormal);
	  clip->Update();

 	  mesh->DeepCopy(clip->GetOutput());
	  connec->SetInput(mesh);
	  connec->Update();

	  this->m_SecondOutput->DeepCopy(connec->GetOutput());
	  
	}

	//--------------------------------------------------
	double meRingCut::GetRadius()
	//--------------------------------------------------
	{
	  // If we have selected a pointId in the skeleton, compute the coordinates of the point
	  // and the tangent vector at this point, if they have not been already computed.

		if ( !(this->m_PointId == -1 || this->m_PointAndTangentComputed) )
		{
			this->GetPointCoordinates();
			this->GetTangentVector();
			this->m_PointAndTangentComputed = true;
		}
	//	if ( this->m_Radius == 0 )
	//	{
			meNormalPlaneToVessel::Pointer normalCorrector = meNormalPlaneToVessel::New();
			normalCorrector->SetSurfaceMesh(this->m_Input);
			normalCorrector->SetClosedSurfaceMesh(this->m_ClosedMesh);
			if(this->meshPointUsed)
			{
				normalCorrector->SetOriginMeshId(this->meshPointId);
				if(this->meshSecondPointUsed)
				{
					normalCorrector->SetSecondOriginMeshId(this->meshSecondPointId);
				}
				normalCorrector->InitializeNormal();
			}
			else
			{
				normalCorrector->SetOrigin(this->m_PointCoordinates);
				normalCorrector->InitializeNormal(this->m_Normal);
			}
			normalCorrector->Update();
			normalCorrector->GetNormal(this->m_Normal);
			normalCorrector->ComputeCircumscribingCenterAndRadius();
			normalCorrector->GetCircumscribingCenter(this->m_PointCoordinates);
			this->m_Radius = normalCorrector->GetCircumscribingRadius();
			normalCorrector->GetContourCrossSection(this->m_Intersection);
	//	}
		return this->m_Radius;
	}

	//--------------------------------------------------
	void meRingCut::GetPointCoordinates()
	//--------------------------------------------------
	{
	  for(int i=0; i<3; i++)
	  {
		m_PointCoordinates[i] = this->m_Skeleton->GetPoint(this->m_PointId)[i];
	  }
	}

	//--------------------------------------------------
	void meRingCut::GetTangentVector()
	//--------------------------------------------------
	{
	  vtkSmartPointer<vtkIdList>  NeighbourCells = 
		  vtkSmartPointer<vtkIdList>::New();
	  vtkSmartPointer<vtkIdList>  FirstCell = 
		  vtkSmartPointer<vtkIdList>::New();
	  vtkSmartPointer<vtkIdList>  SecondCell = 
		  vtkSmartPointer<vtkIdList>::New();
	  int FirstPointId,
		  SecondPointId;
	  this->m_Skeleton->GetPointCells(this->m_PointId, NeighbourCells);
	  if( NeighbourCells->GetNumberOfIds() != 2 ) 
	  {
		cout << "ERROR: The number of neighbours of this point is not 2" << endl;
	  }
	  this->m_Skeleton->GetCellPoints(NeighbourCells->GetId(0), FirstCell);
	  this->m_Skeleton->GetCellPoints(NeighbourCells->GetId(1), SecondCell);
	  if( FirstCell->GetId(0) == this->m_PointId )
	  {
		FirstPointId = FirstCell->GetId(1);
	  }
	  else
	  {
		FirstPointId = FirstCell->GetId(0);
	  }
	  if( SecondCell->GetId(0) == this->m_PointId )
	  {
		SecondPointId = SecondCell->GetId(1);
	  }
	  else
	  {
		SecondPointId = SecondCell->GetId(0);
	  }
	  for(int i=0; i<3; i++)
	  {
		// If we don't use local variables, the computation is wrong
		// but we don't know why...
		double val1 = this->m_Skeleton->GetPoint(FirstPointId)[i];
		double val2 = this->m_Skeleton->GetPoint(SecondPointId)[i];
		m_Normal[i] = val1 - val2;
	  }
	}

	//--------------------------------------------------
	void meRingCut::CloseMesh()
	//--------------------------------------------------
	{
	meCloseHoles::Pointer meshcloser = meCloseHoles::New();
	meshcloser->SetInput(this->m_Input);
	meshcloser->Update();
	this->m_ClosedMesh->DeepCopy(meshcloser->GetOutput());
	//this->m_ClosedMesh->DeepCopy(this->m_Input);
	}

	// 	//--------------------------------------------------
	// 	void meRingCut::SetOriginMeshId(pointId)
	// 	//--------------------------------------------------
	// 	{
	// 		
	// 	}


void meRingCut::SetInput( vtkPolyData *Input )
{
	if ( Input != NULL )
	{
		this->m_Input->DeepCopy(Input);
	}
	else
	{
		this->m_Input = vtkSmartPointer<vtkPolyData>::New();
	}
	//this->m_ClosedMesh->DeepCopy(this->m_Input);
	this->CloseMesh();
}

void meRingCut::SetSkeleton( vtkPolyData *Skeleton )
{
this->m_Skeleton->DeepCopy(Skeleton);
}

vtkPolyData * meRingCut::GetFirstOutput()
{
	return this->m_FirstOutput;
}

vtkPolyData * meRingCut::GetSecondOutput()
{
	return this->m_SecondOutput;
}

void meRingCut::SetSkeletonPoint( int PointId )
{
	this->m_PointId = PointId;
	this->m_PointAndTangentComputed = false;
}

void meRingCut::SetSkeletonPoint( double skeletonPoint[ 3 ] )
{
	this->m_PointCoordinates[ 0 ] = skeletonPoint[ 0 ];
	this->m_PointCoordinates[ 1 ] = skeletonPoint[ 1 ];
	this->m_PointCoordinates[ 2 ] = skeletonPoint[ 2 ];
	this->m_Radius = 0;
}
void meRingCut::SetPoint( double point[3] )
{
	for(int i=0; i<3; i++) this->m_PointCoordinates[i] = point[i];
}

void meRingCut::SetNormal( double normal[3] )
{
	this->m_Normal[ 0 ] = normal[ 0 ];
	this->m_Normal[ 1 ] = normal[ 1 ];
	this->m_Normal[ 2 ] = normal[ 2 ];
}

void meRingCut::SetOriginMeshId( int pointId )
{
	this->meshPointId = pointId;
	for(int i=0; i<3; i++) this->m_PointCoordinates[i] = this->m_Input->GetPoint(pointId)[i];
	this->meshPointUsed = true;
	this->meshSecondPointUsed = false;
}

void meRingCut::SetSecondOriginMeshId( int pointId )
{
	this->meshSecondPointId = pointId;
	this->meshSecondPointUsed = true;
}

vtkPolyData * meRingCut::GetContourCrossSection()
{
	return this->m_Intersection;
}

void meRingCut::GetContourCrossSection( vtkPolyData * contour )
{
	contour->DeepCopy(this->m_Intersection);
	contour->Update();
}

double* meRingCut::GetCenter()
{
	GetRadius();
	return this->m_PointCoordinates;
}

void meRingCut::GetCenter( double center[3] )
{
	GetRadius();
	for(int i=0; i<3; i++)
	{
		center[i] = this->m_PointCoordinates[i];
	}
}

double* meRingCut::GetNormal()
{
	GetRadius();
	return this->m_Normal;
}

void meRingCut::GetNormal( double normal[3] )
{
	GetRadius();
	for(int i=0; i<3; i++)
	{
		normal[i] = this->m_Normal[i];
	}
}