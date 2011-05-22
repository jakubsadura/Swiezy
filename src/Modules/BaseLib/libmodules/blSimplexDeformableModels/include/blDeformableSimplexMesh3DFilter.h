/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef BLDEFORMABLESIMPLEXMESH3DFILTER_H
#define BLDEFORMABLESIMPLEXMESH3DFILTER_H

// std
#include <set>

// ITK
#include <itkMeshToMeshFilter.h>
#include <itkImage.h>
#include <itkCovariantVector.h>
#include <itkVector.h>
#include <itkSphereSpatialFunction.h>
#include <itkFloodFilledSpatialFunctionConditionalIterator.h>
#include <itkVectorGradientMagnitudeImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkArray.h>

// VTK
// While ITK people don't implement the point/cell locator...
#include <vtkCellLocator.h>
#include <vtkPointLocator.h>

// BaseLib
#include "blSimplexMeshGeometry.h"

/** \class DeformableSimplexMesh3DFilter
* \brief Three-dimensional deformable model for image segmentation
*
* DeformableSimplexMesh3DFilter is a discrete three-dimensional deformable model, which 
* can be used to deform a 3-D SimplexMesh. 
*
* The mesh deformation is constrained by internal forces. The internal force can be scaled 
* via SetAlpha (typical values are 0.01 < alpha < 0.3). The external force is derived from 
* the image one wants to delineate. Therefore an image of type GradientImageType needs to 
* be set by calling SetGradientImage(...). The external forces are scaled 
* via SetBeta (typical values are 0.01 < beta < 1). One still needs to play around with 
* these values.
* 
* To control the smoothness of the mesh a rigidity parameter can be adjusted. Low values (1 or 0) 
* allow areas with high curvature. Higher values (around 7 or 8) will make the mesh smoother.
*
* By setting the gamma parameter the regularity of the mesh is controlled. Low values (< 0.03) 
* produce more regular mesh. Higher values ( 0.3 < gamma < 0.2) will allow to move the vertices to 
* regions of higher curvature.
*
* This approach for segmentation follows that of Delingette et al. (1997).
*
* This filter currently assumes that the spacing of the input image is 1. 
*
* The user has to set the number of iterations for mesh evolution.
* 
* \author Thomas Boettger. Division Medical and Biological Informatics, German Cancer Research Center, Heidelberg.
* 
* \ingroup MeshFilters
* \ingroup MeshSegmentation 
*/
template <class TInputMesh, class TOutputMesh>
class blDeformableSimplexMesh3DFilter : public itk::MeshToMeshFilter<TInputMesh, TOutputMesh>
{
public:
	/** Standard "Self" typedef. */
	typedef blDeformableSimplexMesh3DFilter  Self;

	/** Standard "Superclass" typedef. */
	typedef itk::MeshToMeshFilter<TInputMesh, TOutputMesh> Superclass;

	/** Smart pointer typedef support */
	typedef SmartPointer<Self>  Pointer;
	typedef SmartPointer<const Self>  ConstPointer;

	/// \brief Static constructor
	itkFactorylessNewMacro( Self );
	///** Method of creation through the object factory. */
	//itkNewMacro(Self);

	///** Run-time type information (and related methods). */
	//itkTypeMacro(blDeformableSimplexMesh3DFilter,MeshToMeshFilter);

	/** Some typedefs. */
	typedef TInputMesh InputMeshType;
	typedef TOutputMesh OutputMeshType;

	typedef typename InputMeshType::PointsContainer                 InputPointsContainer;
	typedef typename InputMeshType::PointsContainerConstPointer     InputPointsContainerConstPointer;
	typedef typename InputMeshType::PointsContainer::ConstIterator  InputPointsContainerConstIterator;

	/** Other definitions. */
	typedef typename InputMeshType::PointType             PointType;
	typedef typename PointType::VectorType                VectorType;
	typedef CovariantVector< 
		typename VectorType::ValueType, 3 >   CovariantVectorType;
	typedef typename InputMeshType::PixelType             PixelType;

	/** Image and Image iterator definition. */
	typedef CovariantVector<PixelType, 3>                   GradientType;
	typedef itk::Image<GradientType, 3>                          GradientImageType;
	typedef itk::Image<unsigned char, 3>                         BinaryOutput;      
	typedef itk::Image<float, 3>                                 MagnitudeOutput;

	typedef typename GradientImageType::Pointer             GradientImagePointer;
	typedef typename GradientImageType::IndexType           GradientIndexType;
	typedef typename GradientImageType::PixelType           GradientPixelType;
	typedef typename GradientIndexType::IndexValueType      GradientIndexValueType;
	typedef typename GradientImageType::SizeType            GradientImageSizeType;

	/* Mesh pointer definition. */
	typedef typename InputMeshType::Pointer         InputMeshPointer;
    typedef typename InputMeshType::ConstPointer    InputMeshConstPointer;
	typedef typename OutputMeshType::Pointer        OutputMeshPointer;

	typedef typename InputMeshType::PointType                     MeshPointType;
	typedef typename InputMeshType::CellsContainerPointer         CellsContainerPointer;
	typedef typename InputMeshType::CellsContainer::Iterator      CellsContainerIterator;
	typedef typename InputMeshType::NeighborListType              InputNeighbors;
	typedef typename InputMeshType::NeighborListType::iterator    InputNeighborsIterator;

	typedef typename TOutputMesh::PointsContainer                 OutputPointsContainer;
	typedef typename OutputPointsContainer::Pointer               OutputPointsContainerPointer;

	typedef itk::MapContainer<unsigned long, std::set<unsigned long> > VertexNeighborListType;
	typedef std::set<unsigned long>      NeighborSetType;
	typedef std::set<unsigned long>      IndexSetType;
	typedef typename NeighborSetType::iterator    NeighborSetIterator;
	typedef typename IndexSetType::iterator       IndexSetIterator;

	typedef std::vector<double>	                                CrossSectionMeanMinDistType;
	typedef vnl_matrix<unsigned long>							CrossSectionMatrixType;

	typedef typename InputMeshType::GeometryMapType   GeometryMapType;
	typedef typename GeometryMapType::Pointer         GeometryMapPointer;
	typedef typename GeometryMapType::Iterator        GeometryMapIterator;

	typedef			std::pair<unsigned long ,bool>						  FixedPointsPairType;
	typedef			std::map<unsigned long ,bool>						  FixedPointsMapType;

	/** Routines. */

	/** Set/Get routines. */

	itkSetMacro(CrossSectionCenterThreshold, double);
	itkGetMacro(CrossSectionCenterThreshold, double);

	/** 
	*  Setter for gradient image
	*/
	itkSetMacro(Gradient, GradientImagePointer);

	/** 
	*  Getter for gradient image
	*/
	itkGetMacro(Gradient, GradientImagePointer);

	/** 
	*Set number of iterations for deformation process  
	*/
	itkSetMacro(Iterations, int);
	itkGetMacro(Iterations, int);

	/** Set scalar factor for internal force */
	itkSetMacro(Alpha, double);

	/** Get internal force scaling factor */
	itkGetMacro(Alpha, double);

	/** Set external force scaling factor */
	itkSetMacro(Beta, double);

	/** Get external force scaling factor */
	itkGetMacro(Beta, double);

	/** Set reference metrics update scaling factor */
	itkSetMacro(Gamma, double);

	/** Get reference metrics update scaling factor */
	itkGetMacro(Gamma, double);

	/** Set reference metrics update scaling factor */
	itkSetMacro(Damping, double);

	/** Get reference metrics update scaling factor */
	itkGetMacro(Damping, double);

	/** control smoothness of the mesh */
	itkSetMacro(Rigidity, unsigned int);

	/** control smoothness of the mesh */
	itkGetMacro(Rigidity, unsigned int);

	itkSetObjectMacro(Data, GeometryMapType );
	itkGetObjectMacro(Data, GeometryMapType );

	itkSetMacro(DistanceToSurfaceTolerance, double);
	itkGetMacro(DistanceToSurfaceTolerance, double);

	itkSetMacro(Units, double);
	itkGetMacro(Units, double);

	/** Width, height and depth of the image */
	itkGetMacro(ImageWidth,int);
	itkGetMacro(ImageHeight,int);
	itkGetMacro(ImageDepth,int);

	/** current iteration number */
	itkGetMacro(Step, int);

	void SetSurface(vtkPolyData *surf);
	vtkPolyData *GetSurface(){return this->surf;};

	void SetCenterline(vtkPolyData *vtkPDCenterline);

	void SetCheckPoints(bool checkPoints);

protected:
	blDeformableSimplexMesh3DFilter();
	~blDeformableSimplexMesh3DFilter();
	blDeformableSimplexMesh3DFilter(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented

	virtual void PrintSelf(std::ostream& os, Indent indent) const;


	/** */
	virtual void GenerateData();

	virtual double GetCorrespondingCrossSectionMeanDist(unsigned long idx);

	/**
	* Initializes the data structures necessary for mesh 
	* deformation with the values from the passed input 
	* mesh.
	*/
	virtual void Initialize();

	/**
	* Compute geometric properties like curvature and 
	* normals, which are necessary for the computation 
	* of the internal force components for each point of the mesh.
	*/
	virtual void ComputeGeometry();

	/**
	* \brief Computes the displacement of each point. Therefore 
	* internal and external forces are computed and multiplied 
	* by the constants (alpha and beta) set by the user.
	*/
	virtual void ComputeDisplacement();

	/**
	* \brief Compute the component resultant from the newtonian law of motion
	*/
	virtual void ComputeNewtonianLawOfMotion(unsigned long pointId, blSimplexMeshGeometry *data);

	/**
	* \brief Compute the internal force component
	*/
	virtual void ComputeSmoothingForce(unsigned long pointId, blSimplexMeshGeometry* data);

	/**
	* \brief Compute the external force component
	*/
	virtual void ComputeExternalForce(blSimplexMeshGeometry* data);  

	/**
	* At the end of the deformation the output mesh is created
	* by creating a new mesh
	*/
	virtual void ComputeOutput();

	/**
	* Method updates the reference metrics for each mesh point
	*/ 
	virtual void UpdateReferenceMetrics();

	/**
	*  L function implemented following the paper of Delingette
	*/
	double L_Func(double r,double d, double phi);

	/**
	*  Method computes the barycentric coordinates of the passed point 
	*/
	PointType ComputeBarycentricCoordinates(PointType p, blSimplexMeshGeometry* data);

	/** Update the mean minimum distance to surface of each section */
	void UpdateMeanMinimumDistance();

	/** Parameters definitions. */

	/**
	* Scalar defining the influence of the internal forces
	* Values should lie between 0.001 and 0.3. Higher values
	* increase the stiffness of the mesh
	*/
	double    m_Alpha;

	/**
	* Scalar defining the influence of the external force components
	* The choice for this parameter strongly depends on the underlying 
	* data. Typical value range from 0.00001 to 0.3
	* 
	*/
	double    m_Beta;

	/**
	* Gamma influences the distribution of the mesh points. It should 
	* lie between 0.01 and 0.2. Smaller values force the mesh to be 
	* more regular. When increasing gamma, mesh points will have higher
	* density in places of high curvature.
	*/
	double    m_Gamma;      

	/**
	* This scalar defines the damping factor in the discretized equation of Delingette to compute 
	* the evolution of the simplex mesh under the law of motion. This parameters prevents oscillations 
	* of the system.
	* In this equation unitary mass and step size are considered.
	* In practice we choose a value of 0.65.
	**/
	double    m_Damping;

	/**
	* This scalar determines the smoothness of the surface model. Values
	* should range from 0 to 10. It determines the radius of the neighborhood
	* during internal force computation using the curvature shape contraint. 
	* The higher the rigidity the higher the smoothness.
	*/
	unsigned int m_Rigidity;

	// definition of internal parameters
	/** Number of iterations */
	int       m_Step;          
	/** Image size */
	int       m_ImageWidth;      
	/** Image size */
	int       m_ImageHeight;
	/** Image size */
	int       m_ImageDepth;

	/** This threshold decides when to stop the model. */
	int       m_Iterations;

	// \brief With this tolerance, the points are stopped such that they don't cross the surface.
	// If this does not work (e.g., step is too big), I can use dot product to know 
	// on which side of the surface I'am and move the point backwards.
	double m_DistanceToSurfaceTolerance;

	/**
	* map stores a Geometry object for every mesh point 
	*/
	GeometryMapPointer m_Data;

	FixedPointsMapType m_FixedPoints;

	/* gradient image pointer */
	GradientImagePointer m_Gradient;

	// \brief Attracting surface.
	vtkDataSet *surf;

	// \brief Centerline that generated the initial mesh configuration
	vtkPolyData *centerline;

	// \brief VTK Cell locator used to find closest cells in the surface.
	vtkCellLocator *locator;

	vtkPointLocator *centerlineLocator;

	CrossSectionMeanMinDistType m_CrossSectionMeanMinDist;

	double m_CrossSectionCenterThreshold;
	double m_AverageSurfaceCurvature;
	double m_AverageFixedDistanceToCenterline;
	// \brief Contains the average distance of the points in the mesh to the centerline.
	double m_AverageDistanceToCenterline;

	double m_Units;

	bool checkPoints;

}; // end of class


#ifndef ITK_MANUAL_INSTANTIATION
#include "blDeformableSimplexMesh3DFilter.txx"
#endif

#endif // BLDEFORMABLESIMPLEXMESH3DFILTER_H

