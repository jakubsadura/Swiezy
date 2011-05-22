/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreVTKUnstructuredGridReader.h"
#include <blShapeUtils.h>
#include "coreVTKUnstructuredGridHolder.h"

using namespace Core::IO;

//!
VTKUnstructuredGridReader::VTKUnstructuredGridReader(void) 
{
	m_ValidExtensionsList.push_back( ".vtk" );
	m_ValidExtensionsList.push_back( ".vti" );
	m_ValidExtensionsList.push_back( ".vol" );
	m_ValidTypesList.push_back( VolumeMeshTypeId );
}

//!
VTKUnstructuredGridReader::~VTKUnstructuredGridReader(void)
{
}

//!
void VTKUnstructuredGridReader::ReadData( )
{
	ReadAllTimeSteps( Core::VolumeMeshTypeId );
}


boost::any VTKUnstructuredGridReader::ReadSingleTimeStep( int iTimeStep, const std::string &filename )
{
	Core::vtkUnstructuredGridPtr pVolume;
	pVolume.TakeReference(
		blShapeUtils::ShapeUtils::LoadVolumeFromFile( filename.c_str() ) );

	if ( pVolume.GetPointer() == NULL )
	{
		return boost::any( );
	}

	double *bounds = pVolume->GetBounds();
	if ( abs( bounds[1] - bounds[0] ) < 0.10 &&
		abs( bounds[3] - bounds[2] ) < 0.10 &&
		abs( bounds[5] - bounds[4] ) < 0.10 )
	{
		vtkUnstructuredGridPtr newProcessingDataPtr = vtkUnstructuredGridPtr::New();
		//mesh was given in meters!
		blVTKHelperTools::ScaleVolume( 
			pVolume, 
			newProcessingDataPtr, 
			1000 );	
		return newProcessingDataPtr;
	}

	return pVolume;
}
