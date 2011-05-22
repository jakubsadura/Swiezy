/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blIDSFileReader_h
#define _blIDSFileReader_h

// basic includes
#include <vtkImageReader.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>

// BaseLib
#include "BaseLibVTKWin32Header.h"
#include "blITKImagesDefines.h"
#include <blLightObject.h>
#include <blMacro.h>

/**
\brief Loads ids image files and creates a vtk object (structured points) that can be retrieved.

\author Chiara Riccobene & Martin Bianculli 
\date Aug 2009
*/

class BASELIBVTK_EXPORT blIDSFileReader: public blLightObject
{
	public:
		//! Typedefs
		typedef blIDSFileReader Self;
		typedef blSmartPointer< Self > Pointer;

		//! Static constructor.
		blNewMacro( Self );

		//! Set the file name.
		void SetFileName( std::string fileName );

		//! Read the image from a file, once specified with SetFileName() as well as other optional parameters such as scaling, m_spacing, origin, m_dimensions and verbose execution.
		void Update();

		//! Get the VTK object from the specified IDS file, once Update() has been called.
		vtkSmartPointer<vtkDataObject> GetVTKOutput();

		//! Get the ITK volumetric object from the specified IDS file, once Update() has been called.
		base::UnsignedShortVolumeType::Pointer GetOutput();

	protected:
		
		//! brief Default Constructor.
		blIDSFileReader();

		//! Destructor.
		virtual ~blIDSFileReader();

		//! Get the file name.
		std::string GetFileName();

	private:

		//! Purposely not implemented
		blIDSFileReader( const blIDSFileReader& );	

		//! Purposely not implemented
		void operator = ( const blIDSFileReader& );	

		//! A function for reading the ics header file.
		/*!
			It only reads the dimensions and the spacing of the image
		\param filename [in] the ics filename, without the extension (without .ics)
		\param  dimension [out] the dimensions of the image in a vector. dimensions[0]=x, dimensions[1]=y, dimensions[2]=z
		\param  spacing [out] the image spacing
		\return 0 if failed, 1 if OK
		\sa ReadIDSFile
		*/
		int ReadISCFile( std::string filename, unsigned int dimension[3], float spacing[3] );

		//! A function for reading the ids header file. 
		/*!
		\param dimensions [in] the dimensions of the image
		\param  spacing [in] the spacing of the image
		\return 0 if failed, 1 if OK
		\sa ReadISCFile
		*/
		int ReadIDSFile( unsigned int * dimensions, float * spacing );

	private:
		//! The filename, without any extension
		std::string	m_fileName;
		
		//!
		vtkSmartPointer<vtkImageData> vtkImageObj;	// resulting 3D vtk image object containing the read data from a IDS file
		//!
		base::UnsignedShortVolumeType::Pointer	itkVolumeObj;	// resulting 3D itk image object containing the read data from a v3d file
};


#endif // _blIDSImageFileReader_h
