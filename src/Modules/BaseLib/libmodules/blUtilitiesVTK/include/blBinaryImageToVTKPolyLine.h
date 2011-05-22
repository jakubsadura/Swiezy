// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __blBinaryImageToVTKPolyLine_h
#define __blBinaryImageToVTKPolyLine_h

// ITK
#include <itkLightObject.h>
#include <itkImage.h>
#include <itkImageRegionIterator.h>

// VTK
#include <vtkPolyData.h>

// blLib
#include "BaseLibVTKWin32Header.h"

using namespace itk;

/**
* \brief 
*
*/
class BASELIBVTK_EXPORT blBinaryImageToVTKPolyLine: public LightObject
{
	public:

		//-----------------------------------------------------
		// Typedefs
		//-----------------------------------------------------

		typedef blBinaryImageToVTKPolyLine Self;
		typedef LightObject                    Superclass;
		typedef SmartPointer<Self>             Pointer;
		typedef SmartPointer<const Self>       ConstPointer;

		const static int kDIM_3D = 3;
		typedef itk::Image<unsigned char, kDIM_3D >		BinaryImageType;
		typedef BinaryImageType::Pointer				BinaryImagePointer;
		typedef itk::ImageRegionIterator<BinaryImageType>  BinaryImageIteratorType;

		//-----------------------------------------------------
		// Methods
		//-----------------------------------------------------

		/// \brief Static constructor
		itkFactorylessNewMacro(Self);

		/// \brief Set input VTK PolyLine to be converted into ITK PolyLine.
		void SetInput( BinaryImagePointer inputCenterline )
			{ this->itkCenterline = inputCenterline; }

		/// \brief Get output VTK PolyLine.
		vtkPolyData *GetOutput()
			{ return this->outputPolyData; }

		void Update();


	protected:

		blBinaryImageToVTKPolyLine();
		~blBinaryImageToVTKPolyLine();

		//-----------------------------------------------------
		// Variables
		//-----------------------------------------------------

		BinaryImagePointer itkCenterline;
		vtkPolyData *outputPolyData;
	private:

		blBinaryImageToVTKPolyLine( const blBinaryImageToVTKPolyLine& );  //purposely not implemented
		void operator = ( const blBinaryImageToVTKPolyLine& );  //purposely not implemented
};

#endif //__blBinaryImageToVTKPolyLine_h
