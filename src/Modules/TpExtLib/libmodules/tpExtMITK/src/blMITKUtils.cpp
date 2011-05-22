/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <blMITKUtils.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkDataArray.h>
#include <vtkLookupTable.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include "vtkImageData.h"
#include "vtkLookupTable.h"
#include "vtkOBBTree.h"
#include "vtkCamera.h"
#include "vtkMath.h"
#include "vtkRenderer.h"
#include "vtkLine.h"

#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include "mitkVtkScalarModeProperty.h"
#include "mitkLookupTable.h"
#include "mitkLookupTableProperty.h"
#include "mitkSurface.h"
#include "mitkDataTreeNode.h"
#include "mitkProperties.h"
#include "mitkLevelWindowProperty.h"
#include "mitkVtkResliceInterpolationProperty.h"

void blMITKUtils::ApplyLookupTable(
	bool enable,
	mitk::DataTreeNode::Pointer node, 
	blLookupTables::LUT_TYPE lutType,
	blShapeUtils::ShapeUtils::VTKScalarType scalarType)
{
	if ( node.IsNull() )
	{
		return;
	}

	mitk::Image* mitkImage = dynamic_cast<mitk::Image*>( node->GetData() ); 
	mitk::Surface* mitkSurface = dynamic_cast<mitk::Surface*>(node->GetData()); 
	if ( mitkImage == NULL && mitkSurface == NULL)
	{
		return;
	}
	if ( mitkImage != NULL )
	{
		ApplyLookupTableToImage( enable, node, lutType );
	}
	if ( mitkSurface != NULL )
	{
		ApplyLookupTableToSurface( node, lutType , scalarType.name, scalarType.mode);
	}
}


void blMITKUtils::ApplyLookupTableToImage(
	bool enable,
	mitk::DataTreeNode::Pointer node, 
	blLookupTables::LUT_TYPE lutType )
{
	mitk::Image* mitkImage = dynamic_cast<mitk::Image*>( node->GetData() ); 
	if ( mitkImage == NULL )
	{	
		return;
	}

	// Set the node properties
	node->SetProperty("use color", mitk::BoolProperty::New( !enable ) );

	if ( enable )
	{
		// We need to create a new one, because the MITK code checks if the pointer is different
		vtkSmartPointer<vtkLookupTable> vtkLut = vtkSmartPointer<vtkLookupTable>::New( );
		blLookupTables lookupTables;
		vtkLut->DeepCopy( lookupTables.BuildLookupTable( lutType) );
		if ( lookupTables.BuildLookupTable( lutType)->GetNumberOfColors() < 256 )
		{
			throw std::runtime_error( 
				"Selected LUT cannot contain less than 256 color for an image" );
		}

		// Set range
		blMITKUtils::SetVtkLutScalarRange( node, vtkLut );

		CreateLut( node, lutType );
	}
}

void blMITKUtils::ApplyLookupTableToSurface(
	mitk::DataTreeNode::Pointer node, 
	blLookupTables::LUT_TYPE lutType,
	std::string name ,
	int mode )
{

	
	// Set scalar mode and range
	SetScalarMode(  node, name, mode);
	
	node->SetProperty("use color", mitk::BoolProperty::New(true));
	node->SetProperty("color mode", mitk::BoolProperty::New(true));
	
	// Create the LUT
	
	CreateLut(node, lutType);
}

void blMITKUtils::SetVtkLutScalarRange( 
	mitk::DataTreeNode::Pointer node,
	vtkLookupTable* vtkLut )
{
	// Get scalars range
	mitk::Image::Pointer image;
	image = static_cast<mitk::Image*>( node->GetData() );
	if (image.IsNull()) 
	{
		return;
	}

	//Get the minimum and the maximum scalar data to the Lookup Table 
	vtkImageData* vtkImage = image->GetVtkImageData( );
	double range[2];
	vtkImage->GetScalarRange(range);

	vtkLut->SetRange( range[0], range[1]);
	vtkLut->Build();
}

void blMITKUtils::SetVtkLutScalarRangeForMesh( 
	mitk::DataTreeNode::Pointer node,
	double range[2])
{
	mitk::Surface* surfacedata = dynamic_cast<mitk::Surface*>(node->GetData()); 
	if (surfacedata == NULL) 
	{
		return;
	}

	node->SetProperty("ScalarsRangeMaximum", mitk::FloatProperty::New(float(range[1])));
	node->SetProperty("ScalarsRangeMinimum", mitk::FloatProperty::New(float(range[0])));
}

void blMITKUtils::CreateLut(
					  mitk::DataTreeNode::Pointer node,
					  blLookupTables::LUT_TYPE lutType)
{
	// The pointer will be managed by mitk::LookupTable
	vtkSmartPointer<vtkLookupTable> vtkLut =  vtkSmartPointer<vtkLookupTable>::New( );
	blLookupTables lookupTables;
	vtkLut->DeepCopy( lookupTables.BuildLookupTable( lutType ));
	vtkLut->SetVectorModeToMagnitude();
	node->SetProperty( "blLookupTablesType", mitk::IntProperty::New(lutType) );
	
	mitk::LookupTable::Pointer mitkLut = mitk::LookupTable::New();
	mitk::LookupTableProperty::Pointer mitkLutProp = mitk::LookupTableProperty::New();
	mitkLut->SetVtkLookupTable( vtkLut );
	mitkLutProp->SetLookupTable(mitkLut);
	node->ReplaceProperty( "LookupTable", mitkLutProp );
	
}

void blMITKUtils::SetScalarMode(
								mitk::DataTreeNode::Pointer node,
								std::string name,
								int mode)
{
	
	mitk::Surface* surfacedata = dynamic_cast<mitk::Surface*>(node->GetData()); 
	if (surfacedata == NULL) 
	{
		return;
	}

	mitk::VtkScalarModeProperty::Pointer scalarMode = mitk::VtkScalarModeProperty::New();
	double maxScalar = DBL_MIN;
	double minScalar = DBL_MAX;

	for (size_t i = 0; i< surfacedata->GetTimeSteps() ; i++ )
	{
		vtkSmartPointer<vtkPolyData> surfaceMesh = surfacedata->GetVtkPolyData( i );

		double range[2];

		vtkSmartPointer<vtkDataArray> ptScalars;
		vtkSmartPointer<vtkDataArray> cellScalars; 
		switch (mode)
		{
		case 0:
			if ( surfaceMesh->GetPointData() )
			{
				ptScalars = surfaceMesh->GetPointData( )->GetScalars(name.c_str());
				if ( ptScalars )
				{
					surfaceMesh->GetPointData()->SetActiveScalars(name.c_str());
					ptScalars->GetRange(range, -1);
				}
			}
			scalarMode->SetScalarModeToPointData();
			break;
		case 1:
			if ( surfaceMesh->GetCellData() )
			{
				cellScalars = surfaceMesh->GetCellData( )->GetScalars(name.c_str());
				if ( cellScalars )
				{
					surfaceMesh->GetCellData()->SetActiveScalars(name.c_str() );
					cellScalars->GetRange(range,-1);
				}
			}
			scalarMode->SetScalarModeToCellData();
			break;
		case 2:
			node->SetProperty("scalar visibility", mitk::BoolProperty::New(false));
			return;
			break;
		default:
			break;
		}
		maxScalar =  std::max( range[ 1 ], maxScalar );
		minScalar = std::min( range[ 0 ], minScalar );
	}

	//todo change maxscalar...to maxrange
	double maxRange[2];
	maxRange[0] = minScalar;
	maxRange[1] = maxScalar;

	node->SetProperty("scalar mode", scalarMode );
	node->SetProperty("scalar visibility", mitk::BoolProperty::New(true));
	SetVtkLutScalarRangeForMesh(node, maxRange);

}

void blMITKUtils::ApplyMeasurementProperties(
								mitk::DataTreeNode::Pointer node,
								std::string name /* = "distance"*/)
{
	node->SetProperty("show contour", mitk::BoolProperty::New(true));
	node->SetProperty("name", mitk::StringProperty::New(name));
	node->SetProperty("show distances",mitk::BoolProperty::New(true));
}

void blMITKUtils::ApplyMaskImageProperties(
								mitk::DataTreeNode::Pointer node,
								std::string name /* = "maskImage"*/)
{
	//node->SetProperty( "binary", mitk::BoolProperty::New(true) );
	//node->SetProperty("name", new mitk::StringProperty(name));
	node->SetProperty( "texture interpolation", mitk::BoolProperty::New(false) );
	node->SetProperty( "layer", mitk::IntProperty::New(10) );
	//node->SetProperty( "levelwindow", mitk::LevelWindowProperty::New( mitk::LevelWindow(0.5, 1) ) );
	node->SetProperty( "opacity", mitk::FloatProperty::New(0.6) );
	node->SetProperty( "segmentation", mitk::BoolProperty::New(true) );
	node->SetProperty( "reslice interpolation", mitk::VtkResliceInterpolationProperty::New() ); 
	//node->SetColor(255,255,0);
	blMITKUtils::ApplyLookupTable( 
		true, 
		node, 
		blLookupTables::LUT_TYPE_RAINBOW_HUE,
		blShapeUtils::ShapeUtils::VTKScalarType( ) );

}

bool blMITKUtils::ComputePointSize( 
							 vtkPolyData* polyData, 
							 double &pointSize, 
							 double scaleFactor )
{
	pointSize = 0;
	vtkSmartPointer<vtkOBBTree> obbTree = vtkSmartPointer<vtkOBBTree>::New();
	double corner[3], max[3], mid[3], min[3], size[3];
	
	if ( polyData != NULL && polyData->GetPoints() != NULL )
	{
		obbTree->ComputeOBB(polyData->GetPoints(), corner, max, mid, min, size);
		double setSize = sqrt( max[0] * max[0] + max[1] * max[1] + max[2] * max[2]);
		pointSize = scaleFactor * setSize;
		return true;
	}

	return false;
}

bool blMITKUtils::ComputePointSize( mitk::BaseData* data, double &pointSize, double scaleFactor /*= 0.1 */ )
{
	pointSize = -1;
	if ( data == NULL )
	{
		return false;
	}

	pointSize = scaleFactor * data->GetTimeSlicedGeometry()->GetDiagonalLength();

	return true;
}

template <class T>
bool ExtractImagePointExecute1( vtkImageData *in1Data, T *in1Ptr,
								 int outExt[6], int point[3])
{
	vtkIdType inIncX, inIncY, inIncZ;
	int idxR, idxY, idxZ;
	int maxY, maxZ;
	int rowLength;

	// find the region to loop over
	rowLength =
		(outExt[1] - outExt[0]+1)*in1Data->GetNumberOfScalarComponents();
	// What a pain.  Maybe I should just make another filter.
	maxY = outExt[3] - outExt[2];
	maxZ = outExt[5] - outExt[4];

	point[ 0 ] = 0;
	point[ 1 ] = 0;
	point[ 2 ] = 0;


	// Get increments to march through data
	in1Data->GetContinuousIncrements(outExt, inIncX, inIncY, inIncZ);

	// Loop through output pixels
	for (idxZ = 0; idxZ <= maxZ; idxZ++)
	{
		for (idxY = 0; idxY <= maxY; idxY++)
		{
			for (idxR = 0; idxR < rowLength; idxR++)
			{
				// Pixel operaton
				if (*in1Ptr)
				{
					point[ 0 ] = idxR;
					point[ 1 ] = idxY;
					point[ 2 ] = idxZ;
					return true;
				}
				in1Ptr++;
			}
			in1Ptr += inIncY;
		}
		in1Ptr += inIncZ;
	}

	return false;
}

bool blMITKUtils::ExtractImagePoint( 
	mitk::Image::Pointer image,
	int iTimeStep,
	mitk::Point3D &worldPoint )
{
	vtkImageData *inData;
	void *inPtr1;
	int ext[6];

	inData = image->GetVtkImageData( iTimeStep );
	inData->GetExtent( ext );
	inPtr1 = inData->GetScalarPointerForExtent( ext );

	int point[3];
	bool found = false;
	switch ( inData->GetScalarType() )
	{
		vtkTemplateMacro( 
			found = ExtractImagePointExecute1( 
				inData, static_cast<VTK_TT *>(inPtr1), ext, point ) );
	}

	if ( found )
	{
		mitk::Point3D index;
		index[ 0 ] = point[ 0 ];
		index[ 1 ] = point[ 1 ];
		index[ 2 ] = point[ 2 ];
		image->GetGeometry( iTimeStep )->IndexToWorld( index, worldPoint );
	}

	return found;
}

void blMITKUtils::ComputeRay(
	double selectionX, double selectionY, double selectionZ, 
	vtkRenderer *renderer,
	double p1World[4], double p2World[4] )
{
	int i;
	vtkCamera *camera;
	int picked=0;
	double cameraPos[4], cameraFP[4];
	double *displayCoords, *worldCoords;
	double *clipRange;
	double ray[3], rayLength;
	double tF, tB;
	double cameraDOP[3];

	//  Initialize picking process
	double PickPosition[3]; //selection point in world coordinates
	double SelectionPoint[3]; //selection point in window (pixel) coordinates
	SelectionPoint[0] = selectionX;
	SelectionPoint[1] = selectionY;
	SelectionPoint[2] = selectionZ;

	// Get camera focal point and position. Convert to display (screen) 
	// coordinates. We need a depth value for z-buffer.
	//
	camera = renderer->GetActiveCamera();
	camera->GetPosition(cameraPos);
	cameraPos[3] = 1.0;
	camera->GetFocalPoint(cameraFP);
	cameraFP[3] = 1.0;

	renderer->SetWorldPoint(cameraFP[0],cameraFP[1],cameraFP[2],cameraFP[3]);
	renderer->WorldToDisplay();
	displayCoords = renderer->GetDisplayPoint();
	selectionZ = displayCoords[2];

	// Convert the selection point into world coordinates.
	//
	renderer->SetDisplayPoint(selectionX, selectionY, selectionZ);
	renderer->DisplayToWorld();
	worldCoords = renderer->GetWorldPoint();
	for (i=0; i < 3; i++) 
	{
		PickPosition[i] = worldCoords[i] / worldCoords[3];
	}

	//  Compute the ray endpoints.  The ray is along the line running from
	//  the camera position to the selection point, starting where this line
	//  intersects the front clipping plane, and terminating where this
	//  line intersects the back clipping plane.
	for (i=0; i<3; i++)
	{
		ray[i] = PickPosition[i] - cameraPos[i];
	}
	for (i=0; i<3; i++)
	{
		cameraDOP[i] = cameraFP[i] - cameraPos[i];
	}

	vtkMath::Normalize(cameraDOP);

	if (( rayLength = vtkMath::Dot(cameraDOP,ray)) == 0.0 ) 
	{
		return ;
	} 

	clipRange = camera->GetClippingRange();

	if ( camera->GetParallelProjection() )
	{
		tF = clipRange[0] - rayLength;
		tB = clipRange[1] - rayLength;
		for (i=0; i<3; i++) 
		{
			p1World[i] = PickPosition[i] + tF*cameraDOP[i];
			p2World[i] = PickPosition[i] + tB*cameraDOP[i];
		}
	}
	else
	{
		tF = clipRange[0] / rayLength;
		tB = clipRange[1] / rayLength;
		for (i=0; i<3; i++) 
		{
			p1World[i] = cameraPos[i] + tF*ray[i];
			p2World[i] = cameraPos[i] + tB*ray[i];
		}
	}
	p1World[3] = p2World[3] = 1.0;
}

vtkIdType blMITKUtils::ComputeClosestPoint(
	mitk::StateEvent const* stateEvent,
	double pPoint[ 3 ],
	vtkPolyData* mesh)
{
	if ( mesh == NULL )
	{
		return -1;
	}

	bool isSkeleton = false;
	if ( mesh->GetNumberOfVerts() == 0 &&
		mesh->GetNumberOfStrips() == 0 &&
		mesh->GetNumberOfPoints() != 0 &&
		mesh->GetNumberOfLines() != 0  &&
		mesh->GetNumberOfPolys() == 0 
		)
	{
		isSkeleton = true;
	}

	// If is not an skeleton -> Find closest point
	if ( !isSkeleton )
	{
		return mesh->FindPoint( pPoint );
	}

	// Special case for skeleton
	mitk::DisplayPositionEvent const *posEvent = 
		dynamic_cast < const mitk::DisplayPositionEvent * > 
		(stateEvent->GetEvent());

	if (posEvent == NULL) 
		return -1;


	// Compute ray
	mitk::BaseRenderer* renderer = stateEvent->GetEvent()->GetSender();
	if ( !renderer )
	{
		return -1;
	}

	mitk::VtkPropRenderer* propRenderer = dynamic_cast<mitk::VtkPropRenderer*>( renderer );

	double selectionX;
	double selectionY;
	double selectionZ;
	selectionX = posEvent->GetDisplayPosition()[ 0 ];
	selectionY = posEvent->GetDisplayPosition()[ 1 ];
	selectionZ = 0;
	double p1World[4];
	double p2World[4];
	blMITKUtils::ComputeRay( 
		selectionX, 
		selectionY, 
		selectionZ, 
		propRenderer->GetVtkRenderer(), 
		p1World, p2World );


	// Compute closest point to the skeleton and the ray
	vtkIdType ptId, numPts;
	numPts = mesh->GetNumberOfPoints( );
	double distanceMin = DBL_MAX;
	vtkIdType ptIdMin = -1;
	for (ptId=0; ptId < numPts; ptId++)
	{
		double pointPosition[ 3 ];
		mesh->GetPoint( ptId, pointPosition );

		double distance = vtkLine::DistanceToLine( pointPosition, p1World, p2World );
		if ( distance < distanceMin )
		{
			distanceMin = distance;
			ptIdMin = ptId;
		}
	}

	return ptIdMin;
}

bool blMITKUtils::ComputeBoundingBox( mitk::BaseData* data, double boundingBox[ 6 ] )
{
	for ( int i = 0 ; i < 6 ; i++ )
	{
		boundingBox[ i ] = 0;
	}

	if ( data == NULL )
	{
		return false;
	}

	// Compute bounding box
	mitk::BoundingBox::BoundsArrayType bounds;
	bounds = data->GetTimeSlicedGeometry()->GetBounds();
	for ( int i = 0 ; i < 6 ; i++ )
	{
		boundingBox[ i ] = bounds[ i ];
	}
	for( int i = 0; i < 3; ++i )
	{
		double width = boundingBox[2*i + 1] - boundingBox[2*i];
		double delta = 0.5 * 0.1 * width;
		boundingBox[2*i] -= delta;
		boundingBox[2*i + 1] += delta;
	}

	return true;

}

void blMITKUtils::UpdateLookupTableProperty( mitk::DataTreeNode::Pointer node )
{
	if ( node.IsNull() )
	{
		return;
	}

	mitk::IntProperty::Pointer blLookupTablesTypeProperty;
	blLookupTablesTypeProperty = static_cast<mitk::IntProperty*>(node->GetProperty("blLookupTablesType"));
	if ( blLookupTablesTypeProperty.IsNull() )
	{
		return;
	}

	blLookupTables::LUT_TYPE type;
	type = blLookupTables::LUT_TYPE( blLookupTablesTypeProperty->GetValue() );
	CreateLut( node, type );
}
