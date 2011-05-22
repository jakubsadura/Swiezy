/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blIDSFileReader.h"
#include "itkImportImageFilter.h"

#include <stdio.h>
#include <stdlib.h>



blIDSFileReader::blIDSFileReader()
{
	// initialization of image data object to NULL
	vtkImageObj = NULL;
	itkVolumeObj = NULL;
	m_fileName = "";

	//initialization of properties of image
}


blIDSFileReader::~blIDSFileReader() 
{
}


void blIDSFileReader::Update()
{
	// Read the header file for obtaining the dimensions and the spacing
	unsigned int dimensions[3]; float spacing[3];
	
	ReadISCFile( m_fileName, dimensions, spacing);

	ReadIDSFile(dimensions, spacing);
}
	
/**
*/
int blIDSFileReader::ReadISCFile( std::string filename, unsigned int dimension[3], float spacing[3] )
{
	// read the header (.ics)
	FILE * icsFile = NULL;
	long lSize;

	std::string icsFilename = filename + ".ics";

	icsFile = fopen( icsFilename.c_str()  , "rb" );
	if (icsFile==NULL) {fputs ("File error",stderr); exit (0);}

	// obtain file size:
	fseek (icsFile , 0 , SEEK_END);
	lSize = ftell (icsFile);
	rewind (icsFile);

	char str [80];
	while( ftell (icsFile) < lSize)
	{
		// Go through all the lines of the header
		fscanf(icsFile, "%s", str, 80);

		//get the m_dimensions
		if (strcmp(str, "sizes") == 0)
		{
			fscanf(icsFile, "%u", &dimension[0]); // throw away the first one, don't know what it means
			fscanf(icsFile, "%u", &dimension[0]);
			fscanf(icsFile, "%u", &dimension[1]);
			fscanf(icsFile, "%u", &dimension[2]);
		}

		//get the m_spacing
		if (strcmp(str, "scale") == 0)
		{
			fscanf(icsFile, "%f", &spacing[0]);// throw away the first one, don't know what it means
			fscanf(icsFile, "%f", &spacing[0]);
			fscanf(icsFile, "%f", &spacing[1]);
			fscanf(icsFile, "%f", &spacing[2]);
		}
	}			
	// terminate
	fclose (icsFile);
	return 1;
}


/**
*/
int blIDSFileReader::ReadIDSFile( unsigned int * dimensions, float * spacing ) 
{
	/// Define the pixel type
	typedef unsigned short PixelType;

	// Import filter used to write the gabor filter to disk
	typedef itk::ImportImageFilter< PixelType, 3 > ImportFilter3D;

	const unsigned long int nPixels = dimensions[0] * dimensions[1] * dimensions[2];
	PixelType *buffer = new PixelType[ nPixels ];
	FILE * pFile;
	std::string idsFilename = m_fileName + ".ids";
	pFile = fopen( idsFilename.c_str(), "r" );
	fread(buffer,sizeof(unsigned short), nPixels , pFile);

	// using the data read from the IDS file create an ITK image with the same characteristics and contents
	base::UnsignedShortVolumeType::RegionType region;
	base::UnsignedShortVolumeType::IndexType start;
	start.Fill( 0 );
	region.SetIndex( start );

	itkVolumeObj = base::UnsignedShortVolumeType::New( );

	ImportFilter3D::SizeType size; 
	size[0] = dimensions[0];
	size[1] = dimensions[1];
	size[2] = dimensions[2];
	region.SetSize( size );
	itkVolumeObj->SetRegions( region );
	itkVolumeObj->Allocate( );

	//origin is always set to (0,0,0)
	double origin[3];
	origin[0] = 0.0;
	origin[1] = 0.0;
	origin[2] = 0.0;
	itkVolumeObj->SetOrigin( origin );

	itkVolumeObj->SetSpacing( spacing );
	itkVolumeObj->GetPixelContainer( )->SetImportPointer( buffer, nPixels, true );

	return 1;
}

base::UnsignedShortVolumeType::Pointer blIDSFileReader::GetOutput()
{
	return this->itkVolumeObj;
}

std::string blIDSFileReader::GetFileName()
{
	return m_fileName;
}

vtkSmartPointer<vtkDataObject> blIDSFileReader::GetVTKOutput()
{
	return this->vtkImageObj;
}

void blIDSFileReader::SetFileName( std::string fileName )
{
	this->m_fileName = fileName;
}