/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef ROIINTERACTORHELPER_H
#define ROIINTERACTORHELPER_H


#include "coreObject.h"
#include <mitkCommon.h>
#include <mitkPointSet.h>
#include "coreSmartPointerMacros.h"
#include "coreDataEntityHolder.h"
#include "coreCommonDataTypes.h"
#include "coreContourInteractor.h"

namespace Core
{

namespace Widgets{ 
	class RenderWindowBase; 
	class AcquireDataEntityInputControl;
}

/**
\brief This class is used by the PluginTab and the ViewToolBar to set the 
currentDataEntityHolder
       and the rendering tree that are needed by the contour Interactor

The mask image has only 1 time step.
Each time the current time step, changes, a new mask image is created

\ingroup gmInteractors
\author Chiara Riccobene
\date 9 jun 2009
*/
class ROIInteractorHelper 
	: public Core::SmartPointerObject
{

public:
	enum INTERACTOR_STATE
	{
		INTERACTOR_DISABLED,
		INTERACTOR_ENABLED
	};

	typedef Core::DataHolder<INTERACTOR_STATE> InteractorStateHolderType;

public:
	//! Create itk typedefs and new function for ROIInteractorHelper
	coreDeclareSmartPointerClassMacro(
		ROIInteractorHelper, 
		Core::SmartPointerObject);

	//!
	void SetRenderingTree(Core::RenderingTree::Pointer val);

	//!
	void SetSelectedDataHolder(Core::DataEntityHolder::Pointer dataEntity);

	//!
	Core::DataEntityHolder::Pointer GetSelectedDataHolder( );

	//! Set m_InputDataHolder and m_maskImageDataHolder
	void OnModifiedSelected( );
		
	//!
	void StartInteraction( );

	//!
	void StopInteraction( bool show = true );
	
	//! Set the color of the mask image. This method should only be called after the mask image is created and attached to the rendering tree
	void SetColor(float r, float g, float b);

	//! Set the opacity of the mask image. This method should only be called after the mask image is created and attached to the rendering tree
	void SetOpacity(float opacity);


	//!
	InteractorStateHolderType::Pointer GetInteractorStateHolder() const;

	//!
	bool IsNewMaskImage();

	//!
	void SetNewMaskImage(bool isNew);

	//!
	int GetNextRoiLevel();

	//!
	void SetNextRoiLevel(int n);

	//!
	void SetEraseOnlySelectedColor(bool b);

	//!
	bool GetEraseOnlySelectedColor();

	//! 
	Core::DataEntityHolder::Pointer GetMaskImageDataHolder( );

private:
	//!
	ROIInteractorHelper(void);

	//!
	~ROIInteractorHelper(void);

	/** stop and start interaction
	\note The m_InputDataHolder MUST be an image
	*/
	void OnModifiedInput( );

	//!
	void OnTimeStep( );

	//!
	void OnMaskImageModified( );

	//!
	void CreateMaskImage( Core::DataEntity::Pointer inputDataEntity );

	//!
	std::string GetNumMaskImagesString(int n);

private:

	//! Input data
	Core::DataEntityHolder::Pointer m_SelectedDataHolder;

	//! Input data
	Core::DataEntityHolder::Pointer m_InputDataHolder;

	//! Roi data
	Core::DataEntityHolder::Pointer m_maskImageDataHolder;

	//!
	Core::ContourInteractor::Pointer m_ContourInteractor;

	//!
	Core::RenderingTree::Pointer m_RenderingTree;

	//! State of the interactor to update the views
	InteractorStateHolderType::Pointer m_InteractorStateHolder;

	//!
	bool bNewMaskImage;

	//!
	int m_nextRoiLevel;

	//!
	bool m_bEraseOnlySelectedColor;

	coreDeclareNoCopyConstructors(ROIInteractorHelper);
};

}// Core
#endif //ROIInteractorHelper_h
