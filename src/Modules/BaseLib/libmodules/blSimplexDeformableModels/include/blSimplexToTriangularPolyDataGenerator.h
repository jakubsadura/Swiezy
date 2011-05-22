/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef BLSIMPLEXTOTRIANGLUARPOLYGONALDATA_H
#define BLSIMPLEXTOTRIANGLUARPOLYGONALDATA_H


// ITK
#include <itkLightObject.h>
#include <itkPolygonCell.h>

// VTK
#include <vtkPolyData.h>

// BaseLib
#include "BaseLibVTKWin32Header.h"
#include "blSimplexTubeMesh.h"

using namespace itk;

/**
* \class blSimplexToTriangularPolyDataGenerator
* \brief 
*/
class BASELIBVTK_EXPORT blSimplexToTriangularPolyDataGenerator: public LightObject
{
	public:
		//-----------------------------------------------------
		// Typedefs
		//-----------------------------------------------------

		typedef blSimplexToTriangularPolyDataGenerator        Self;
		typedef LightObject                    Superclass;
		typedef SmartPointer<Self>             Pointer;
		typedef SmartPointer<const Self>       ConstPointer;

		typedef itk::DefaultDynamicMeshTraits< double, 3, 3, double, double >   PolyLineMeshTraits;
		typedef itk::Mesh< double, 3, PolyLineMeshTraits >                      PolyLineMeshType;
		typedef PolyLineMeshType::Pointer                                       PolyMeshPointer;

		typedef itk::DefaultDynamicMeshTraits< double, 3, 3, double, double >   SimplexMeshTraits;
		typedef blSimplexTubeMesh< double, 3, SimplexMeshTraits >               SimplexMeshType;
		typedef SimplexMeshType::PointsContainer					            SimplexPointsContainer;
		typedef SimplexMeshType::Pointer                                        SimplexMeshPointer;
		typedef SimplexPointsContainer::Pointer                                 SimplexPointsContainerPointer;
		typedef SimplexPointsContainer::Iterator                                SimplexPointsContainerIterator;

		typedef SimplexMeshType::CellsContainerPointer					SimplexCellsContainerPointer;
		typedef SimplexMeshType::CellsContainerIterator					SimplexCellsContainerIterator;
		typedef	SimplexMeshType::CellAutoPointer						SimplexCellAutoPointer;
		typedef SimplexMeshType::CellType								SimplexCellType;
		typedef itk::PolygonCell<SimplexCellType>						SimplexPolygonType;

		typedef itk::DefaultDynamicMeshTraits< double, 3, 3, double, double >   TriangleMeshTraits;
		typedef itk::Mesh< double, 3, TriangleMeshTraits >                      TriangleMeshType;
		typedef TriangleMeshType::CellType                                      CellType; 
		typedef TriangleMeshType::CellsContainerPointer                         CellsContainerPointer;
		typedef TriangleMeshType::CellsContainerIterator                        CellsContainerIterator;
		typedef TriangleMeshType::PointType                                     PointType;

		//-----------------------------------------------------
		// Methods
		//-----------------------------------------------------
		/// \brief Static constructor
		itkFactorylessNewMacro(Self);

		/// \brief Set input ITK mesh to be converted to VTK mesh.
		void SetInput( SimplexMeshPointer inputMesh )
			{ this->itkMesh = inputMesh; }

		/// \brief Select the iteration that we are interested in. In none is indicated, the last one is used.
		void SelectIteration(long iteration){this->iteration = iteration;}

		/// \brief Get output mesh as VTK unstructured grid data structure.
		vtkPolyData* GetOutput()
			{ return this->vtkMeshRepresentation; }

		void Update();

	protected:

		blSimplexToTriangularPolyDataGenerator();
		~blSimplexToTriangularPolyDataGenerator();

		//-----------------------------------------------------
		// Variables
		//-----------------------------------------------------

		SimplexMeshPointer itkMesh;
		vtkPolyData *vtkMeshRepresentation;
		long iteration;

	private:

		blSimplexToTriangularPolyDataGenerator( const blSimplexToTriangularPolyDataGenerator& );  //purposely not implemented
		void operator = ( const blSimplexToTriangularPolyDataGenerator& );  //purposely not implemented
};

#endif // BLSIMPLEXTOTRIANGLUARPOLYGONALDATA_H
