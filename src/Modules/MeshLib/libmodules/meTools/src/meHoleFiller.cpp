/*=========================================================================

  Module    : MeshLib
  File      : $RCSfile: $
  Copyright : (C)opyright (Universitat Pompeu Fabra) 2007++
			  See COPYRIGHT statement in top level directory.
  Authors   : Xavier Mellado
  Modified  : $Author: $
  Purpose   : meHoleFiller implementation.
  Date      : $Date: $
  Version   : $Revision: $
  Changes   : $Locker:  $
			  $Log: $


=========================================================================*/

#include "meHoleFiller.h"

//VTK
#include <vtkFeatureEdges.h>
#include <vtkStripper.h>
#include <vtkTriangleFilter.h>
#include <vtkAppendPolyData.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkstd/exception>
#include <vtkCellArray.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolyDataWriter.h>

// MeshLib
#include <meVTKPolyDataRefiner.h>
#include <meVTKPolyDataFairer.h>
#include <meVTKTriangleFilterPaper.h>

#include "blShapeUtils.h"


//uncomment if you want the VTKTriangleFilter holefilling
#define VTKHOLEFILLER

//--------------------------------------------------
meHoleFiller::meHoleFiller()
//--------------------------------------------------
{
	// Create input to allow DeepCopy on SetInput function
	this->input = vtkPolyData::New();
	this->output = NULL;
	this->densityFactor = sqrt( 2.0 );
	this->angle = 0.0;
	this->percentage = 0.0001;
	this->order = 2;
	this->minimumEdgesSwapped = 10;
	this->m_Verbose = false;
}

//--------------------------------------------------
meHoleFiller::~meHoleFiller()
//--------------------------------------------------
{
	this->input->Delete();
	if ( this->output != NULL )
		this->output->Delete();
}

//--------------------------------------------------
void meHoleFiller::PrintPolyDataInfo( vtkPolyData *polydata )
//--------------------------------------------------
{
	cout << "Polydata has:\n\t"
		<< polydata->GetNumberOfPoints()  << " Points\n\t"
		<< polydata->GetNumberOfCells ()  << " Cells	 "
		<< "\n\t\tverts : " << polydata->GetNumberOfVerts ()
		<< "\n\t\tlines : " << polydata->GetNumberOfLines ()
		<< "\n\t\tpolys : " << polydata->GetNumberOfPolys ()
		<< "\n\t\tstrips: " << polydata->GetNumberOfStrips()
		<< endl;
}


//--------------------------------------------------
void meHoleFiller::Update()
//--------------------------------------------------
{

	if ( this->output != NULL )
		this->output->Delete();

	// Allocate output object
	this->output = vtkPolyData::New();

	
	// Extract edges, but only boundary edges
	vtkFeatureEdges *extractEdges = vtkFeatureEdges::New();
	extractEdges->SetInput( this->input );
	extractEdges->BoundaryEdgesOn();
	extractEdges->FeatureEdgesOff();
	extractEdges->ManifoldEdgesOff();
	extractEdges->NonManifoldEdgesOff();

	// Split output, to get boundaries one by one
	vtkPolyDataConnectivityFilter *connectivity = vtkPolyDataConnectivityFilter::New();
	connectivity->SetInput( extractEdges->GetOutput() );
	connectivity->SetExtractionModeToSpecifiedRegions();
	connectivity->Update();

	//unsigned int numHoles = connectivity->GetNumberOfExtractedRegions();
	
	
	unsigned int numHoles = blShapeUtils::ShapeUtils::ComputeNumberOfHoles(this->input); 

	if ( m_Verbose )
	{
		cout << "Number of holes: " << numHoles << endl;
	}

	// Append polydata, input is added and then add hole patch mesh one by one
	vtkAppendPolyData *append = vtkAppendPolyData::New();
	append->AddInput( this->input );

	try
    {
      for (unsigned int i = 0; i < numHoles; i++ )
	   {   
		   	  // Select boundary (hole) to close
			  connectivity->InitializeSpecifiedRegionList();
			  connectivity->AddSpecifiedRegion( i );
			  connectivity->Update();
			  
			  // Create an ordered polygon
			  vtkStripper *stripper = vtkStripper::New();
			  stripper->SetInput( connectivity->GetOutput() );
			  stripper->Update();
			  
			  // Convert to polydata (3D polyline)
			  vtkPolyData *polygon = vtkPolyData::New();
			  polygon->SetPoints( stripper->GetOutput()->GetPoints() );
			  polygon->SetPolys( stripper->GetOutput()->GetLines() );

			  // Triangulate it
              #ifdef VTKHOLEFILLER 
			  vtkTriangleFilter *triangulator = vtkTriangleFilter::New();
			  triangulator->SetInput( polygon );
			  triangulator->Update();
			  #else
			  meVTKTriangleFilterPaper::Pointer trianghole= meVTKTriangleFilterPaper::New();
			  trianghole->SetInput( this->input, polygon );
			  trianghole->Update();
			  #endif
			  //std::cout << "Number of Holes after first triangulation "<<ComputeNumOfHoles() <<std::endl;
			 			 
			  
			  if ( m_Verbose )
			  {
			  std::cout << "Triangulated" << std::endl;
			#ifdef VTKHOLEFILLER
			this->PrintPolyDataInfo( triangulator->GetOutput() );
			#else
			this->PrintPolyDataInfo( trianghole->GetOutput() );
			#endif
			  }

			//
			vtkPolyDataNormals* normals = vtkPolyDataNormals::New();
			#ifdef VTKHOLEFILLER
			normals->SetInput( triangulator->GetOutput() );
			#else
			normals->SetInput( trianghole->GetOutput() );
			#endif
			normals->ConsistencyOn();
			normals->ComputePointNormalsOff();
			normals->ComputeCellNormalsOn();
			normals->FlipNormalsOn();
			normals->Update();

			
			// Add new classes here
			meVTKPolyDataRefiner::Pointer refiner = meVTKPolyDataRefiner::New();
			refiner->SetInput( normals->GetOutput() );
			refiner->SetDensityFactor( this->densityFactor );
			refiner->SetAngle( this->angle );
			refiner->Update();

			meVTKPolyDataFairer::Pointer fairer = meVTKPolyDataFairer::New();
			fairer->SetInput( this->input, refiner->GetOutput() );
			fairer->SetOrder( this->order );
			fairer->SetPercentage( this->percentage );
			fairer->Update();
			

			// Append new hole patch mesh
			#ifdef VTKHOLEFILLER
			//append->AddInput( triangulator->GetOutput() );
			#else
			//append->AddInput( trianghole->GetOutput() );
			#endif
			//append->AddInput( refiner->GetOutput() );
			append->AddInput( fairer->GetOutput() );
			append->Update();

			// Delete loop local objects
			stripper->Delete();
			polygon->Delete();
			#ifdef VTKHOLEFILLER
			triangulator->Delete();
			#endif
			normals->Delete();
			
		}
		
	    append->Update( );
		this->output->DeepCopy( append->GetOutput() );
    }
    catch( vtkstd::exception& e )
    {
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
    }

	// Delete local objects
	append->Delete();
	connectivity->Delete();
	extractEdges->Delete();
}

bool meHoleFiller::GetVerbose() const
{
	return m_Verbose;
}

void meHoleFiller::SetVerbose( bool val )
{
	m_Verbose = val;
}

