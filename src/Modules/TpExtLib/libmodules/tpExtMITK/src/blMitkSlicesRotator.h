/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BL_MITK_SLICES_ROTATOR_H
#define BL_MITK_SLICES_ROTATOR_H

#include "TpExtLibMITKWin32Header.h"
#include <mitkSlicesRotator.h>
#include <mitkInteractionConst.h>
#include <mitkBaseRenderer.h>

namespace mitk{ class Geometry2D; }

class vtkRenderWindow;

namespace blMitk {

typedef enum{
	GEOMETRY_CLICKED,
	GEOMETRY_ROTATED,
	GEOMETRY_OTHER,
	GEOMETRY_MAX
} GEOMETRY_TYPE;

/**
 \brief Redefinition of mitk::SlicesRotator, changing the state machine


 \ingroup blUtilitiesMITK
 \date 19 09 08
 \author Xavi Planes
 */
class TPEXTLIBMITK_EXPORT SlicesRotator : public mitk::SlicesRotator
{
public:

  mitkClassMacro(SlicesRotator, mitk::SlicesRotator);
  
  mitkNewMacro1Param(Self, const char*);

protected:

  //! 
  SlicesRotator(const char* machine);

  //! Clear list of controllers
  virtual ~SlicesRotator();

  //!
  virtual bool ExecuteAction(mitk::Action * action, mitk::StateEvent const* stateEvent);

  //!
  mitk::EActions ComputeMoveOrRotate( 
	mitk::BaseRenderer *renderer,
	mitk::Point3D cursor, 
	std::vector<mitk::Geometry2D*> &geometryArray );

  //!
  bool ComputeCursorAndCenterOfRotation( 
	mitk::Point3D cursor, 
	mitk::DisplayGeometry* displayGeometry,
	std::vector<mitk::Geometry2D*> &geometryArray,
	mitk::ScalarType &cursorCenterDistance );

  //! Compute exclusive mode member variable
  void ComputeExclusiveMode( );

  //!
  void GetGeometries( 
	mitk::BaseRenderer* renderer,
	mitk::Point3D cursor,
	std::vector<mitk::Geometry2D*> &geometryArray );

protected:

	/**
	\brief Last accessed vtkRenderWindow
	We need to restore the cursor of the mouse when this interactor is 
	disabled from the global interactor
	*/
	vtkRenderWindow	*m_vtkLastRenderWindow;


	/// all SNCs that will be moved
	SNCVector m_SNCsToBeMoved; 

};

} // namespace 

#endif  // BL_MITK_SLICES_ROTATOR_H


