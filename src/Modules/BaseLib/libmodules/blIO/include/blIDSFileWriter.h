/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blIDSFileWriter_h
#define _blIDSFileWriter_h


// basic includes
#include <string>
using std::string;

#include <blLightObject.h>
#include <blSmartPointer.h>
#include <blMacro.h>
#include "blITKImagesDefines.h"

#include <vtkImageReader.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>

#include "BaseLibVTKWin32Header.h"


/**
\brief Class that writes an arbitrary image to a ids file.
\ingroup CardiacSegmentationPlugin
\author Chiara Riccobene & Martin Bianculli
\date August 2009
*/

class BASELIBVTK_EXPORT blIDSFileWriter: public blLightObject
{
	public:
		// Typedefs
		typedef blIDSFileWriter Self;
		typedef blSmartPointer< Self > Pointer;
		typedef base::UnsignedShortVolumeType OutputImageType;

		// Methods
		/// \brief Static constructor.
		blNewMacro( Self );

		/// \brief Set the file name.
		void SetFileName( string fileName );

		/// \brief Write the IDS to the file specified using the method SetFileName.
		virtual void Update() ;

		/// \brief Set the VTK object to be written to the specified IDS file, before Update() has been called.
		void SetInput(vtkImageData* dataObj) ;
		

	protected:
		/// \brief Default Constructor.
		blIDSFileWriter();

		/// \brief Destructor.
		virtual ~blIDSFileWriter();

		/// \brief Get the file name.
		string GetFileName();

	protected:
		//-----------------------------------------------------
		// Variables
		//-----------------------------------------------------
		string	m_fileName;

		//!
		vtkSmartPointer<vtkImageData>	vtkImageObj; //input image for the ids file
		//!
		OutputImageType::ConstPointer	itkVolumeObj;

	private:

		//!purposely not implemented
		blIDSFileWriter( const blIDSFileWriter& );	
		
		//!purposely not implemented
		void operator = ( const blIDSFileWriter& );
};

#endif // _blIDSFileWriter_h
