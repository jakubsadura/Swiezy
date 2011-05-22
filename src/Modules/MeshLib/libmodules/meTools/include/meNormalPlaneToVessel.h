/*
* Copyright (c) 2008,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meNormalPlaneToVessel_h
#define _meNormalPlaneToVessel_h

//vtk
#include <vtkPolyData.h>

//BaseLib
#include <blLightObject.h>
#include <blSmartPointer.h>

//#include "meshLibWin32Header.h"


/**
\brief  This class improves the normal plane to a vessel by modifying
the vector obtained directly from a not-very-good centerline.
The metric to minimize is the area bounded by the vessel at 
at the plane.

\ingroup meshLib
\author Jose M Pozo
\date 2008/05/13
*/
class meNormalPlaneToVessel : public blLightObject
{
public:		
       
	// ----------------------------------------------
	// Typedefs
	//-----------------------------------------------
    
	typedef meNormalPlaneToVessel Self;
	typedef blSmartPointer<Self> Pointer;	

	// ----------------------------------------------
	// Methods
	//-----------------------------------------------

	blNewMacro(Self); //Crea-> :: New()

	void SetSurfaceMesh(vtkPolyData * mesh);

	void SetClosedSurfaceMesh(vtkPolyData * closedMesh);

	void SetOrigin(double point[3]);

	void SetOriginMeshId(int pointId);

	void SetSecondOrigin(double point[3]);

	void SetSecondOriginMeshId(int pointId);


	void InitializeNormal(double normal[3]);

	void InitializeNormal();

	void Update();

	void GetNormal(double normal[3]);

	void SetNormal(double normal[3]);

	double * GetNormal();

	void GetE1(double e1[3]);

	double * GetE1();

	void GetE2(double e2[3]);

	double * GetE2();

	void GetE1E2(double e1[3], double e2[3]);

	void SetE1E2(double e1[3], double e2[3]);

	void GetMaxDiameterDirection(double e[3]);

	double * GetMaxDiameterDirection();

	void GetMinDiameterDirection(double e[3]);

	double * GetMinDiameterDirection();

	void GetMaxAndMinDiameterDirections(double e1[3], double e2[3]);

	double GetMaxDiameter();

	double GetMinDiameter();

	void GetMaxAndMinDiameters(double & max, double & min);
	
	double * GetMaxDiameterPoints1();
	
	double * GetMaxDiameterPoints2();
	
	double * GetMinDiameterPoints1();
	
	double * GetMinDiameterPoints2();
	
	void GetMaxDiameterPoints(double point1[3], double point2[3]);
	
	void GetMinDiameterPoints(double point1[3], double point2[3]);

	void GetCenter(double center[3]);

	void SetCenter(double center[3]);

	double * GetCenter();

	void GetCircumscribingCenter(double center[3]);

	double * GetCircumscribingCenter();

	double GetCircumscribingRadius();

	void GetInscribedCenter(double center[3]);

	double * GetInscribedCenter();

	double GetInscribedRadius();

	void ComputeInscribedCenterAndRadius();

	void ComputeCircumscribingCenterAndRadius();

	vtkPolyData * GetContourCrossSection();

	void GetContourCrossSection(vtkPolyData * contour);

	void ComputeMeshTangentPlaneAtPoint();

	void ComputeMaxAndMinDiameters();

	double GetArea();
	
	void SetConnectedCutPlane(vtkPolyData *connec);
	void GetConnectedCutPlane(double normal[3]);

protected:

	/// \brief Constructor  
	meNormalPlaneToVessel(void);

	/// \brief Destructor  
	virtual ~meNormalPlaneToVessel();

	// ----------------------------------------------
	// Member functions 
	// ----------------------------------------------

	void InitializeNormalFromOnePoint();
	void InitializeNormalFromTwoPoints();
	void ImproveNormal();
	void ImproveNormalFromOnePoint();
	void ImproveNormalFromTwoPoints();
	double GetNeighbourArea(double e[3], double angle);
	double GetArea(double normal[3]);
	double ComputeArea();	
	void ComputeE1E2();
	void InitializeCenter();
	void ImproveCenter();
//	void CloseMesh();
	
	// ----------------------------------------------
	// Member variables
	// ----------------------------------------------

	double m_Normal[3],
	       m_Point[3],
	       m_Point2[3],
	       e1[3],
	       e2[3],
	       m_MaxDDirection[3],
	       m_MinDDirection[3],
	       maxD,
	       maxDPoint1[3],
	       maxDPoint2[3],
	       minD,
	       minDPoint1[3],
	       minDPoint2[3],
	       m_Radius,
	       circumscribingCenter[3],
	       circumscribingRadius,
	       inscribedCenter[3],
	       inscribedRadius,
	       tangentPlane[3],
	       segment[3];

	vtkPolyData * m_Mesh,
	            * m_ClosedMesh,
	            * m_Intersection;

	bool centerComputed,
	     secondPointSet;

	int pointId,
	    point2Id;

private:        	

	meNormalPlaneToVessel(const Self&); //purposely not implemented
	void operator=(const Self&);    //purposely not implemented	

};

#endif // _meNormalPlaneToVessel_h




