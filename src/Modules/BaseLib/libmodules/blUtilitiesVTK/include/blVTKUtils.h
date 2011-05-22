/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blVTKUtils_h
#define __blVTKUtils_h


	//---------------------------------------------------
	// HEADERS
	//---------------------------------------------------

	#include "BaseLibVTKWin32Header.h"
	#include <vector>


	//---------------------------------------------------
	// FORWARD DECLARATIONS
	//---------------------------------------------------

	namespace itk{
		template <class TPixel, unsigned int VImageDimension> class Image;
	}

	class vtkImageData;
	class vtkLookupTable;




	//---------------------------------------------------
	// CLASS DEFINITION
	//---------------------------------------------------


/// \ingroup blUtilitiesVTK
namespace blVTKHelperTools
{

	/**
	 * \brief Static class with some utilities.
	 * \ingroup blUtilitiesVTK
	 */
	class BASELIBVTK_EXPORT blVTKUtils
	{

	//-----------------------------------
	// OPERATIONS
	//-----------------------------------
	public:
		
		/**\brief type of short int image*/
		typedef itk::Image<short int, 3> Short2ImageType;
		
		/**\brief type of unsigned short int image*/
		typedef itk::Image<unsigned short int, 3> UShort2ImageType;

		/**\brief type of pointer to vtkImageData*/
		typedef vtkImageData* VTKImagePointer;



		/**
		 * \brief This method creates a new vtkImageData from a given itk image 
		 *	with pixel type Unsigned Short.
		 */
		static vtkImageData * ITK2VTK(Short2ImageType * itkImagePt);


		/**
		 * \brief This method creates a new 3D vtkImageData from a list of 2D slices
		 *	\param imageVector vector of slices
		 *	\param spacingBetweenSlices spacing between 2 consecutive slices
		 */
		static vtkImageData * Build3DVolume( 
								std::vector<VTKImagePointer> & imageVector,
								double spacingBetweenSlices);

		
		/**\brief NOT IMPLEMENTED*/
		static void Interpolate(const unsigned char * reds, 
								 const unsigned char * greens,
								 const unsigned char * blues,
								 unsigned int numberOfInputColors,
								 unsigned char * newReds,
								 unsigned char * newGreens,
								 unsigned char * newBlues,
								 unsigned int numberOfOutputColors);

		/**
		\brief Read the header of the file
		*/
		static void ReadHeader(const char *shapeFileName, std::string &strHeader);


};



}

#endif
