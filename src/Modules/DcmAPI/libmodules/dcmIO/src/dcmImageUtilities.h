/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dcmImageUtilities_h
#define _dcmImageUtilities_h

//DcmAPI
#include "dcmAPIIOWin32Header.h"
#include "dcmTypes.h"
#include "dcmBase.h"
#include "dcmDataSet.h"

//ITK
#include "itkGDCMImageIO.h"
#include "itkImageFileReader.h"
#include "itkOrientImageFilter.h"

//VTK
#include "vtkSmartPointer.h"
#include "vtkImageData.h"
#include "blSliceImage.h"


namespace dcmAPI
{

/** 
* \brief This class is used to read 2D and 3D standard DICOM images.
*
* It also provide reading methods for specific data e.g. US Philips data.
* \ingroup io
*/
class DCMAPIIO_EXPORT ImageUtilities : public Base
{
public:
	//! Default Contructor
	ImageUtilities();
	//! Destructor
	virtual ~ImageUtilities();
	//!
	static std::string ReadPixelType(std::string path);
	//!
	static void StringToDouble(const char * string, double * array, unsigned int numberOfTokens);

	/** Assure corect spacing (handle spacing between slices that is lower 
	than 0), currently working just for axial aquisition
	*/
	template< typename ImageType > 
	static void AssureCorrectSpacing( typename ImageType::Pointer itkImage );

 	//!
	static itk::ImageIOBase::IOComponentType ParsePixelType(std::string type);

   // -----------------------------------------
   // The folowing methods duplicate functionality from
   // the API, they should be removed and their specificity
   // included in the API... (ym, 03/03/2010)
   // -----------------------------------------

	//! Read dicom image to the itkImage 
	template< typename ImageType > 
	static typename ImageType::Pointer ReadMultiSliceImageFromFiles(
		const std::vector< std::string > &sliceFilePaths,
		bool isTaggedMR = false );

	//! Read dicom image to the itkImage 
	template< typename ImageType > 
	static typename ImageType::Pointer ReadImageFromFile(
		const char *imageFileName );

	//! Read a single VtkImage using one single DICOM file
	template <class PixelType, unsigned int VImageDimension>
	static vtkSmartPointer<vtkImageData> ReadVtkImageFromFile( 
		const char *imageFileName,
		bool reorientData );

	//! Read a single VtkImage using multiple DICOM files
	template <class PixelType, unsigned int VImageDimension>
	static vtkSmartPointer<vtkImageData> ReadMultiSliceVtkImageFromFiles( 
		std::vector< std::string > sliceFilePaths,
		bool reorientData,
		bool isTaggedMR = false );

	//! Read a single blSliceImage using multiple DICOM files
	template <class PixelType>
	static blSliceImage::Pointer ReadMultiSliceImageSliceFromFiles( 
		std::vector< std::string > sliceFilePaths );

	//! Read a 2D slice and convert to VtkImage (this method does not do reorienting because it is just for 2D image)
	template <class PixelType>
	static vtkSmartPointer<vtkImageData> Read2DVtkImageFromFile( 
		const char *imageFileName );

private:

	//! Read dicom image to the itkImage 
	template< typename ImageType > 
	static typename ImageType::Pointer itkReadMultiSliceImageFromFiles(
		const std::vector< std::string > &sliceFilePaths );

};

} // namespace dcmAPI

#include "dcmImageUtilities.txx"

#endif // _dcmImageUtilities_h
