/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLVECTORFIELDUTILS_h
#define BLVECTORFIELDUTILS_h

#include "BaseLibVTKWin32Header.h"

#include "CILabExceptionMacros.h"
#include <exception>
#include <string>
#include "vtkType.h"
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"

/**
* \brief Static class with some utilities.
* \ingroup blDisplacementFieldUtilities
* \date 08 Agu 2008
* \author Martin Bianculli
*/
class BASELIBVTK_EXPORT blVectorFieldUtils
{
	public:

	typedef vtkSmartPointer<vtkPolyData> vtkPolyDataPtr;

	/** Enumeration of the valid image formats that blVectorFieldUtils can handle */
	enum VectorFieldFormats { UnknownType, MHDVectorFieldType };
 
	/// Checks that the extension is correct
	static bool CheckLoadFileExtension(const char *vectorFieldFileName);

	/**\brief compares the filename extension with vtk valid types
	*  \param checkReadingOfFile if set, the function attempts loading them to verify the type of image that it represents
	*  \param shapeFileName file name
	*  \return an enumeration field describing what format the file contains (ImageFormats)
	*/
	static VectorFieldFormats GetTypeOfVectorFieldData(const char *vectorFieldFileName, bool checkReadingOfFile = true);

	/**\brief reads an image from file (automatically adapts to .vtk) and retrieves a vtkStructuredPoints
	*  \param vectorFieldFileName file name
	*  \return input shape (vtkStructuredPoints*)
	* You need to call Delete( ) to the output of this function to 
	* destroy the object
	*/
	static vtkPolyData* LoadVectorFieldFromFileAsVTK(const char *vectorFieldFileName);

private:
	/**\brief Creates a glyph from a given ImageData
	*  \param imageData vtkImageData
	* You need to call Delete( ) to the output of this function to 
	* destroy the object
	*/
	static vtkPolyData* CreateGlyph(const char *vectorFieldFileName);

};

#endif // BLVECTORFIELDUTILS_h
