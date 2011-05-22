/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreMHDVectorFieldReader.h"
#include "coreDataEntityInfoHelper.h"
#include "coreVTKPolyDataHolder.h"
#include "blVectorFieldUtils.h"

using namespace Core::IO;

//!
MHDVectorFieldReader::MHDVectorFieldReader(void) : BaseDataEntityReader() 
{
	m_ValidExtensionsList.push_back( ".mhd" );
	m_ValidTypesList.push_back( VectorFieldTypeId );
}

//!
MHDVectorFieldReader::~MHDVectorFieldReader(void)
{
}

//!
void MHDVectorFieldReader::ReadData()
{
	// build the data entity
	ReadAllTimeSteps( Core::VectorFieldTypeId );
}

boost::any MHDVectorFieldReader::ReadSingleTimeStep( int iTimeStep, const std::string &filename )
{
	Core::vtkPolyDataPtr pSurface;
	pSurface.TakeReference( 
		blVectorFieldUtils::LoadVectorFieldFromFileAsVTK( filename.c_str())  );

	if ( pSurface.GetPointer() == NULL )
	{
		return boost::any( );
	}

	return pSurface;
}
