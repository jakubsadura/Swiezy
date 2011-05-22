/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreVTKPolyDataReader.h"
#include <blShapeUtils.h>
#include "coreVTKPolyDataHolder.h"

using namespace Core::IO;

//!
VTKPolyDataReader::VTKPolyDataReader(void) 
{
	m_ValidExtensionsList.push_back( ".vtk" );
	m_ValidExtensionsList.push_back( ".vti" );
	m_ValidExtensionsList.push_back( ".stl" );
	m_ValidExtensionsList.push_back( ".vtp" );
	m_ValidTypesList.push_back( SurfaceMeshTypeId );
	m_ValidTypesList.push_back( PointSetTypeId );
	m_ValidTypesList.push_back( ContourTypeId );
}

//!
VTKPolyDataReader::~VTKPolyDataReader(void)
{
}

//!
void VTKPolyDataReader::ReadData( )
{
	// Read data entity description if any...
	m_DataEntityType = Core::DataEntityInfoHelper::ReadDataEntityTypeDescription( m_Filenames[ 0 ] );
	if ( m_DataEntityType == UnknownTypeId )
	{
		m_DataEntityType = SurfaceMeshTypeId;
	}

	// build the data entity
	ReadAllTimeSteps( m_DataEntityType );
}

boost::any VTKPolyDataReader::ReadSingleTimeStep( int iTimeStep, const std::string &filename )
{
	Core::vtkPolyDataPtr pSurface;
	pSurface.TakeReference(
		blShapeUtils::ShapeUtils::LoadShapeFromFile( filename.c_str() ) );

	if ( pSurface.GetPointer() == NULL )
	{
		return boost::any( );
	}

	double *bounds = pSurface->GetBounds();
	if ( m_DataEntityType == SurfaceMeshTypeId &&
		 abs( bounds[1] - bounds[0] ) < 0.10 &&
		 abs( bounds[3] - bounds[2] ) < 0.10 &&
		 abs( bounds[5] - bounds[4] ) < 0.10 )
	{
		vtkPolyDataPtr newProcessingDataPtr = vtkPolyDataPtr::New();
		//mesh was given in meters!
		blVTKHelperTools::ScaleShape( 
			pSurface, 
			newProcessingDataPtr, 
			1000 );	
		return newProcessingDataPtr;
	}

	return pSurface;
}
