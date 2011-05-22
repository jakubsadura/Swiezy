/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blMitkScalarBar_h
#define _blMitkScalarBar_h

#include "TpExtLibMITKWin32Header.h"
#include <mitkBaseData.h>
#include <mitkDataTreeNode.h>

#include "vtkSmartPointer.h"

#include "mitkLookupTable.h"

class vtkRenderWindow;
class vtkScalarBarActor;

namespace mitk{ class DataTreeNode; }

/**
 * \brief Draw a scalar bar in a view
 */
class TPEXTLIBMITK_EXPORT blMitkScalarBar : public itk::DataObject
{
public:

	mitkClassMacro( blMitkScalarBar, itk::DataObject );

	itkNewMacro( Self );

	/**
	* Sets the render window, in which the 3d cube
	* will be shown. Make sure, you have called this function
	* before calling Enable()
	*/
	void SetRenderWindow( vtkRenderWindow* renderWindow );

	//!
	void Enable (bool a);

	//!
	bool IsEnabled( );

	//!
	void SetDataTreeNode( mitk::DataTreeNode* node );

	//!
	mitk::DataTreeNode* GetDataTreeNode( );

	//!
	mitk::LookupTable::Pointer GetLookUpTable( );

	//!
	void SetPropertiesLegend(double xPos, double yPos, double Height, double Width);

protected:

	/**
	* Constructor
	*/
	blMitkScalarBar();

	/**
	* Destructor
	*/
	~blMitkScalarBar();

	//! 
	void OnColorLegendNodeDeleted( );

	//! 
	void OnColorLegendNodeModified( );

protected:
	//! Render window
	vtkRenderWindow*    m_RenderWindow;

	//! Actor to show the legend of the LUT
	vtkSmartPointer<vtkScalarBarActor> m_ScalarBarActor;

	/**
	\brief Color legend node. 
	\note We must not have a smart pointer because we need to catch the event
	DeleteEvent
	*/
	mitk::DataTreeNode* m_ColorLegendNode;

	//!
	unsigned long m_ColorLegendNodeTagDelete;

	//!
	unsigned long m_ColorLegendNodeTagModified;

	//!
	bool m_isEnabled;
};


#endif // _blMitkScalarBar_h
