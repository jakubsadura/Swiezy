/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _BLIMAGEUTILS_h
#define _BLIMAGEUTILS_h

#include "BaseLibVTKWin32Header.h"
#include "CILabExceptionMacros.h"
#include <exception>
#include <string>
#include "vtkSmartPointer.h"

class vtkStructuredPoints;
class vtkImageData;

namespace BaseLib
{
	cilabDeclareExceptionMacro(UnsupportedImagePixelTypeException, std::exception);
	cilabDeclareExceptionMacro(UnsupportedImageTypeException, std::exception);
}

/**
* \brief Static class with some utilities.
* \ingroup blImageUtilities
* \date 23 Nov 2007
* \author J.A. Moya, X.Mellado
*/
class BASELIBVTK_EXPORT blImageUtils
{
	public:

	/** Enumeration of the valid image formats that blImageUtils can handle */
	enum ImageFormats { 
		UnknownType, 
		VTKXMLStructuredPointsType,
	    V3DImageType, 
		VTKImageDataType, 
		MHDImageType, 
		AnalyzeImageType, 
		NiftyImageType,
		DicomType
		};
	
	/// Checks that the extension is correct
	static bool CheckSaveFileExtension(const char *imageFileName);

	/**\brief writes an image (vtkStructuredPoints) to file with format according to file extension (.vtk)
	* \param imagePt input shape
	* \param imageFileName file name
	* \param header Header of the file
	*/
	static void SaveImageToFile(
						vtkStructuredPoints *imagePt, 
						const char *imageFileName,
						const char *strHeader,
						bool bSaveAsASCII = true );

	/**\brief writes an image (vtkImageData) to file with format according to file extension (.vtk)
	* \param imagePt input shape
	* \param imageFileName file name
	* \param header Header of the file
	*/
	static void SaveImageToFile(
						vtkImageData *imagePt, 
						const char *imageFileName,
						const char *strHeader,
						bool bSaveAsASCII = true );

	/// Checks that the extension is correct
	static bool CheckLoadFileExtension(const char *imageFileName);

	/**\brief compares the filename extension with vtk valid types
	*  \param checkReadingOfFile if set, the function attempts loading them to verify the type of image that it represents
	*  \param shapeFileName file name
	*  \return an enumeration field describing what format the file contains (ImageFormats)
	*/
	static ImageFormats GetTypeOfImageData(const char *imageFileName, bool checkReadingOfFile = true);

	/**\brief reads an image from file (automatically adapts to .vtk) and retrieves a vtkStructuredPoints
	*  \param imageFileName file name
	*  \return input shape (vtkStructuredPoints*)
	* You need to call Delete( ) to the output of this function to 
	* destroy the object
	*/
	static vtkSmartPointer< vtkImageData > LoadImageFromFileAsVTK(const char *imageFileName);

	/**
	* \brief Write a VTK image to disc using itk::Image::IO.
	* \param data The data to write.
	* \param filename The name of the file to write to.
	* \note Only for 3D data.
	*/
	static void WriteITKImage( vtkImageData* vtkInputImage, const std::string& filename, bool flip = false);
	
	/**
	\brief Load image from disk usin itk::ImageFileReader and converting
	the image to vtkImageData.
	*/
	static vtkSmartPointer<vtkImageData> LoadITKImage(const char *imageFileName, bool orientedImage = false);

	/**
	\brief Convert from vtkImageData to itk::Image.
	Note that the image data is ALWAYS copied.
	*/
	template <typename inputPixelType, typename OutputItkImageType>
	static typename OutputItkImageType::Pointer VtkToItkImage
	( 
		vtkSmartPointer<vtkImageData> currentVtkInputImage
	);

	/**
	\brief A function that calls a filter to adapt a vtkImage data inside a DataEntity to an
	itkImage representation
	Note that the image data is ALWAYS copied.
	*/
	template <typename outputItkImageType>
	static typename outputItkImageType::Pointer ComputeVtkToItkImageAdaptation
	(
		vtkImageData* vtkInputImage,
		int pixelType
	);

	//!
	template < typename ImageType > 
	static vtkSmartPointer<vtkImageData> ConvertFromItkImageToVtkImage( 
		typename ImageType::Pointer itkImage );

	/**
	\brief Compare two images loading the files from disk
	*/
	static bool CompareImages( 
						const char *imageFileName1, 
						const char *imageFileName2,
						float fEpsilon = 1.5 );


	/**
	\brief Compare two vtkStructuredPoints and return if they are equal
	\note vtkStructuredPoints with precision float are written with 3 
	decimals, so default value for fEpsilon could be 0.001. However,
	the vtkStructuredPoints generated with 32 bit machine are different than
	64 bit machine. I put 1.5 default value.
	
	Compare:
	- Number of points
	- For each point
	
	\param [in] image1 input image 1
	\param [in] image2 input image 2
	\param [out] fMaxDifference max difference in points coordinates 
			position
	\param [in] fEpsilon Error range to compare the points 
			coordinates position
	*/
	static bool CompareImages( 
					vtkSmartPointer<vtkImageData>	image1,
					vtkSmartPointer<vtkImageData>	image2,
					float fEpsilon = 1.5 );

	/**
	Computes the cyclic redundancy code for \a image (http://en.wikipedia.org/wiki/Cyclic_redundancy_check).
	Uses boost::crc_basic<16> crc_ccitt1( 0x1021, 0xFFFF, 0, false, false ) for computing the CRC.
	*/
	static int ComputeCRC(vtkImageData* image);
};

#include "blImageUtils.txx"

#endif // _BLIMAGEUTILS_h
