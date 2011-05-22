/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef BLSIMPLEXTOPOLYDATA_H
#define BLSIMPLEXTOPOLYDATA_H

// ITK
#include <itkLightObject.h>
#include <itkDefaultDynamicMeshTraits.h>

// VTK
#include <vtkPolyData.h>

// BaseLib
#include "BaseLibVTKWin32Header.h"
#include "blSimplexMeshGeometry.h"
#include "blSimplexTubeMesh.h"

using namespace itk;

/**
* \class blSimplexToPolyDataGenerator
* \brief 
*/
class BASELIBVTK_EXPORT blSimplexToPolyDataGenerator: public LightObject
{
public:

	//-----------------------------------------------------
	// Typedefs
	//-----------------------------------------------------

	typedef blSimplexToPolyDataGenerator           Self;
	typedef LightObject                    Superclass;
	typedef SmartPointer<Self>             Pointer;
	typedef SmartPointer<const Self>       ConstPointer;

	typedef DefaultDynamicMeshTraits< double, 3, 3, double, double >   PolyLineMeshTraits;
	typedef Mesh< double, 3, PolyLineMeshTraits >                      PolyLineMeshType;
	typedef PolyLineMeshType::Pointer                                  PolyMeshPointer;

	typedef itk::DefaultDynamicMeshTraits< double, 3, 3, double, double >   SimplexMeshTraits;
	typedef blSimplexTubeMesh< double, 3, SimplexMeshTraits >               SimplexMeshType;
	typedef SimplexMeshType::PointsContainer					            SimplexPointsContainer;
	typedef SimplexMeshType::Pointer                                        SimplexMeshPointer;
	typedef SimplexPointsContainer::Pointer                                 SimplexPointsContainerPointer;
	typedef SimplexPointsContainer::Iterator                                SimplexPointsContainerIterator;

	//-----------------------------------------------------
	// Methods
	//-----------------------------------------------------

	/// \brief Static constructor
	itkFactorylessNewMacro(Self);

	/// \brief Set input ITK mesh to be converted to VTK mesh.
	void SetInput( SimplexMeshPointer inputMesh )
	{ this->itkMesh = inputMesh; }

	/// \brief Select the iteration that we are interested in. If none is indicated, the last one is used.
	void SelectIteration(long iteration)
	{this->iteration = iteration;}

	/// \brief Get output mesh as VTK unstructured grid data structure.
	vtkPolyData* GetOutput()
	{ return this->vtkMeshRepresentation; }

	void Update();


protected:

	blSimplexToPolyDataGenerator();
	~blSimplexToPolyDataGenerator();

	//-----------------------------------------------------
	// Variables
	//-----------------------------------------------------

	SimplexMeshPointer itkMesh;
	vtkPolyData *vtkMeshRepresentation;
	long iteration;

private:

	blSimplexToPolyDataGenerator( const blSimplexToPolyDataGenerator& );  //purposely not implemented
	void operator = ( const blSimplexToPolyDataGenerator& );  //purposely not implemented
};

#endif // BLSIMPLEXTOPOLYDATA_H
