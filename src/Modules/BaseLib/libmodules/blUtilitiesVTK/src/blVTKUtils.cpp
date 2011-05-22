/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


	//---------------------------------------------------
	// HEADERS
	//---------------------------------------------------

	#include <itkImage.h>

	#include <vtkPointData.h>
	#include <vtkImageData.h>
	#include <vtkLookupTable.h>
	#include <vtkShortArray.h>
	#include <vtkDataReader.h>

	#include <blVTKUtils.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <fstream>




	//---------------------------------------------------
	// OPERATIONS
	//---------------------------------------------------


/**
 *	This method creates a new vtkImageData from a given itk image 
 *	with pixel type Unsigned Short.
 */
//-----------------------------------------------------------------
vtkImageData * blVTKHelperTools::blVTKUtils::ITK2VTK(Short2ImageType * itkImagePt)
//-----------------------------------------------------------------
{
	if (!itkImagePt) return 0;	// return if null pointer
	

	// region
	Short2ImageType::RegionType region = 
							itkImagePt->GetLargestPossibleRegion();


	// define origin, spacing and dimensions for VTK image
	double origin[3];
	origin[0] = itkImagePt->GetOrigin()[0];
	origin[1] = itkImagePt->GetOrigin()[1];
	origin[2] = 0.0;

	double spacing[3];
	spacing[0] = itkImagePt->GetSpacing()[0];
	spacing[1] = itkImagePt->GetSpacing()[1];
	spacing[2] = 1.0;

	unsigned int dim[3];
	dim[0] = region.GetSize()[0];
	dim[1] = region.GetSize()[1];
	dim[2] = 1;


	
	// new vtk image	
	//--------------------------
	vtkImageData * vtkImage = vtkImageData::New();	
	
///	vtkImage->SetScalarTypeToUnsignedShort();
	vtkImage->SetScalarTypeToShort();
	vtkImage->SetNumberOfScalarComponents(1);
	vtkImage->SetDimensions(dim[0], dim[1], dim[2]);	
	vtkImage->SetOrigin(origin[0], origin[1], origin[2]);
	vtkImage->SetSpacing(spacing[0], spacing[1], spacing[2]);
	


	// get buffer from itk image
	Short2ImageType::PixelContainer * pixelContainer = 
											itkImagePt->GetPixelContainer();

	// this does NOT copy the buffer, just share it
///	vtkUnsignedShortArray * dataBuffer = vtkUnsignedShortArray::New();
	vtkShortArray * dataBuffer = vtkShortArray::New();
	dataBuffer->SetArray(pixelContainer->GetBufferPointer(), 
										pixelContainer->Size(), 1);

	// set buffer to vtk image
	vtkPointData * pointData = vtkImage->GetPointData();
	pointData->SetScalars(dataBuffer);


	return vtkImage;
}


/**
 *	Creates a new 3D Image from a list of 2D slices
 *
 *	This only works for short pixel type
 */
//-------------------------------------------------------------------------
vtkImageData * blVTKHelperTools::blVTKUtils::Build3DVolume( 
								std::vector<VTKImagePointer> & imageVector,
								double spacingBetweenSlices)
//--------------------------------------------------------------------------
{
	unsigned int numberOfSlices = imageVector.size();
	
	if (numberOfSlices == 0) return 0;


	vtkImageData * firstImage = imageVector[0];

	int dim[3];
	dim[0] = firstImage->GetDimensions()[0];
	dim[1] = firstImage->GetDimensions()[1];
	dim[2] = numberOfSlices;


	double origin[3];
	origin[0] = 0.0;
	origin[1] = 0.0;
	origin[2] = 0.0;
	

	double spacing[3];
	spacing[0] = firstImage->GetSpacing()[0];
	spacing[1] = firstImage->GetSpacing()[1];
	spacing[2] = spacingBetweenSlices;



	// new vtk image	
	//--------------------------
	vtkImageData * volumePt = vtkImageData::New();	
	
	volumePt->SetDimensions(dim);	
	volumePt->SetOrigin(origin);
	volumePt->SetSpacing(spacing);

///	volumePt->SetScalarType(firstImage->GetScalarType());
	volumePt->SetScalarTypeToShort();

	volumePt->SetNumberOfScalarComponents(
							firstImage->GetNumberOfScalarComponents());
	
	// copy pixels from each slice
	// ----------------------------------
	unsigned int pixelsPerSlice = dim[0] * dim[1];
	unsigned int pixelsPerVolume = pixelsPerSlice * dim[2];

	short * pixelBuffer = new short[pixelsPerVolume];

	
	unsigned int i;
	for (i = 0; i < numberOfSlices; i++)
	{		
		vtkImageData * slicePt = imageVector[i];

		short * slicePixels = (short *) slicePt->GetScalarPointer();

		void * to = (void *) (pixelBuffer + i * pixelsPerSlice);
		memcpy(to, 
				slicePixels, 
				pixelsPerSlice * sizeof(short));

	}

	vtkShortArray * dataBuffer = vtkShortArray::New();
	dataBuffer->SetArray(pixelBuffer, pixelsPerVolume, 0);
		// 0 --> the data array will delete the pixel buffer

	// set buffer to volume
	vtkPointData * pointData = volumePt->GetPointData();
	pointData->SetScalars(dataBuffer);


	return volumePt;
}




/**
 *	not implemented yet
 */
//-----------------------------------------------------------------------
void blVTKHelperTools::blVTKUtils::Interpolate(const unsigned char * reds, 
							 const unsigned char * greens,
							 const unsigned char * blues,
							 unsigned int numberOfInputColors,
							 unsigned char * newReds,
							 unsigned char * newGreens,
							 unsigned char * newBlues,
							 unsigned int numberOfOutputColors)
//-----------------------------------------------------------------------
{
/*
		byte[] r = new byte[nColors]; 
		byte[] g = new byte[nColors]; 
		byte[] b = new byte[nColors];
		System.arraycopy(reds, 0, r, 0, nColors);
		System.arraycopy(greens, 0, g, 0, nColors);
		System.arraycopy(blues, 0, b, 0, nColors);
		double scale = nColors/256.0;
		int i1, i2;
		double fraction;
		for (int i=0; i<256; i++) {
			i1 = (int)(i*scale);
			i2 = i1+1;
			if (i2==nColors) i2 = nColors-1;
			fraction = i*scale - i1;
			//IJ.write(i+" "+i1+" "+i2+" "+fraction);
			reds[i] = (byte)((1.0-fraction)*(r[i1]&255) + fraction*(r[i2]&255));
			greens[i] = (byte)((1.0-fraction)*(g[i1]&255) + fraction*(g[i2]&255));
			blues[i] = (byte)((1.0-fraction)*(b[i1]&255) + fraction*(b[i2]&255));
		}
*/
}



void blVTKHelperTools::blVTKUtils::ReadHeader(const char *shapeFileName, std::string &strHeader)
{
	vtkDataReader * reader = vtkDataReader::New();
	reader -> SetFileName (shapeFileName);
	if ( reader -> OpenVTKFile() != 0 )
	{
		int iValue = vtkObject::GetGlobalWarningDisplay( );
		vtkObject::SetGlobalWarningDisplay( 0 );
		if ( reader -> ReadHeader() != 0 )
		{
			strHeader = reader -> GetHeader();
		}
		vtkObject::SetGlobalWarningDisplay( iValue );
	}
	reader -> Delete();
}






