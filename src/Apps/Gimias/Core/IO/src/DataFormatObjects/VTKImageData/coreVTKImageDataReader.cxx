/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreVTKImageDataReader.h"
#include "coreVTKImageDataHolder.h"
#include "coreStringHelper.h"

#include "blImageUtils.h"
#include "blVTKUtils.h"

#include <itksys/SystemTools.hxx>

using namespace Core::IO;


VTKImageDataReader::VTKImageDataReader(void)
{
	m_ValidExtensionsList.push_back( ".vtk" );
	m_ValidExtensionsList.push_back( ".vti" );
	m_ValidExtensionsList.push_back( ".v3d" );
	m_ValidExtensionsList.push_back( ".mhd" );
	m_ValidExtensionsList.push_back( ".hdr" );
	m_ValidExtensionsList.push_back( ".nii" );
	m_ValidExtensionsList.push_back( ".dcm" );
	m_ValidTypesList.push_back( ImageTypeId );
	m_ValidTypesList.push_back( ROITypeId );
}

VTKImageDataReader::~VTKImageDataReader(void)
{
}

void VTKImageDataReader::ReadData()
{
	Core::DataEntityType dataEntityType;
	std::string header;
	dataEntityType = Core::DataEntityInfoHelper::ReadDataEntityTypeDescription( m_Filenames[ 0 ] );
	if ( dataEntityType == UnknownTypeId )
	{
		dataEntityType = ImageTypeId;
	}

	ReadAllTimeSteps( dataEntityType );

	// Read "MultiROILevel" tag from header (deprecated)
	// Now it uses MetaData XML files
	if ( GetOutputDataEntity( 0 ).IsNotNull() &&
		 GetOutputDataEntity( 0 )->GetMetadata( )->FindTagByName( "MultiROILevel" ).IsNull() )
	{
		std::map<int,std::string> ROIlevels;
		ReadMultiROILevels( m_Filenames[ 0 ], ROIlevels );

		// Convert to blTagMap
		blTagMap::Pointer levels = blTagMap::New();
		std::map<int,std::string>::iterator it;
		for ( it = ROIlevels.begin(); it!= ROIlevels.end(); ++it )
		{
			levels->AddTag( it->second, it->first );
		}
		GetOutputDataEntity( 0 )->GetMetadata( )->AddTag( "MultiROILevel", levels );
	}
}


boost::any VTKImageDataReader::ReadSingleTimeStep( int iTimeStep, const std::string &filename )
{
	/*Core::vtkImageDataPtr pImage;
	pImage.TakeReference( 
	(vtkImageData*)blImageUtils::LoadImageFromFileAsVTK( fileName.c_str() ) );*/

	//return pImage;
	vtkSmartPointer<vtkImageData> image = blImageUtils::LoadImageFromFileAsVTK( filename.c_str() );
	if ( image.GetPointer() == NULL )
	{
		return boost::any( );
	}

	return image;
}

void VTKImageDataReader::ReadMultiROILevels( 
	const std::string &fileName,
	std::map<int,std::string> &ROIlevels )
{
	
	std::string header;
	blVTKHelperTools::blVTKUtils::ReadHeader( fileName.c_str(), header );

	if (header.substr(0,std::string("DataEntityType: ROI ").size()) == "DataEntityType: ROI ") 
	{
		std::string aux;
		aux = header.substr(std::string("DataEntityType: ROI ").size()+2,header.size());

		int levelInt = 0;
		while ( aux.size()>0 ) 
		{
			levelInt++;
			std::string currentLevel = "";
			for ( int i = 1; i < aux.size(); i++ ) 
			{
				if ( aux[i] == ']' )
				{
					break;
				}
				else 
				{
					currentLevel += aux[i];
				}
			}

			ROIlevels[levelInt] = currentLevel;

			if (aux.size()>currentLevel.size()+3) 
			{
				aux = aux.substr( currentLevel.size()+3, aux.size() );
			} 
			else
			{
				aux = "";
			}
		}
	}

}
