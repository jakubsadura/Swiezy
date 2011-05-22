/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef BLSIMPLEXTUBEMESHCREATOR_H
#define BLSIMPLEXTUBEMESHCREATOR_H

// ITK
#include <itkMesh.h>
#include <itkLineCell.h>
#include <itkVertexCell.h>
#include <itkMapContainer.h>
#include <itkMeshToMeshFilter.h>
#include <itkVectorContainer.h>
#include <itkNumericTraits.h>
#include <itkPolygonCell.h>
#include <itkDefaultDynamicMeshTraits.h>
#include <vnl/vnl_cross_product_matrix.h>

// VTK
#include <vtkCellLocator.h>
#include <vtkPolyData.h>

// BaseLib
#include "blSimplexPatchHelper.h"
#include "blSimplexTubeMesh.h"

/**
* \class blSimplexTubeMeshCreator
* \brief This filter creates a simplex mesh around a polyline.
* All the mesh points are stored using a cyclic matrix, where the top and bottom rows are neighbors. This 
* means that for a mesh of N x M nodes, the first row (0) is neighbor of the last row (N-1), i.e., 
* that row 0 and N are the same. 
* To create the mesh, first, all the nodes are created and posteriorly the connectivities between 
* them are added.
* To easilly locate previously created nodes, their Ids are stored in a N x M matrix.  
* When this object is updated, it first generates the points and then the simplex mesh using these points. 
* The generation of the simplex mesh is handled at patch level, where all the points within a simplex patch 
* are known (allowing create the links properlly).
* By the way the points in the mesh are generated, the grid is regular and does not look like a "normal" 
* simplex mesh after the generation. Nevertheless, as observed after the application of any simplex deformation filter,
* the mesh quickly recovers its regularity properties, where the deformation process makes it smooth again.
* In order to preserve the simplex mesh topology, the boundaries of the mesh need to be taken care specially. 
* A 2-simplex mesh needs to have each point of the mesh connected with three other neighbor points. By the way 
* the mesh is created, this is automatically done in all the mesh BUT the boundaries, where some neighbors 
* are missing. This is solved by eliminating half of the points in the external simplex mesh boundary. For 
* example, is the mesh has 12 points in one sweep, then, for the boundary column of points, only 6 points 
* are generated. This ensures that when the simplex mesh conectivity is generated, all the neighbors will 
* have exactly 3 neighbors.
* 
* \author Nacho Larrabide. CILab - Computational Imaging Laboratory, UPF - Barcelona, Spain.
* 
*/
template <class TInputMesh, class TOutputMesh>
class blSimplexTubeMeshCreator : public itk::MeshToMeshFilter<TInputMesh, TOutputMesh>
{
public:
	typedef blSimplexTubeMeshCreator Self;
	typedef itk::SmartPointer< Self > Pointer;
	typedef itk::MeshToMeshFilter<TInputMesh, TOutputMesh> Superclass;

	/// \brief Static constructor
	itkFactorylessNewMacro( Self );

	typedef TInputMesh                                                  InputMeshType;
	typedef typename TInputMesh::Pointer                                InputMeshPointer;
	typedef typename TInputMesh::PointType                              InputPointType;
	typedef typename TInputMesh::BoundaryAssignmentsContainerPointer    InputBoundaryAssignmentsContainerPointer;
	typedef typename TInputMesh::PointsContainer                        InputPointsContainer;
	typedef typename InputPointsContainer::ConstPointer                 InputPointsContainerConstPointer;
	typedef typename InputPointsContainer::ConstIterator                InputPointsContainerConstIterator;
	typedef typename TInputMesh::CellType                               InputCellType;
	typedef typename InputCellType::CellAutoPointer                     InputCellAutoPointer;
	typedef          itk::LineCell<InputCellType>                       LineType;

	typedef			 TOutputMesh										OutputMeshType;
	typedef typename TOutputMesh::Pointer			   					OutputMeshPointer;
	typedef typename TOutputMesh::PointType								OutputPointType;
	typedef typename TOutputMesh::PixelType                             OutputPixelType;
	typedef typename TOutputMesh::PointsContainer                       OutputPointsContainer;
	typedef typename OutputMeshType::CrossSectionMatrixType				OutputCrossSectionMatrixType;
	typedef typename OutputMeshType::PointToCenterMapType 				OutputPointToCenterMapType;
	typedef typename OutputMeshType::PointToCenterMapPointer 			OutputPointToCenterMapPointer;
	typedef typename OutputPointsContainer::Pointer                     OutputPointsContainerPointer;
	typedef typename TOutputMesh::PointsContainer::Iterator             OutputPointsContainerIterator;
	typedef	typename TOutputMesh::CellAutoPointer					    OutputCellAutoPointer;
	typedef typename TOutputMesh::CellType                              OutputCellType;
	typedef          itk::PolygonCell<OutputCellType>                   OutputPolygonType;

	typedef	blSimplexPatchHelper<TOutputMesh>							PatchHelperType;
	typedef	typename PatchHelperType::Pointer			 			    PatchHelperPointer;

	typedef	typename std::vector<PatchHelperPointer>					PatchHelperVectorType;
	typedef	typename std::vector<PatchHelperVectorType>					PatchHelperMatrixType;

	typedef			 vnl_matrix<unsigned long>							PointIndexMatrixType;
	typedef			 std::vector<unsigned long>							PointIndexListType;

	// \brief Returns the pointer to the Point Index Matrix generated
	PointIndexMatrixType *GetPointIndexMatrix(){return this->pointIndexMatrix;};

	// \brief Set/Get the units conversion factor (m, mm, etc.).
	void SetRadialYoungModulus(double radialYoungModulus);
	double GetRadialYoungModulus() {return this->radialYoungModulus;};

	// \brief Set/Get the units conversion factor (m, mm, etc.).
	void SetUnits(double units);
	double GetUnits() {return this->units;};

	// \brief Set/Get the initial radius of the tube.
	void SetRadius(double r);
	double GetRadius() {return this->r;};

	// \brief Set/Get the final radius of the tube. NOTE: this is not the optimal solution, the creator should not care about this.
	void SetFinalRadius(double finalRadius);
	double GetFinalRadius() {return this->finalRadius;};

	// \brief Set/Get polyline start point from which the mesh is going to be generated.
	void SetStart(unsigned int start);
	unsigned int GetStart() {return this->start;};

	// \brief Set/Get polyline start point from which the mesh is going to be generated.
	void SetCentralPoint(unsigned int centralPoint);
	unsigned int GetCentralPoint() {return this->centralPoint;};

	// \brief Set/Get polyline start point from which the stent mesh is going to be generated.
	void SetTubeLength(double tubeLength);
	unsigned int GetTubeLength() {return this->tubeLength;};

	void SetPatchesOnSweep(unsigned long patchesOnSweep);
	unsigned long GetPatchesOnSweep() {return this->patchesOnSweep;};

	void SetPatchesOnLength(unsigned long patchesOnLength);
	unsigned long GetPatchesOnLength() {return this->patchesOnLength;};

	// \brief Set the surface associated to centerline where the tube will be placed.
	// This surface is used to compute the minumum tube radius.
	void SetCenterlineOriginalSurface(vtkPolyData *surf);

	void SetCenterlineAsPD(vtkPolyData *cl)
	{ this->vtkCL = cl;}

	void ComputeStartingPointIdFromCenterPointId();

	// NOTE: where is best to put this method?, can I put it only in the child class?
	// how do I isert the shift angle from the child class in the point generation from 
	// the parent class?

	// \brief Set/Get the angle shift of used to generate the stent. Used to 
	// initialize the stent in different positions. Should be between 0 and 2*pi.
	void SetAngleShift(double angleShift) {this->angleShift = angleShift;};
	double SetAngleShift() {return this->angleShift;};

protected:

	blSimplexTubeMeshCreator();

	~blSimplexTubeMeshCreator();

	blSimplexTubeMeshCreator(const Self&) {}

	void operator=(const Self&) 
	{
	}

	/**
	* \brief Print self method
	*/
	virtual void PrintSelf(std::ostream& os, Indent indent) const;

	/** 
	* \brief Override from ProcessObject
	*/
	virtual void GenerateData();

	/** 
	* \brief Initializes all necessary data structures
	*/
	void Initialize();

	/*
	* \brief Walk over all the points and create the cells
	*/
	void CreateSimplexCells();

	/** 
	* \brief Method that inserts the new computed simplex points into the output mesh
	*/
	int CreateSimplexPoints();

	/**
	* \brief Verify the integrity of the centerline and the stent (stent length vs. centerline length, etc.).
	*/
	int PreprocessCenterline();

	/**
	* \brief Cell auto pointer used in the generation of the mesh.
	*/
	OutputCellAutoPointer  NewSimplexCellPointer;

	/**
	* \brief Matrix containing the indexes of all the points created. Algorithmic data.
	*/
	PointIndexMatrixType *pointIndexMatrix;
	PatchHelperMatrixType patchMatrix;

	// \brief VTK Cell locator used to find closest cells in the surface.
	vtkCellLocator *locator;
	vtkPolyData *surf;
	vtkPolyData *vtkCL;

	double angleShift; // Angle shift used to start generating the stent. This is used to release the stent in different positions.
	double tubeLength; // Stent length in mm

	unsigned int start;
	unsigned int centralPoint;
	unsigned long Rows,Cols;
	unsigned long patchesOnSweep, patchesOnLength;
	unsigned int simplexExtraPoints;
	double patchAngle;// The length of one cell.
	double patchLength;// The length of one cell.
	double units;
	double radialYoungModulus;
	double maxExpansion;

	double dx;
	double dTheta;
	double r; // Radius of the initial mesh
	double maxDist; // Radius of the initial mesh
	// \brief The final radius is the initial radius (r) times this factor (final radius)
	double finalRadius;

	OutputMeshPointer stentMesh;
};

#ifndef ITK_MANUAL_INSTANTIATION
#include "blSimplexTubeMeshCreator.txx"
#endif

#endif // BLSIMPLEXTUBEMESHCREATOR_H
