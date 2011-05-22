/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLMITKTRIANGLESELECTINTERACTOR_H
#define BLMITKTRIANGLESELECTINTERACTOR_H

#include "TpExtLibMITKWin32Header.h"
#include "mitkCommon.h"
#include "mitkInteractor.h"
#include "mitkPointSet.h"
#include "mitkDisplayPositionEvent.h"
#include "vtkSphere.h"
#include "vtkSphereSource.h"
#include "vtkSmartPointer.h"

class vtkPolyData;

/**
	 * \brief Interactor to select a triangle of a vtkPolyData
	 * \ingroup blUtilitiesMITK
	 * \author Chiara Riccobene
	 * \date 16 Apr 2010
	 *
	 * interactor description missing
	 */
class TPEXTLIBMITK_EXPORT blMitkTriangleSelectInteractor : public mitk::Interactor
{
public:
	//!
	mitkClassMacro(blMitkTriangleSelectInteractor, mitk::Interactor);
	//! Implementation of the ITK New function (not using the macro this time!). See the constructor.
	static Pointer New(
		const char * type, 
		mitk::DataTreeNode* dataTreeNode,
		vtkPolyData	*meshPointSet,
		mitk::PointSet* points,
		double relativeSelectedPointSize = 0.005,
		double relativeMarginAroundBoundingBox = 0.1
	);
	//! Set the bounding box to select the interactor
	void SetBoundingBox( double bounds[ 6 ] );
	//! See m_Radius
	void SetRadius(double radius);
	//! See m_Radius
	double GetRadius( );

	vtkSphere* GetImplicitSphere();

protected:
	/**
	\param type - The type of MITK interactor.
	\param dataTreeNode - Location where to attach this interactor.
	\param meshPointSet - Set of points from which the user can select a point.
	\param relativeSelectedPointSize - Display size of the selected point relative to the extent of the
	pointset (as calculated with vtkOBBTree).
	\param marginAroundBoundingBoxAsPercentage - a bounding box is used to check if the area where the user
	is clicking contains points of the mesh. The bounding box calculated by vtk is sometimes too small. Therefore,
	with this parameter, you can enlarge it. A value of 0.1 means 10% enlargement.

	*/
	blMitkTriangleSelectInteractor(
						const char * type = "SurfaceSelectTriangleInteractor", 
						mitk::DataTreeNode* dataTreeNode = NULL,
						vtkPolyData	*meshPointSet = NULL,
						mitk::PointSet* points = NULL,
						double relativeSelectedPointSize = 0.005,
						double relativeMarginAroundBoundingBox = 0.1
	);

	//! Destructor 
	virtual ~blMitkTriangleSelectInteractor();

	//! Executes Actions
	virtual bool ExecuteAction(mitk::Action* action, mitk::StateEvent const* stateEvent);

	bool GetPickedPoint( 
					mitk::StateEvent const* stateEvent,
					mitk::Point3D &worldPoint );

	
private:
	//! Set of points that can be selected
	vtkPolyData	*m_MeshPointSet;

 	vtkSmartPointer<vtkSphere> m_ImplicitSphere;
// 	vtkSmartPointer<vtkSphereSource> m_SphereSource;
// 	mitk::DataTreeNode::Pointer m_SphereNode;

	//! Direction attached to each selected point.
	//! MN: please explain what this is for.
	int m_Direction;

	//! Bounding box to select the interactor
	double m_BoundingBox[ 6 ];

	double m_Radius;
	double m_pointSize;
	bool m_selected;

	mitk::PointSet* pointSet;
};

#endif // BLMITKTriangleSELECTINTERACTOR_H
