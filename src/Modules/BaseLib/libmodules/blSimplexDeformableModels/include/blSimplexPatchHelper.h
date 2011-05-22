/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef BLSIMPLEXPATCHHELPER_H
#define BLSIMPLEXPATCHHELPER_H

// std
#include <vector>

// ITK
#include <itkLightObject.h>
#include <itkLineCell.h>
#include <itkPolygonCell.h>

// VNL classes
#include <vnl/vnl_vector.h>
#include <vnl/vnl_matrix.h>

/**
* \class blSimplexPatchHelper
* \brief This class is responsible for creating the connections of points at the level 
* of a simplex patch. 
* As the connectivity within a patch is fixed, this class creates the connections between the 
* points in the patch using always the same patch points. This class has ben specifically 
* created as a helper in the creation of the simplex mesh and has no posterior use.
* 
* \author Nacho Larrabide. CILab - Computational Imaging Laboratory, UPF - Barcelona, Spain.
* 
*/
template <class TOutputMesh>
class blSimplexPatchHelper : public itk::LightObject
{
public:

	//-----------------------------------------------------
	// Typedefs
	//-----------------------------------------------------

	typedef blSimplexPatchHelper Self;
	typedef itk::SmartPointer< Self > Pointer;

	/// \brief Static constructor
	itkFactorylessNewMacro( Self );

	typedef typename TOutputMesh::Pointer                                 OutputMeshPointer;
	typedef typename TOutputMesh::PointType                               OutputPointType;
	typedef typename TOutputMesh::PixelType                               OutputPixelType;
	typedef typename TOutputMesh::PointsContainer                         OutputPointsContainer;
	typedef typename OutputPointsContainer::Pointer                       OutputPointsContainerPointer;
	typedef typename TOutputMesh::PointsContainer::Iterator               OutputPointsContainerIterator;

	typedef typename TOutputMesh::CellType                                OutputCellType;
	typedef typename TOutputMesh::CellAutoPointer                         OutputCellAutoPointer;
	typedef          itk::PolygonCell<OutputCellType>                     OutputPolygonType;

	typedef          itk::LineCell<OutputCellType>                        LineType;
	typedef          itk::PolygonCell<OutputCellType>                     PolygonType;
	typedef          itk::VertexCell<OutputCellType>                      VertexType;

	typedef			 vnl_matrix<unsigned long>							  PointIndexMatrixType;
	typedef			 std::vector<unsigned long>							  PointIndexListType;

	typedef typename blSimplexPatchHelper::Pointer						  CellPatchHelperPointer;

	/// \brief Get the number of points in patch height
	unsigned int GetPatchHeightPoints(){return this->patchHeightPoints;};

	/// \brief Get the number of points in patch width
	unsigned int GetPatchWidthPoints(){return this->patchWidthPoints;};

	/// \brief Set/Get the height of the matrix at which the patch starts
	void SetPatchHeightStart(unsigned int patchHeightStart){this->patchHeightStart = patchHeightStart;};
	long int GetPatchHeightStart(){return this->patchHeightStart;};

	/// \brief Set/Get the width of the matrix at which the patch starts
	void SetPatchWidthStart(unsigned int patchWidthStart){this->patchWidthStart = patchWidthStart;};
	long int GetPatchWidthStart(){return this->patchWidthStart;};

	/// \brief Set/Get the point index matrix
	void SetPointIndexMatrix(PointIndexMatrixType *pointIndexMatrix);
	PointIndexMatrixType *GetPointIndexMatrix(){return this->pointIndexMatrix;}

	// \brief Returns the internal ids of the patch
	PointIndexListType &GetInternalIds(){return this->internalIds;};

	/// \brief Set/Get bool that indicates if this is the last patch of the stent
	void SetIsLastPatch(bool isLastPatch) {this->isLastPatch = isLastPatch;};
	bool GetIsLastPatch() {return this->isLastPatch;};

	/// \brief Set/Get bool that indicates if this is the first patch of the stent
	void SetIsFirstPatch(bool isFirstPatch) {this->isFirstPatch = isFirstPatch;};
	bool GetIsFirstPatch() {return this->isFirstPatch;};

	// \brief Set the mesh.
	void SetMesh(OutputMeshPointer mesh){this->mesh = mesh;};

	// \brief Generate the links for a patch inside this cell.
	void GenerateData();

protected:

	/// \brief Default Constructor
	blSimplexPatchHelper();
	/// \brief Destructor
	virtual ~blSimplexPatchHelper();

	// \brief Initialize internal data structures
	void Initialize();

	//-----------------------------------------------------
	// Variables
	//-----------------------------------------------------
	unsigned int patchHeightPoints;
	unsigned int patchWidthPoints;
	long int patchHeightStart;
	long int patchWidthStart;

	PointIndexMatrixType *pointIndexMatrix;

	PointIndexListType internalIds;

	OutputMeshPointer mesh;

	bool isLastPatch;
	bool isFirstPatch;

	/** 
	* Auto pointer definition for creation of new simplex cells
	*/
	OutputCellAutoPointer  newSimplexCellPointer;

private:

	blSimplexPatchHelper( const blSimplexPatchHelper& );  //Purposely not implemented
	void operator = ( const blSimplexPatchHelper& );  //Purposely not implemented
};

#ifndef ITK_MANUAL_INSTANTIATION
#include "blSimplexPatchHelper.txx"
#endif 

#endif // BLSIMPLEXPATCHHELPER_H
