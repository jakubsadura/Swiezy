/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef BLSIMPLEXMESHGEOMETRY_H
#define BLSIMPLEXMESHGEOMETRY_H

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

// std
#include <set>

// ITK
#include "itkFixedArray.h"
#include "itkPoint.h"
#include "itkVector.h"
#include "itkCovariantVector.h"

// BaseLib
#include "BaseLibVTKWin32Header.h"
#include "blLightObject.h"
#include "blMacro.h"
#include "blSmartPointer.h"

using namespace itk;

/**
  * \class SimplexMeshGeometry
  * \brief Handle geometric properties for vertices of a simplex mesh
  * 
  * It stores and recomputes geometric properties of simplex mesh
  * vertices, i.e. the normal vector, the barycentric coordinates of
  * the point related to its three neighbor vertices, simplex angle
  * circumsphere and circumcircle radius and center.
  *
  *
  * \author Nacho Larrabide. Extrended from itk::SimplexGeometry.
  *
  */
class BASELIBVTK_EXPORT blSimplexMeshGeometry//: public blLightObject
{
public:

	////-----------------------------------------------------
	//// Typedefs
	////-----------------------------------------------------
	//typedef blSimplexMeshGeometry Self;
	//typedef blSmartPointer< Self > Pointer;

	///// \brief Static constructor
	//blNewMacro( Self );

  typedef itk::Point<double,3>                PointType;
  typedef itk::Vector<double,3>               VectorType;
  typedef itk::CovariantVector<double,3>      CovariantVectorType;
  typedef itk::FixedArray<unsigned long,3>    IndexArray;
  typedef itk::FixedArray<PointType,3>        PointArray;
  typedef std::set<unsigned long>             NeighborSetType;
  typedef std::vector<PointType>			  TrajectoryType;
  typedef std::vector<unsigned long>		  NeghborCellsArray;

  blSimplexMeshGeometry();
  ~blSimplexMeshGeometry();


  /** Definition of some attributes for 
    * faster deformable model computation
    */

  /* Stores the indices of the three direct neighbors */
  IndexArray neighborIndices;

  /* Stores the coordinates of the three direct neighbors */
  PointArray neighbors;

  /* Stores the mean curvature of the mesh in the point */
  double meanCurvature;

  /* Coordinates of the corresponding point */
  PointType pos;

  /* Coordinates of the corresponding point in previous iteration */
  PointType oldPos;

  /* Barycentric coordinates of corresponding point with respect 
   * to its three direct neighbors
   */
  PointType eps;

  /*
   * Reference metric params
   */
  PointType referenceMetrics;

  /* Normal vector of corresponding point */
  CovariantVectorType normal;

  /* Stores external force component for 
   * current deformable model iteration
   */
  VectorType externalForce;

  /* Stores internal force component for 
   * current deformable model iteration
   */
  VectorType smoothingForce;

  /* Stores the component resultant from applying the Newtonian law of motion for 
  * current deformable model iteration
  */
  VectorType newtonianLawOfMotion;

  /*
   * Store the location of the closest attractor to this point
   */
  PointType closestAttractor;

  /*
   * Stores the index of the closest attractor to this point
   */
  unsigned long closestAttractorIndex;

  /* Stores circum circle radius */
  double circleRadius;

  /* Stores circum circle center */
  PointType circleCenter;

  /* Stores circum sphere radius */
  double sphereRadius;

  /* Stores distance to foot point */
  double distance;

  /* Stores angle */
  double phi;

  /* Stores the neighbor set */
  NeighborSetType neighborSet;

  /* Stores multiplier for interactive deformable model filter */
  double multiplier;

  /* Stores the local final radius of the cilinder*/
  double localFinalRadius;

  /* Stores the local area associated to this radius*/
  double associatedArea;

  /* Stores the history of the positions of the points*/
  TrajectoryType trajectory;

  NeghborCellsArray neighborCells;

  unsigned long forceIndex;

  bool isFixed;

  /**
    * Computes the center and radius of the circum circle of the
    * three neighbor points and of the circum sphere 
    */
  void ComputeGeometry();

};// end of class SimplexMeshGeometry

#endif // BLSIMPLEXMESHGEOMETRY_H

