/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blV3DImageFileReader_h
#define _blV3DImageFileReader_h

// basic includes
#include <vtkDataObject.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkImageData.h>
#include <vtkImageReader.h>
#include "itkVTKImageToImageFilter.h"

//#include <itkImageRegionIterator.h>
//#include <itkImageFileReader.h>
//#include <itkImageFileWriter.h>
//#include <itkImportImageFilter.h>
//#include <itkVTKImageIO.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>


// BaseLib
#include "BaseLibVTKWin32Header.h"
#include "blITKImagesDefines.h"

#include "blImageFileReader.h"


/// \brief Loads v3d image files (versions 3 and 4) and creates a vtk object (structured points) that can be retrieved.
///

/// \brief Indicates the different data types used during the v3d data values reading process
enum BLV3DTYPE { 
	BLV3Dchar = 2, 
	BLV3Duchar, 
	BLV3Dshort, 
	BLV3Dushort, /*default*/
	BLV3Dint, 
	BLV3Duint, 
	BLV3Dlong, 
	BLV3Dulong,
	BLV3Dfloat, 
	BLV3Ddouble
};

/// \brief Indicates the type of units conversion (if required)
enum BLV3DUNITCONV { 
	BLV3DnoConv = 100, /*default*/
	BLV3Dmm2cm 
};

class BASELIBVTK_EXPORT blV3DImageFileReader: public blImageFileReader
{
	public:

		//-----------------------------------------------------
		// Typedefs
		//-----------------------------------------------------

		typedef blV3DImageFileReader Self;
		typedef blSmartPointer< Self > Pointer;


		//typedef unsigned short UShortPixelType;
		//typedef itk::Image< UShortPixelType, 3 > UnsignedShortVolumeType;
		//typedef std::vector<double> VectorDoubleType;


		//-----------------------------------------------------
		// Methods
		//-----------------------------------------------------

		/// \brief Static constructor.
		blNewMacro( Self );

		/// \brief Read the image from a file, once specified with SetFileName() as well as other optional parameters such as scaling, spacing, origin, dimensions and verbose execution.
		void Update();

		/// \brief Get the VTK object from the specified V3D file, once Update() has been called.
		vtkDataObject* GetVTKOutput(void) 
		{
			return this->vtkImageObj;
		}

		/// \brief Get the VTK object from the specified V3D file, once Update() has been called.
		void GetOutput(vtkDataObject*& dataObj) 
		{
			dataObj = this->vtkImageObj;
		}

		/// \brief Get the ITK volumetric object from the specified V3D file, once Update() has been called.
		void GetOutput(base::UnsignedShortVolumeType::ConstPointer& itkVolumeObj) 
		{
			itkVolumeObj = this->itkVolumeObj;
		}

		/// \brief Get the ITK volumetric object from the specified V3D file, once Update() has been called.
		base::UnsignedShortVolumeType::ConstPointer GetITKOutput(void) 
		{
			return this->itkVolumeObj;
		}

		//-----------------------------------------------------
		// Initialization methods
		//-----------------------------------------------------
		/// \brief Set the origin as <float, float, float>.
		void SetOrigin(float fx, float fy, float fz) 
		{
			this->v3Origin[0] = fx; this->v3Origin[1] = fy; this->v3Origin[2] = fz;
		}

		/// \brief Set the spacing as <float, float, float>.
		void SetSpacing(float fx, float fy, float fz) 
		{
			this->v3Spacing[0] = fx; this->v3Spacing[1] = fy; this->v3Spacing[2] = fz;
		}

		/// \brief Set the dimensions as <float, float, float>.
		void SetDimensions(float fx, float fy, float fz) 
		{
			this->v3Dimensions[0] = fx; this->v3Dimensions[1] = fy; this->v3Dimensions[2] = fz;
		}

		/// \brief Set the scale as a float.
		void SetScale(float fScale) 
		{
			this->fScale = fScale;
		}

		/// \brief Set the data type among char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long, float or double.
		void SetType(BLV3DTYPE blV3DType) 
		{
			this->blV3DType = blV3DType;
		}

		/// \brief Indicates it is necessary byte swapping (default is false).
		void SetSwapOn() 
		{
			this->bSwapBytes = true;
		}
		
		/// \brief Indicates it is NOT necessary byte swapping (default is false).
		void SetSwapOff() 
		{
			this->bSwapBytes = false;
		}

		/// \brief Indicate what kind of unit conversion is required if necessary (default is no conversion is required).
		void SetUnitConversion(BLV3DUNITCONV blV3DUnitConvType) 
		{
			this->blV3DUnitConvType = blV3DUnitConvType;
		}

		/// \brief Indicates it is necessary printing data for debugging purposes during the reading process (default is false).
		void SetVerboseOn() 
		{
			this->bVerbose = true;
		}
		
		/// \brief IndicateS it is NOT necessary printing data for debugging purposes during the reading process (default is false).
		void SetVerboseOff() 
		{
			this->bVerbose = false;
		}
	
	protected:
		
		/// \brief Default Constructor.
		blV3DImageFileReader();

		/// \brief Destructor.
		virtual ~blV3DImageFileReader();

		//-----------------------------------------------------
		// Variables
		//-----------------------------------------------------
		vtkImageData					*vtkImageObj;	// resulting 3D vtk image object containing the read data from a v3d file
		base::UnsignedShortVolumeType::ConstPointer	itkVolumeObj;	// resulting 3D itk image object containing the read data from a v3d file

		base::VectorDoubleType	v3Origin;			// origin location of the 3D image object
		base::VectorDoubleType	v3Spacing;			// spacing of the 3D image object
		base::VectorDoubleType	v3Dimensions;		// dimensions of the 3D image object
		double				fScale;				// data scaling factor of the 3D image object
		BLV3DTYPE			blV3DType;			// data type of the information contained in the 3D image object
		BLV3DUNITCONV		blV3DUnitConvType;	// unit conversion type of the information contained in the 3D image object
		bool				bSwapBytes;			// indicates if byte swapping is necessary

		unsigned long		headerSize;			// header size of the v3d file

		bool				bVerbose;			// indicates if verbose mode is necessary (for debugging purposes)
		bool				bOrigin;			// indicates if manual selection of origin location was performed
		bool				bSpacing;			// indicates if manual selection of voxel spacing was performed
		bool				bDimensions;		// indicates if manual selection of dimensions was performed
		bool				bScaleData;			// indicates if a manual data scaling operation was specified

	private:

		blV3DImageFileReader( const blV3DImageFileReader& );	//purposely not implemented
		void operator = ( const blV3DImageFileReader& );	//purposely not implemented

		/// \brief Reads the data from the V3D file.
		void ReadDataFromV3D();

		/// \brief Performs reading operations specific to a v3d file version 3.
		void ReadV3Dversion3(FILE * fp);
		
		/// \brief Performs reading operations specific to a v3d file version 4.
		void ReadV3Dversion4(FILE * fp);

		/// \brief Performs reading operations specific to a v3d file version 6.
		void ReadV3Dversion6(FILE * fp);

		/// \brief After reading the data from a v3d file creates the corresponding VTK object.
		void CreateVTKDataObject();

		/// \brief Once the VTK data object is read from the V3D file, store the reference to it.
		void SetVTKDataObject(vtkDataObject* dataObj) {
			this->vtkImageObj = (vtkImageData*) dataObj;
		}
				/// \brief After reading the data from a v3d file creates the corresponding ITK object.
		void CreateITKDataObject();

		/// \brief Once the ITK data object is read from the V3D file, store the reference to it.
		void SetITKDataObject(base::UnsignedShortVolumeType::Pointer dataObj) {
			this->itkVolumeObj = (base::UnsignedShortVolumeType::Pointer) dataObj;
		}

		/// \brief Read a number of bytes from a file.
		void ReadBytes(void *ptr, int size, int num, FILE * fp);
		
		/// \brief Performs the byte swapping operation most suitable for the data specified by the parameters.
		void SwapBytes(void *ptr, int size, int num);
		
		/// \brief Sets the size of the current v3d file header after determining the version (3 or 4).
		void SetHeaderSize(unsigned long headerSize) 
		{
			this->headerSize = headerSize;
		}
		
		/// \brief Provides the size of the current v3d file header.
		unsigned long GetHeaderSize() 
		{
			return this->headerSize;
		}
		
		/// \brief Indicates it is necessary setting the dimensions (default is false).
		void SetOriginOn() 
		{
			this->bOrigin = true;
		}
		
		/// \brief Indicates it is NOT necessary setting the dimensions (default is false).
		void SetOriginOff() 
		{
			this->bOrigin = false;
		}

		/// \brief Indicates it is necessary setting the dimensions (default is false).
		void SetSpacingOn() 
		{
			this->bSpacing = true;
		}
		
		/// \brief Indicates it is NOT necessary setting the dimensions (default is false).
		void SetSpacingOff() 
		{
			this->bSpacing = false;
		}

		/// \brief Indicates it is necessary setting the dimensions (default is false).
		void SetDimensionsOn() 
		{
			this->bDimensions = true;
		}
		
		/// \brief Indicates it is NOT necessary setting the dimensions (default is false).
		void SetDimensionsOff() 
		{
			this->bDimensions = false;
		}

		/// \brief Indicates it is necessary scaling data values (default is false).
		void SetScaleDataOn() 
		{
			this->bScaleData = true;
		}
		
		/// \brief Indicates it is NOT necessary scaling data values (default is false).
		void SetScaleDataOff() 
		{
			this->bScaleData = false;
		}

		/// \brief Sets the VTK image object.
		void SetImageObj(vtkDataObject* vtkImageObj) 
		{
			this->vtkImageObj = (vtkImageData*)vtkImageObj;
		}

};


//--------------------------------------------------
//--------------------------------------------------

#ifdef BASELIB_TESTING

#include <itkImageFileReader.h>

namespace blTests
{

	int blV3DImageFileReaderTest( string fileName );

} // blTests namespace

#endif // BASELIB_TESTING

#endif // _blV3DImageFileReader_h

