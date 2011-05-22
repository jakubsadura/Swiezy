/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blVectorFieldUtils.h"
#include "vtkMetaImageReader.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"
#include "vtkUnstructuredGrid.h"
#include "vtkArrowSource.h"
#include "vtkImageData.h"
#include "vtkFloatArray.h"
#include "vtkGlyph3D.h"
#include <cmath>

/*
*/
bool blVectorFieldUtils::CheckLoadFileExtension(const char *vectorFieldFileName)
{
	bool bRes = false;

	if ( strlen(vectorFieldFileName) < 4 )
	{
		return false;
	}

	bRes |= strcmp(vectorFieldFileName+strlen(vectorFieldFileName)-4,".mhd") == 0;
	//vtk displacement field should be also supported. Uncommenting the following line does nothing	
	//bRes |= strcmp(vectorFieldFileName+strlen(vectorFieldFileName)-4,".vtk") == 0;

	return bRes;
}


/*
*/
blVectorFieldUtils::VectorFieldFormats blVectorFieldUtils::GetTypeOfVectorFieldData(const char *vectorFieldFileName, bool checkReadingOfFile)
{
	blVectorFieldUtils::VectorFieldFormats type = blVectorFieldUtils::UnknownType;

	if (!strcmp(vectorFieldFileName+strlen(vectorFieldFileName)-4,".mhd"))
	{
		if (checkReadingOfFile)
		{
			vtkMetaImageReader* reader = vtkMetaImageReader::New();
			if(reader->CanReadFile(vectorFieldFileName))
				type = blVectorFieldUtils::MHDVectorFieldType;
			reader->Delete();
		}
		else
			type = blVectorFieldUtils::MHDVectorFieldType;
	}

	return type;
}


/*
*/
vtkPolyData* blVectorFieldUtils::CreateGlyph(const char *vectorFieldFileName)
{
	//the subsampleFactor factor. a factor of 10 is 10^3
	const int subsampleFactor = 1000;

	//! Meta image properties
	double metaOrigin[3] = {VTK_DOUBLE_MAX,	VTK_DOUBLE_MAX,	VTK_DOUBLE_MAX };
	double metaSpacing[3]	= {	- VTK_DOUBLE_MAX, - VTK_DOUBLE_MAX,	- VTK_DOUBLE_MAX };
	int metaDimensions[3]	= {	VTK_INT_MIN, VTK_INT_MIN, VTK_INT_MIN };

	vtkMetaImageReader* metaImageReader = vtkMetaImageReader::New();
	metaImageReader->SetFileName(vectorFieldFileName);
	metaImageReader->Update();

	metaImageReader->GetDataOrigin( metaOrigin );
	metaImageReader->GetDataSpacing( metaSpacing );
	metaImageReader->GetOutput()->GetDimensions( metaDimensions );

	int metaNumberOfPixels = metaImageReader->GetOutput()->GetNumberOfPoints();

	// Create Glyph Data: Scalar and vectors
	vtkImageData* ImageDataArray = metaImageReader->GetOutput();
	vtkPointData* PointDataArray = ImageDataArray->GetPointData();

	vtkFloatArray* VectorField = vtkFloatArray::New();
	VectorField->SetNumberOfComponents(3);
	VectorField->SetNumberOfTuples(metaNumberOfPixels);

	vtkFloatArray* ScalarField = vtkFloatArray::New();
	ScalarField->SetNumberOfComponents(1);
	ScalarField->SetNumberOfTuples(metaNumberOfPixels);

	vtkFloatArray* Scalar_tmp = vtkFloatArray::New();
	Scalar_tmp->DeepCopy(PointDataArray->GetScalars());

	//! Calculate the magnitude and the vector
	for (int i=0;i<metaNumberOfPixels;i++)
	{
		double* tmp_Tuple = Scalar_tmp->GetTuple3(i);
		double tmp0 = tmp_Tuple[0];
		double tmp1 = tmp_Tuple[1];
		double tmp2 = tmp_Tuple[2];
		double norm2 = std::sqrt(tmp0*tmp0 + tmp1*tmp1 + tmp2*tmp2);

		ScalarField->InsertTuple1(i,norm2);
		VectorField->InsertTuple3(i,tmp0/norm2,tmp1/norm2,tmp2/norm2);
	}

	// Creating point list for Glyph
	vtkPoints* PointList = vtkPoints::New();
	PointList->SetNumberOfPoints(metaNumberOfPixels);

	int counter = 0;

	for (int iz=0;iz<metaDimensions[2];iz++)
	{
		for (int iy=0;iy<metaDimensions[1];iy++)
		{
			for (int ix=0;ix<metaDimensions[0];ix++)
			{
				PointList->InsertPoint (counter, ix * metaSpacing[0] + metaOrigin[0], iy * metaSpacing[1] + metaOrigin[1], iz * metaSpacing[2] + metaOrigin[2]);
				counter ++;
			}
		}
	}

	//Resampled vector, scalar and point list
	vtkFloatArray* resampledVector = vtkFloatArray::New();
	resampledVector->SetNumberOfComponents(3);
	resampledVector->SetNumberOfTuples(metaNumberOfPixels/subsampleFactor);

	vtkFloatArray* resampledScalarField = vtkFloatArray::New();
	resampledScalarField->SetNumberOfComponents(1);
	resampledScalarField->SetNumberOfTuples(metaNumberOfPixels/subsampleFactor);

	vtkPoints* resampledPointList = vtkPoints::New();
	resampledPointList->SetNumberOfPoints(metaNumberOfPixels/subsampleFactor);

	double scaleFactor = 0.1; 
	
	//subsampling
	counter = 0;
	for (int i=0;i<metaNumberOfPixels; i= i + subsampleFactor)
	{
		resampledScalarField->InsertTuple1(counter,ScalarField->GetTuple1(i)*scaleFactor);
		double* tmpVector = VectorField->GetTuple3(i);
		resampledVector->InsertTuple3(counter,tmpVector[0],tmpVector[1],tmpVector[2]);
		double* tmpPoint = PointList->GetPoint(i);
		resampledPointList->InsertPoint(counter,tmpPoint[0],tmpPoint[1],tmpPoint[2]);
		counter++;
	}

	 //Preparing Glyph Input
	vtkUnstructuredGrid* GridArray = vtkUnstructuredGrid::New();
	GridArray->SetPoints(resampledPointList);

	GridArray->GetPointData()->SetVectors(resampledVector);
	GridArray->GetPointData()->SetScalars(resampledScalarField);

	// Glyph Visualization
	vtkArrowSource* arrow = vtkArrowSource::New();
	vtkGlyph3D* glyph = vtkGlyph3D::New();
	glyph->SetInput(GridArray);
	glyph->SetSourceConnection(arrow->GetOutputPort());
	glyph->SetVectorModeToUseVector();
	glyph->SetScaleModeToScaleByScalar();
	glyph->SetColorModeToColorByScalar();
	glyph->Update();

	vtkPolyData* PolyGlyph = glyph->GetOutput();

	vtkPolyData* PolyDataGlyphCopy = vtkPolyData::New();
	PolyDataGlyphCopy->DeepCopy(PolyGlyph);

	//clean up
	metaImageReader->Delete();
	VectorField->Delete();
	ScalarField->Delete();
	Scalar_tmp->Delete();
	PointList->Delete();
	resampledVector->Delete();
	resampledScalarField->Delete();
	resampledPointList->Delete();
	GridArray->Delete();
	arrow->Delete();
	glyph->Delete();

	return PolyDataGlyphCopy;
}


/*
*/
vtkPolyData* blVectorFieldUtils::LoadVectorFieldFromFileAsVTK(const char *vectorFieldFileName)
{
	vtkPolyData* glyphPolyData = NULL;
	bool checkReading = true;

	if (CheckLoadFileExtension(vectorFieldFileName))
	{
		switch(GetTypeOfVectorFieldData(vectorFieldFileName,checkReading))
		{
		case MHDVectorFieldType:
			{
				vtkMetaImageReader* reader = vtkMetaImageReader::New();
				reader->SetFileName(vectorFieldFileName);
				reader->Update();
				//! Check that the image is a vector image field 
				if ( reader->GetOutput()->GetPointData()->GetScalars()->GetNumberOfComponents() == 3 )
				{
					//! Create the Glyph
					glyphPolyData = vtkPolyData::New();
					glyphPolyData = CreateGlyph( vectorFieldFileName );
					
					//vtkPolyData* PolyDataGlyphCopy = vtkPolyData::New();
					//glyphPolyData->DeepCopy(reader->GetOutput());
				}
				reader->Delete();
			}
			break;

		case UnknownType:
		default:
			break;
		}
	}
	return glyphPolyData;
}

