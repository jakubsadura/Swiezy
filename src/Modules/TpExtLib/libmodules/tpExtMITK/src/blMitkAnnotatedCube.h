/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blMitkAnnotatedCube_h
#define _blMitkAnnotatedCube_h

#include "TpExtLibMITKWin32Header.h"
#include <mitkBaseData.h>

#include "vtkSmartPointer.h"

class vtkRenderWindow;
class vtkAnnotatedCubeActor;
class vtkOrientationMarkerWidget;
class vtkAxesActor;

/**
 * \brief Draw a anatomical orientation marker in the 3D scene 
 */
class TPEXTLIBMITK_EXPORT blMitkAnnotatedCube : public itk::DataObject
{
public:

	mitkClassMacro( blMitkAnnotatedCube, itk::DataObject );

	itkNewMacro( Self );

	/**
	* Sets the render window, in which the 3d cube
	* will be shown. Make sure, you have called this function
	* before calling Enable()
	*/
	void SetRenderWindow( vtkRenderWindow* renderWindow );

	/**
	*/
	void Enable (bool a);

	//!
	void SetAxisColors( float colorX[3], float colorY[3], float colorZ[3] );

protected:

	/**
	* Constructor
	*/
	blMitkAnnotatedCube();

	/**
	* Destructor
	*/
	~blMitkAnnotatedCube();

	//!
	void CreateAnnotatedCube( );

	//!
	void CreateCornerAnnotations( );

protected:
	//! Render window
	vtkRenderWindow*    m_RenderWindow;

	//! Actor
	vtkSmartPointer<vtkAnnotatedCubeActor> m_CubeActor;

	//!
	vtkSmartPointer<vtkAxesActor> m_CubeAxes;

	//! 
	vtkSmartPointer<vtkOrientationMarkerWidget> m_Marker;

};


#endif // _blMitkAnnotatedCube_h
