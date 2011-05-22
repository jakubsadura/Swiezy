/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blIDSFileWriter.h"
#include "blImageUtils.h"
#include "itkVTKImageToImageFilter.h"
#include "vtkImageCast.h"

#include <iostream>
#include <fstream>


//--------------------------------------------------
blIDSFileWriter::blIDSFileWriter() 
//--------------------------------------------------
{
	this->m_fileName = "";
	itkVolumeObj = NULL;
	vtkImageObj = NULL;
}

//--------------------------------------------------
blIDSFileWriter::~blIDSFileWriter() 
//--------------------------------------------------
{
}

//--------------------------------------------------
void blIDSFileWriter::SetInput(vtkImageData* dataObj) 
//--------------------------------------------------
{
	// IMPORTANT: For simplicity, we only convert vtk unsigned short to ids file (always unsigned short) 
	// ! TODO: CAST from input image pixel type to unsigned short

	vtkSmartPointer<vtkImageCast> castImage = vtkSmartPointer<vtkImageCast>::New();
	castImage->SetInput(dataObj);
	castImage->SetOutputScalarTypeToUnsignedShort();
	castImage->Update();

	itk::VTKImageToImageFilter<OutputImageType>::Pointer filter = itk::VTKImageToImageFilter<OutputImageType>::New();
	filter->SetInput(castImage->GetOutput()/*dataObj*/);
	filter->Update();
	itkVolumeObj = filter->GetOutput();

	//itkVolumeObj = blImageUtils::VtkToItkImage< base::UnsignedShortPixelType /*inputPixelType*/ , OutputImageType>(dataObj);
}


//--------------------------------------------------
void blIDSFileWriter::Update() 
//--------------------------------------------------
{
	
	itk::Size<3> dimensions = itkVolumeObj->GetLargestPossibleRegion().GetSize(); 
	OutputImageType::SpacingType spacing = itkVolumeObj->GetSpacing();
	int length = dimensions[0]*dimensions[1]*dimensions[2];
	const base::UnsignedShortPixelType *buffer ;
	std::string fileOutput = m_fileName +".ids";

	//write .ids file
	FILE * pFile;
	pFile = fopen(fileOutput.c_str(), "wb" );
	size_t size = sizeof(unsigned short);
	buffer = itkVolumeObj->GetBufferPointer( );
	fwrite( buffer, size, length, pFile);
	fclose( pFile );

	//write .ics file
	fileOutput = m_fileName + ".ics";
	std::ofstream outputfile;
	outputfile.open (fileOutput.c_str());

	outputfile << "ics_version\t"<<"1.0"<<"\n";
	outputfile << "filename\t" << m_fileName <<"\n";
	outputfile << "layout\t" <<"parameters\t"<<"4\t"<<"\n";
	outputfile << "layout\t" <<"order\t" << "bits\t" << "x\ty\tz\n";
	outputfile << "layout\t" <<"sizes\t" << "16\t" << dimensions[0]<<"\t" << dimensions[1]<<"\t" << dimensions[2]<<"\n";
	outputfile << "layout\t" <<"coordinates	video\n";
	outputfile << "layout\t" <<"significant_bits\t" <<"16\n";
	outputfile << "representation\t" <<	"format\t" << "integer\n";
	outputfile << "representation\t" <<	"sign\t" <<	"unsigned\n";
	outputfile << "representation\t" <<	"byte_order\t" << "1\t" <<"2\n";
	outputfile << "representation\t" <<	"SCIL_TYPE\t" << "g3d\n";
	outputfile << "parameter\t" <<	"scale\t" << "1\t"	<<  spacing[0]<<"\t" <<	spacing[1]<<"\t" <<	 spacing[2]<<"\n";	
	outputfile << "parameter\t"	<< "units\t" << "relative\t" << "mm\t" << "mm\t" << "mm\t" << "undefined\n";

	outputfile.close();
}

string blIDSFileWriter::GetFileName()
{
	return m_fileName;
}

void blIDSFileWriter::SetFileName( string fileName )
{
	this->m_fileName = fileName;
}
