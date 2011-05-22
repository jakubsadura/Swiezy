/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef LINEBOUNDPOINTINTERACTOR_H
#define LINEBOUNDPOINTINTERACTOR_H

#include "TpExtLibMITKWin32Header.h"
#include "itkPoint.h"
#include "mitkInteractor.h"


/**
An interactor that allows the user to move a point along a line with the mouse.
When the mouse moves down, the point is moved to one end of the line, and 
mouse up moves it towards to other end.

\ingroup blUtilitiesMITK
\author Maarten Nieber
\date 18 dic 2008
*/

class TPEXTLIBMITK_EXPORT blMitkLineBoundPointInteractor : public mitk::Interactor
{
public:
	typedef mitk::Point3D PointType;

	//! \param offset - Fixes a point on the line.
	//! \param direction - Fixes the direction along the line (i.e. the line orientation)
	mitkClassMacro(blMitkLineBoundPointInteractor, mitk::Interactor);
	mitkNewMacro3Param(Self, mitk::DataTreeNode*, const PointType&, const PointType& );

	//!
	PointType GetDirection() const;
	//!
	void SetDirection(PointType val);
	//!
	PointType GetOffset() const;
	//!
	void SetOffset(PointType val);

	//!
	float GetPrecision() const;
	void SetPrecision(float val);

protected:
	//!
	blMitkLineBoundPointInteractor
	(
		mitk::DataTreeNode*,
		const PointType& offset,
		const PointType& direction
	);

	virtual ~blMitkLineBoundPointInteractor();

	//!
	bool ExecuteAction(mitk::Action* action, mitk::StateEvent const* stateEvent);

	//!
	virtual float CalculateJurisdiction(mitk::StateEvent const* stateEvent) const;

	/** Get picked point in 2D or 3D views 
	Return the closest point ID < m_Precision
	*/
	int GetPickedPoint( 
		mitk::StateEvent const* stateEvent,
		mitk::Point3D &worldPoint,
		mitk::ScalarType timeInMS = 0.0 ) const;

	//!
	void OnNodeDeleted( );

private:
	//!
	void RepositionPoint( mitk::Point3D &newPosition, mitk::ScalarType timeInMS = 0.0 );

private:
	//! Position of the point
	PointType m_Offset;

	//! Direction of the point
	PointType m_Direction;

	/** \brief to store the value of precision to pick a point */
	float m_Precision;
};

#endif //LINEBOUNDPOINTINTERACTOR_H
