/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef BLSIMPLEXTUBEMESH_H
#define BLSIMPLEXTUBEMESH_H

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

// std
#include <vector>
#include <algorithm>
#include <set>
#include <list>

// ITK
#include <itkMesh.h>
#include <itkVertexCell.h>
#include <itkTriangleCell.h>
#include <itkCellInterface.h>
#include <itkMapContainer.h>
#include <itkFixedArray.h>
#include <itkNumericTraits.h>
#include <itkPolygonCell.h>
#include <itkCovariantVector.h>

// BaseLib
#include "blSimplexMeshGeometry.h"

  /** \class blSimplexTubeMesh
  * \brief The class represents a 2-simplex tubular mesh. 
  *
  * A simplex mesh can be used for deformable model segmentation of 3D image data.
  * To create a simplex mesh one needs a triangle mesh, which can be converted 
  * to using the class itkTriangleMeshToSimplexMeshFilter. The back filtering 
  * (from simplex to triangle mesh) is done through a itkSimplexMeshToTriangleMeshFilter.
  * Inside these set classes, the class "blSimplexToTriangularPolyDataGenerator" allow 
  * to do this convertion.
  *
  *
  * \author Nacho Larrabide (adapted from itk Simplex Mesh class).
  */
  template <typename TPixelType, unsigned int VDimension = 3,
    typename TMeshTraits = DefaultStaticMeshTraits< TPixelType , VDimension, VDimension, TPixelType , TPixelType , TPixelType >
  >
  class blSimplexTubeMesh : public itk::Mesh<TPixelType, VDimension, TMeshTraits>
  {
  public:
    /** Standard typedefs. */
    typedef blSimplexTubeMesh                Self;

    /** Standard typedefs. */
	typedef itk::Mesh<TPixelType, VDimension, TMeshTraits>  Superclass;

    /** Standard typedefs. */
    typedef SmartPointer<Self>  Pointer;

    /** Standard typedefs. */
    typedef SmartPointer<const Self>  ConstPointer;

    /** Definition for array of indices*/
    typedef typename blSimplexMeshGeometry::IndexArray            IndexArray;

    /** Definition for a set of neighbor indices */
    typedef std::set<unsigned long>                             NeighborSetType;
    typedef typename NeighborSetType::iterator                  NeighborSetIterator;
    typedef std::vector<unsigned long>                          NeighborListType;

    /** */
    typedef typename TMeshTraits::PointType                     PointType;
    typedef typename TMeshTraits::PointIdentifier               PointIdentifier;
    typedef typename PointType::VectorType VectorType;
    typedef CovariantVector<typename VectorType::ValueType, 3>  CovariantVectorType;

    /** */

	typedef typename Superclass::CellsContainer::ConstPointer	CellsContainerConstPointer;
	typedef typename Superclass::CellsContainer::ConstIterator	CellsContainerConstIterator;
	typedef typename Superclass::CellType                       CellType;
    typedef typename CellType::CellAutoPointer                  CellAutoPointer;
    /** */
	typedef itk::PolygonCell<CellType>							PolygonType;
    typedef itk::LineCell<CellType>                             LineType;

	typedef vnl_matrix<unsigned long>							CrossSectionMatrixType;
	typedef std::vector<PointType>							    CrossSectionCenterVectorType;
	typedef std::vector<double>							        CrossSectionAverageToCenterType;

    /** map containing a SimplexMeshGeometry data object for each mesh point*/
	typedef itk::MapContainer<unsigned long, blSimplexMeshGeometry *>   GeometryMapType;
	typedef itk::MapContainer<unsigned long, unsigned long>				 PointToCenterMapType;

    /** smart pointer def for the geometry map */
    typedef typename GeometryMapType::Pointer GeometryMapPointer;
	typedef typename PointToCenterMapType::Pointer PointToCenterMapPointer;

    /** iterator definition for iterating over a geometry map */
    typedef typename GeometryMapType::Iterator GeometryMapIterator;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Hold on to the type information specified by the template parameters. */
    typedef TMeshTraits                                          MeshTraits;
    typedef typename MeshTraits::PixelType                       PixelType;  
    typedef typename MeshTraits::PointsContainer                 PointsContainer;
    typedef typename Superclass::PointsContainerPointer          PointsContainerPointer;
    typedef typename Superclass::PointsContainer::ConstPointer          PointsContainerConstPointer;
    typedef typename Superclass::PointsContainer::Iterator       PointsContainerIterator;
    typedef typename Superclass::PointsContainerConstIterator    PointsContainerConstIterator;
	typedef typename Superclass::CellsContainer					 CellsContainerType;

	typedef			 std::list<double>							 MeanSimplexDisplacementList;	

	/** \brief Set the map of GeometryData to the new pointer */
	itkGetMacro(PointToCenterMap, PointToCenterMapPointer);

	/** \brief Set the map of GeometryData to the new pointer */
	itkSetMacro(GeometryData, GeometryMapPointer );

	/** \brief Returns the current map of GeometryData */
	itkGetMacro(GeometryData, GeometryMapPointer );

	/** \brief Returns the radial young's elasticity modulus */
	itkGetConstMacro(RadialYoungModulus, double );

	/** \brief Returns the radial young's elasticity modulus */
	itkSetMacro(RadialYoungModulus, double );

	/** \brief Returns the radial stiffness */
	itkGetConstMacro(RadialStiffness, double );

	/** \brief Returns the current map of geometrydata */
	itkGetConstReferenceMacro(GeometryData, GeometryMapPointer );

    /** \brief Get the first free id for new cells*/
    itkSetMacro(LastCellId, unsigned long);

    /** \brief Set the id value valid for new cells */
    itkGetConstMacro(LastCellId, unsigned long);

	/** \brief Get the reference curvature radius of the stent in released position*/
	itkSetMacro(ReferenceCurvatureRadius, double);

	/** \brief Set the reference curvature radius of the stent in released position*/
	itkGetConstMacro(ReferenceCurvatureRadius, double);

	/** \brief Set stent radius in the free release state */
	itkSetMacro(FinalRadius, double);
	/** \brief Get stent radius in the free release state */
	itkGetConstMacro(FinalRadius, double);

	/** \brief Set the area corresponding to each point, computed during the initialization */
	itkSetMacro(AreaPerPoint, double);
	/** \brief Get the area corresponding to each point, computed during the initialization */
	itkGetConstMacro(AreaPerPoint, double);
    /**
    * Copy all necessary information from passed object
    * to the mesh
    */
    virtual void CopyInformation(const DataObject *data);

	MeanSimplexDisplacementList GetMeanSimplexDisplacementList() const {return this->meanSimplexDisplacement;};

	void AddSimplexDisplacement(double disp) {this->meanSimplexDisplacement.push_back(disp);};

	/* \brief This method computes the radial stiffness of the tubular mesh from the Young's 
	   elasticity modulus and the unit scale (mm, m, etc.)
	*/
	virtual void SetRadialStiffnessFromYoungModulusAndUnits(double radialYoungModulus, double units);

	/// \brief Set/Get the point index matrix
	void SetCrossSectionMatrix(CrossSectionMatrixType *CrossSections);
	CrossSectionMatrixType *GetCrossSectionMatrix(){return this->m_CrossSections;}

	/**
	* \brief Get the cross section center position corresponding to the ring where idx lays
	*/
	PointType GetCorrespondingCrossSectionCenter(unsigned long idx);

	/**
	* \brief Get the cross section average radius
	*/
	double GetCorrespondingCrossSectionAverageRadio(unsigned long idx);

    /**
    * \brief Add a new edge to the simplex mesh by specifying the ids of the start 
    * and end point of the edge
    * Note: This can destroy the simplex mesh structure! Better use the 
    * simplex mesh modification or creation filters
    */
    unsigned long AddEdge(unsigned long startPointId, unsigned long endPointId);

    /**
    * \brief Add a new simplex mesh cell to the mesh by passing an AutoPointer of a 
    * previously created simplex mesh cell
    * 
    * Note: This can destroy the simplex mesh structure! You should use the 
    * simplex mesh modification or creation filters.
    */
    unsigned long AddFace(CellAutoPointer &cellPointer);

    /**
    * \brief Replaces the cell specified by replaceIndex with the new cell passed by its
    * AutoPopinter
    */
    unsigned long ReplaceFace(unsigned long replaceIndex, CellAutoPointer &cellPointer);

    /** 
    * \brief Get the three direct neighbors of a point 
    */  
    IndexArray GetNeighbors(unsigned long pointId) const;

    /** 
    * \brief Get all neighbor points with a specified radius  
    */  
    NeighborListType* GetNeighbors(unsigned long pointId, unsigned int radius, NeighborListType* list = NULL) const;

    /** 
    * \brief Add a neighbor to a point. 
    * Note: This can destroy the simplex mesh topology! 
    * Better use the simplex mesh creation filters.
    */  
    void AddNeighbor(unsigned long pointId, unsigned long neighborId);

    /** 
    * \brief Replace a neighbor of a specific point by a new one     
    */  
    void ReplaceNeighbor(unsigned long pointId, unsigned long oldNeighborId,unsigned long newNeighborIdx);

    /** 
    * \brief Swap the order of two neighbors  
    */
    void SwapNeighbors(unsigned long pointId, unsigned long firstNeighborId,unsigned long secondNeighborId);

	/**
	* \brief Set the geometry data for a specified point
	*/
	void SetGeometryData(unsigned long pointId, blSimplexMeshGeometry*);

	/**
	* \brief Get the geometry data for a specified point
	*/
	blSimplexMeshGeometry* GetGeometryData(unsigned long pointId);

    /**
    * \brief Set the geometry data for a specified point
    */
    void SetBarycentricCoordinates(unsigned long idx, PointType values);

    /**
    * \brief Set the barycentric coordinates for a specified point
    */
    PointType GetBarycentricCoordinates(unsigned long idx) const;

    /**
    * \brief Set the reference metrics for a specified point
    */
    void SetReferenceMetrics(unsigned long idx, PointType values);

    /**
    * \brief Return the reference metrics for the specified point
    */
    PointType GetReferenceMetrics(unsigned long idx) const;

    /**
    * \brief Set the simplex angle for the specified point
    */
    void SetPhi(unsigned long idx, double values);

    /**
    * \brief Get the simplex angle for the specified point
    */
    double GetPhi(unsigned long idx) const;

    /**
    * \brief Set the mean curvature for the specified point
    */
    void SetMeanCurvature(unsigned long idx, double values);

    /**
    * \brief Get the mean curvature for the specified point
    */
    double GetMeanCurvature(unsigned long idx) const;

    /**
    * \brief Set the circum circles radius for the specified point
    */
    void SetRadius(unsigned long idx, double values);

    /**
    * \brief Get the circum circles radius for the specified point
    */
    double GetRadius(unsigned long idx) const;

    /**
    * \brief Set the distance to the foot point for the specified point
    */
    void SetDistance(unsigned long idx, double values);

    /**
    * \brief Get the distance to the foot point for the specified point
    */
    double GetDistance(unsigned long idx) const;

	/** \brief Compute the normal vector in the specified mesh point */
	CovariantVectorType ComputeNormal(unsigned long idx ) const;

	/** \brief Update the positions of the centers of each section */
	void UpdateSectionCenters();

	/** \brief Update the positions of the centers of each section */
	void UpdateSectionRadius();

	void DeepCopy(ConstPointer orig);

	// \brief Returns the number of cells in the mesh.
	unsigned long GetNumberOfCells() const;

	/* \brief Computes, from the cross sections matrix, the point to section map*/
	void ComputePointToCenterInfo();

	PointToCenterMapPointer m_PointToCenterMap;

  protected:
	  //  /** \brief Constructor for use by New() method. */
	  blSimplexTubeMesh();
	  virtual ~blSimplexTubeMesh();
	  void PrintSelf(std::ostream& os, Indent indent) const;

	  /** 
	  * \brief The map stores a SimplexMeshGeometry object for each mesh point
	  */
	  GeometryMapPointer m_GeometryData;

	  CrossSectionMatrixType *m_CrossSections;

	  CrossSectionCenterVectorType m_CrossSectionsCenters;

	  CrossSectionAverageToCenterType m_CrossSectionAverageToCenter;

	  /**
	  * The last cell id is the index which is used for insertion of new
	  * cells. It increases during mesh creation. This is done because
	  * one cannot rely on the size of the map or the highest index when 
	  * cells are removed.
	  */
	  unsigned long m_LastCellId;

	  /**
	  * \brief This is the reference curvature radius at the released position 
	  * of the stent in an idealized vessel.
	  */
	  double m_ReferenceCurvatureRadius;

	  /**
	  * \brief This is the radius of the stent in the deployed stare.
	  */
	  double m_FinalRadius;

	  double m_RadialYoungModulus;

	  double m_RadialStiffness;

	  std::list<double> meanSimplexDisplacement;

	  /**
	  * \brief This is the area corresponding to each point in the deployed state.
	  */
	  double m_AreaPerPoint;
  private:
	  blSimplexTubeMesh(const Self&); //purposely not implemented
  }; // End Class:  SimplexMesh

#ifndef ITK_MANUAL_INSTANTIATION
#include "blSimplexTubeMesh.txx"
#endif

#endif // BLSIMPLEXTUBEMESH_H
