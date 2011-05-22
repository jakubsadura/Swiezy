/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLMITKPOINTSETINTERACTOR_H
#define BLMITKPOINTSETINTERACTOR_H

#include "TpExtLibMITKWin32Header.h"
#include "mitkCommon.h"
#include "mitkPointSetInteractor.h"
#include "vtkPolyData.h"

class vtkIntArray;

/**
\brief Redefinition mitk::PointSetInteractor with soime new functionalities

\note Registered vtkPolyData until this object is destroyed

\ingroup blUtilitiesMITK
\date 14 07 08
\author Xavi Planes
*/
class TPEXTLIBMITK_EXPORT blMitkPointSetInteractor : public mitk::PointSetInteractor
{
public:
	mitkClassMacro(blMitkPointSetInteractor, mitk::PointSetInteractor);
	mitkNewMacro3Param(Self, const char*, mitk::DataTreeNode*, int);

	/**
	* \brief returns true if the current state is "move point space", ID = 12 
	defined in StateMachine.xml.
	If the user is moving a point -> The pointSet will be changed. However
	we need to know when the user has finished moving the point
	**/
	bool CheckCurrentStateIsMovePointSpace();

	/**
	* \brief returns true if the current action is mitk::AcADDPOINT
	If the user is adding a point or is moving a point -> The pointSet 
	will be changed and we need to differenciate between them
	**/
	bool CheckCurrentActionIsAddPoint();

	/**
	\brief Redefinition of base class function
	Modified for AcADDPOINT action depending on the
	input paramater POINTS_SET_INTERACTOR_TYPE
	 */
	virtual bool ExecuteAction( mitk::Action* action, mitk::StateEvent const* stateEvent );

	//! Restrict point position when adding a point
	void SetRestrictAddPointPosition( bool bVal, mitk::DataTreeNode::Pointer meshPointSet );

	//!
	bool GetIncrementalAdd() const;

	//!
	void SetIncrementalAdd(bool val);

	//! 
	void SetMaxPoints( int iMaxPoints );

	//!
	bool GetLastClickedNormal( mitk::Vector3D &lastClickedNormal ) const;

	//!
	bool GetLastClickedOrigin( mitk::Point3D &lastClickedOrigin ) const;

	//!
	vtkIntArray * GetScalarVector() const;

	//!
	void SetMeasurementPointSet( bool val );

	//! Set the pointSize, when m_MeshPointSet is NULL, otherwise the correct pointSize will be automatically calculated 
	void SetPointSize(double ptSize);
	
	//!
	double GetPointSize();

	//! Set line contour color
	void SetContourColor(double color[3]);

	//! Get line color
	double *GetContourColor();

protected:
	//! Redefined
	blMitkPointSetInteractor(
		const char * type, 
		mitk::DataTreeNode* dataTreeNode, 
		int n = -1 );

	//! Redefined
	virtual ~blMitkPointSetInteractor();

	//!
	bool GetPickedPoint( 
		mitk::StateEvent const* stateEvent,
		mitk::Point3D &worldPoint );

	//!
	void GetInteraction2DSlicePosition(
		mitk::StateEvent const* stateEvent );

	/** Get position of the point using m_CutType,
	use a special method for skeletons and check the bounding box
	*/
	bool GetPointPosition( 
		mitk::StateEvent const* stateEvent,
		mitk::Point3D &selectedPoint );

protected:
	//! Set of points that can be selected
	mitk::DataTreeNode::Pointer	m_MeshPointSet;

	//! Restrict point position when adding a point to the mesh
	bool m_bRestrictAddPointPosition;

	/** Add the new point to the current position of the pointset 
	(m_CurrentAddPosition) or push the point at the end
	*/
	bool m_bIncrementalAdd;

	//! Current pointset position for adding a point using m_bIncrementalAdd = false
	long m_CurrentAddPosition;

	//! True if this class is adding a point 
	bool m_bAddingPoint;

	/**
	* \brief the number of possibl points in this object
	* \note Redefinition of base class
	* if -1, then no limit set
	*/
	int m_iMaxPoints;

	//! Normal of the last clicked 2D Slice plane
	mitk::Vector3D m_LastClickedNormal;

	//! Origin of the last clicked 2D Slice plane
	mitk::Point3D m_LastClickedOrigin;
	
	//!
	bool m_LastClickedPlaneIsOk;

	//! Scalar value for the last clicked point
	vtkIntArray *m_ScalarVector;

	//! Store one of the cell the point belongs to
	vtkIdType m_cellId;

	//! Bounding box to select the interactor
	double m_BoundingBox[ 6 ];

	//! the size of each point
	double m_ptSize;

	double  m_color[3];

};

#endif // BLMITKPOINTSETINTERACTOR_H
