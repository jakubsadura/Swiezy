/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLMITKPOINTSELECTINTERACTOR_H
#define BLMITKPOINTSELECTINTERACTOR_H

#include "TpExtLibMITKWin32Header.h"
#include "mitkCommon.h"
#include "mitkInteractor.h"
#include "mitkPointSet.h"
#include "mitkDisplayPositionEvent.h"
#include "vtkType.h"
#include "vtkRenderer.h"

class vtkPolyData;

typedef enum
{
	CUT_TYPE_NONE,
	SKELETON_TYPE,
	RING_TYPE
} CUT_TYPE;


/**
	 * \brief Interactor to select ONE point of a vtkPolyData
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
class TPEXTLIBMITK_EXPORT blMitkPointSelectInteractor : public mitk::Interactor
{
public:
	//!
	mitkClassMacro(blMitkPointSelectInteractor, mitk::Interactor);
	//! Implementation of the ITK New function (not using the macro this time!). See the constructor.
	static Pointer New(
		const char * type, 
		mitk::DataTreeNode* pointSetNode,
		mitk::DataTreeNode* dataTreeSet,
		double relativeSelectedPointSize = 0.005,
		double relativeMarginAroundBoundingBox = 0.1
	);

	//! See m_Direction.
	int GetDirection( );
	//! Set the bounding box to select the interactor to bounds rescaled by scaleFactor.
	void SetBoundingBox( double const bounds[ 6 ] );

	//!
	void SetCuttingType(CUT_TYPE cutting_type);

	//!
	vtkIdType GetCellId();

	/**
	* \brief calculates how good the data, this statemachine handles, is hit
	* by the event.
	*
	* overwritten, cause we don't look at the boundingbox, we look at each point
	*/
	virtual float CalculateJurisdiction(mitk::StateEvent const* stateEvent) const;

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
	blMitkPointSelectInteractor(
						const char * type = "PointSelectInteractor", 
						mitk::DataTreeNode* dataTreeNode = NULL,
						mitk::DataTreeNode* meshPointSet = NULL,
						double relativeSelectedPointSize = 0.005,
						double relativeMarginAroundBoundingBox = 0.1
	);

	//! Destructor 
	virtual ~blMitkPointSelectInteractor();

	//! Executes Actions
	virtual bool ExecuteAction(mitk::Action* action, mitk::StateEvent const* stateEvent);

	//! Get world point using PointPicking mode
	bool GetPickedPoint( 
					mitk::StateEvent const* stateEvent,
					mitk::Point3D &worldPoint );

	/** Get position of the point using m_CutType,
	use a special method for skeletons and check the bounding box
	*/
	bool GetPointPosition( 
					mitk::StateEvent const* stateEvent,
					mitk::Point3D &selectedPoint );

private:
	//! Set of points that can be selected
	mitk::DataTreeNode::Pointer m_MeshTreeNode;

	//! Direction attached to each selected point.
	//! MN: please explain what this is for.
	int m_Direction;

	//! Bounding box to select the interactor
	double m_BoundingBox[ 6 ];

	//! Store one of the cell the point belongs to
	vtkIdType m_cellId;

	CUT_TYPE m_CutType;
};

#endif // BLMITKPOINTSELECTINTERACTOR_H
