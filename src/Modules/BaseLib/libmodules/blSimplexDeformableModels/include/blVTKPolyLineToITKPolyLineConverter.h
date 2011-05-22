/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef BLVTKPOLYLINETOITKPOLYLINE_H
#define BLVTKPOLYLINETOITKPOLYLINE_H

// ITK
#include <itkLightObject.h>
#include <itkDefaultDynamicMeshTraits.h>
#include <itkMesh.h>
#include <itkLineCell.h>

// VTK
#include <vtkPolyData.h>

// BaseLib
#include "BaseLibVTKWin32Header.h"

using namespace itk;

/**
* \class blVTKPolyLineToITKPolyLineConverter
* \brief 
*/
class BASELIBVTK_EXPORT blVTKPolyLineToITKPolyLineConverter: public LightObject
{
	public:

		//-----------------------------------------------------
		// Typedefs
		//-----------------------------------------------------

		typedef blVTKPolyLineToITKPolyLineConverter  Self;
		typedef LightObject                    Superclass;
		typedef SmartPointer<Self>             Pointer;
		typedef SmartPointer<const Self>       ConstPointer;

		typedef itk::DefaultDynamicMeshTraits< double, 3, 3, double, double >   PolyLineMeshTraits;
		typedef itk::Mesh< double, 3, PolyLineMeshTraits >                      PolyLineMeshType;
		typedef PolyLineMeshType::Pointer                                       PolyLineMeshPointer;
		typedef PolyLineMeshType::CellType                                      PolyCellType;
		typedef itk::LineCell< PolyCellType >                                   PolyLineCellType;


		//-----------------------------------------------------
		// Methods
		//-----------------------------------------------------

		/// \brief Static constructor
		itkFactorylessNewMacro(Self);

		/// \brief Set input VTK PolyLine to be converted into ITK PolyLine.
		void SetInput( vtkPolyData *inputCenterline )
			{ this->vtkCenterline->DeepCopy( inputCenterline ); }

		/// \brief Set centerline smoothing radius.
		void SetCenterlineSmoothingRadius( const unsigned int inputCenterlineSmoothingRadius )
			{ this->centerlineSmoothingRadius = inputCenterlineSmoothingRadius; }

		/// \brief Get output VTK PolyData centerline.
		vtkPolyData* GetOneCellCenterline()
			{ return this->vtkPDCenterline; }

		/// \brief Get output VTK PolyLine.
		PolyLineMeshType::Pointer GetOutput()
			{ return this->polyLineMesh; }

		void Update();


	protected:

		blVTKPolyLineToITKPolyLineConverter();
		~blVTKPolyLineToITKPolyLineConverter();

		//-----------------------------------------------------
		// Variables
		//-----------------------------------------------------

		vtkPolyData *vtkCenterline;
		vtkPolyData *vtkPDCenterline;
		PolyLineMeshType::Pointer polyLineMesh;
		unsigned int centerlineSmoothingRadius;


	private:

		blVTKPolyLineToITKPolyLineConverter( const blVTKPolyLineToITKPolyLineConverter& );  //purposely not implemented
		void operator = ( const blVTKPolyLineToITKPolyLineConverter& );  //purposely not implemented
};

#endif // BLVTKPOLYLINETOITKPOLYLINE_H

