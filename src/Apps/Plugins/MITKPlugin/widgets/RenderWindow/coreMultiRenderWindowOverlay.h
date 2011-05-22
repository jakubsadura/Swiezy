/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreMultiRenderWindowOverlay_H
#define coreMultiRenderWindowOverlay_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include "coreDataEntityHolder.h"

#include "itkCommand.h"
#include "itkImage.h"

#include "mitkVector.h"

namespace mitk
{
	class Stepper;
	class Image;
}

namespace Core
{
namespace Widgets
{
class MultiRenderWindowMITK;

/** 
\brief Show overlay info to the 2D views

\ingroup gmWidgets
\author Xavi Planes
\date 29 July 2009
*/
class PLUGIN_EXPORT MultiRenderWindowOverlay : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(MultiRenderWindowOverlay, Core::SmartPointerObject);

	//!
	void Init( 
		Core::Widgets::MultiRenderWindowMITK* multiRenderWindow,
		Core::DataEntityHolder::Pointer selectedDataEntity );

	//!
	void RefreshOverlayText( );

	//!
	mitk::Stepper* GetStepperX( );

	//!
	mitk::Stepper* GetStepperY( );

	//!
	mitk::Stepper* GetStepperZ( );

protected:

	MultiRenderWindowOverlay( );

	virtual ~MultiRenderWindowOverlay( );

	//!
	void OnSelectedModified( );

	//!
	void OnPosStepperChanged();

	//!
	void OnTimestepChanged();
	
	//!
	void OnChangedMetadata (blTagMap* tagMap,unsigned long tagId);

	//!
	void InitOberversToSteppers( );

	//!
	void RemoveOberversToSteppers( );

	//!
	mitk::Stepper* GetTime();

	//!
	void GetPixelValue( 
		mitk::Image* image, 
		mitk::Point3D axisCenter,
		unsigned int timeStep,
		std::string &pixelValue );

protected:

	//! The window to center
	Core::Widgets::MultiRenderWindowMITK* m_MultiRenderWindow;

	//!
	Core::DataEntityHolder::Pointer m_SelectedDataEntity;

	//!
	itk::SimpleMemberCommand<MultiRenderWindowOverlay>::Pointer m_PosStepperChangedCommand;

	//!
	itk::SimpleMemberCommand<MultiRenderWindowOverlay>::Pointer m_TimeStepperChangedCommand;

	//!
	unsigned long m_PosStepperChangedCommandTagX;
	//!
	unsigned long m_PosStepperChangedCommandTagY;
	//!
	unsigned long m_PosStepperChangedCommandTagZ;
	//!
	unsigned long m_TimeStepperChangedCommandTagT;

	//!
	typedef itk::Image< double, 3 > ImageType;

};

}
}

#endif // coreMultiRenderWindowOverlay_H

