/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLMITKSURFACESELECTINTERACTOR_H
#define BLMITKSURFACESELECTINTERACTOR_H

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
	 * \brief Interactor to select region of a vtkPolyData
	 * \ingroup blUtilitiesMITK
	 *
	 *
	 * \note Registered vtkPolyData until this object is destroyed
	 *
	 * The interactor is attached to a rendering node mitk::DataTreeNode
	 * that will be used to store the selected points.
	 *
	 * This class uses a vtkPolyData as a parameter in the constructor
	 * to find the closest point to the mouse position when the user
	 * adds a point to the mitk::DataTreeNode. These are the ONLY 
	 * selectable points of this interactor.
	 *
	 * There's also a int attached to each point of the mitk::DataTreeNode
	 * that can be used for other things.
	 *
	 * By default, the interactor state will change to selected if the
	 * picked point is inside the bounding box of the input vtkPolyData.
	 * You can also call the function SetBoundingBox( )
	 * 
	 * The MITK state machine is modified. You can take a look at 
	 * thirdParty/MITK-0.7/mitk/Core/Interactions/StateMachine.xml
	 * The new state machine is added for this class:
	 * <stateMachine NAME="PointSelectInteractor">
	 * A copy of this state machine is in 
	 * BaseLib/libmodules/blUtilitiesMITK/include/StateMachine.xml
	 *
	 * The state machine has two interactions:
	 * - EVENT_ID="3" => EIDLEFTMOUSEBTNANDSHIFT
	 * - EVENT_ID="2" => EIDRIGHTMOUSEBTN
	 *
	 * Actions:
	 * - AcADDPOINT: adds a point
	 * - AcCHECKELEMENT: checks that the mouse position
	 *   is close to ... and will send a mitk::StateEvent EIDYES or EIDNO
	 * - AcCHECKOBJECT: checks that the mouse position
	 *   is close to a point and will send a mitk::StateEvent EIDYES or EIDNO
	 * - AcSELECTSUBOBJECT: Increment the int attached to the selected point
	 *   This action will call the OpSELECTPOINT to the mitk::DataTreeNode,
	 *   so the observers can receive the event.
	 *
	 */
class TPEXTLIBMITK_EXPORT blMitkSurfaceSelectInteractor : public mitk::Interactor
{
public:
	//!
	mitkClassMacro(blMitkSurfaceSelectInteractor, mitk::Interactor);
	//! Implementation of the ITK New function (not using the macro this time!). See the constructor.
	static Pointer New(
		const char * type, 
		mitk::DataTreeNode* dataTreeNode,
		vtkPolyData	*meshPointSet,
		mitk::DataTreeNode* sphereTreeNode,
		mitk::PointSet* points,
		double relativeSelectedPointSize = 0.005,
		double relativeMarginAroundBoundingBox = 0.1
	);
	//! See m_Direction.
	int GetDirection( );
	//! Set the bounding box to select the interactor
	void SetBoundingBox( double bounds[ 6 ] );
	//! See m_Radius
	void SetRadius(double radius);
	//! See m_Radius
	double GetRadius( );
    //! See m_Sphere
	vtkSphere * GetImplicitSphere( );

	void ResetSphere();

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
	blMitkSurfaceSelectInteractor(
						const char * type = "SurfaceSelectInteractor", 
						mitk::DataTreeNode* dataTreeNode = NULL,
						vtkPolyData	*meshPointSet = NULL,
						mitk::DataTreeNode* sphereTreeNode = NULL,
						mitk::PointSet* points = NULL,
						double relativeSelectedPointSize = 0.005,
						double relativeMarginAroundBoundingBox = 0.1
	);

	//! Destructor 
	virtual ~blMitkSurfaceSelectInteractor();

	//! Executes Actions
	virtual bool ExecuteAction(mitk::Action* action, mitk::StateEvent const* stateEvent);

	bool GetPickedPoint( 
					mitk::StateEvent const* stateEvent,
					mitk::Point3D &worldPoint );

	
private:
	//! Set of points that can be selected
	vtkPolyData	*m_MeshPointSet;

	vtkSmartPointer<vtkSphere> m_ImplicitSphere;
	vtkSmartPointer<vtkSphereSource> m_SphereSource;
	mitk::DataTreeNode::Pointer m_SphereNode;

	//! Direction attached to each selected point.
	//! MN: please explain what this is for.
	int m_Direction;

	//! Bounding box to select the interactor
	double m_BoundingBox[ 6 ];

	double m_Radius;
	bool m_selected;

	mitk::PointSet* pointSet;
};

#endif // BLMITKSURFACESELECTINTERACTOR_H
