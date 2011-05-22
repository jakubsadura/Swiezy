/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreROIInteractorHelper.h"
#include "coreDataEntityHelper.h"
#include "coreReportExceptionMacros.h"
#include "coreDataEntityHolder.h"

#include <mitkBaseRenderer.h>
#include <mitkDataTree.h>
#include <mitkRenderingManager.h>

#include "wxMitkRenderWindow.h"

#include "vtkImageMathematics.h"

using namespace Core;

ROIInteractorHelper::ROIInteractorHelper()
{
	m_InputDataHolder = Core::DataEntityHolder::New();
	m_InputDataHolder->AddObserver( 
		this, 
		&ROIInteractorHelper::OnModifiedInput );

	m_maskImageDataHolder = Core::DataEntityHolder::New();
	m_maskImageDataHolder->AddObserver<ROIInteractorHelper>(
		this, 
		&Self::OnMaskImageModified );

	m_InteractorStateHolder = InteractorStateHolderType::New();
	m_InteractorStateHolder->SetSubject( INTERACTOR_DISABLED );

	SetNewMaskImage(false);
	m_nextRoiLevel = 1;
}


Core::ROIInteractorHelper::~ROIInteractorHelper( void )
{

	m_InputDataHolder->RemoveObserver( 
		this, 
		&ROIInteractorHelper::OnModifiedInput );

	if ( m_SelectedDataHolder.IsNotNull() )
	{
		m_SelectedDataHolder->RemoveObserver( 
			this, 
			&ROIInteractorHelper::OnModifiedSelected );
	}

	if ( m_maskImageDataHolder.IsNotNull() )
	{
		m_maskImageDataHolder->RemoveObserver<ROIInteractorHelper>(
			this, 
			&Self::OnMaskImageModified );
	}

}

void ROIInteractorHelper::SetRenderingTree( 
	Core::RenderingTree::Pointer val )
{
	m_RenderingTree = val;
}

void ROIInteractorHelper::SetSelectedDataHolder(
	Core::DataEntityHolder::Pointer dataEntity)
{
	if ( m_SelectedDataHolder.IsNotNull() )
	{
		m_SelectedDataHolder->RemoveObserver( 
			this, 
			&ROIInteractorHelper::OnModifiedSelected );
	}
	m_SelectedDataHolder = dataEntity;
	m_SelectedDataHolder->AddObserver( 
		this, 
		&ROIInteractorHelper::OnModifiedSelected );
}

Core::DataEntityHolder::Pointer ROIInteractorHelper::GetSelectedDataHolder( )
{
	m_RenderingTree->Show( m_maskImageDataHolder->GetSubject(), false );
	return m_SelectedDataHolder;
}

void ROIInteractorHelper::StartInteraction()
{
	if ( m_InputDataHolder.IsNull() )
	{
		return;
	}

	Core::DataEntity::Pointer inputDataEntity;
	inputDataEntity = m_InputDataHolder->GetSubject();
	if ( inputDataEntity.IsNull( ) )
	{
		return;
	}

	// Create a mask image with N time steps
	CreateMaskImage( inputDataEntity );

	// Create interactor
	m_ContourInteractor = Core::ContourInteractor::New( 
		m_RenderingTree,
		m_maskImageDataHolder,
		m_InputDataHolder );
	
	try
	{
		m_ContourInteractor->SetPixelValue(GetNextRoiLevel());
		m_ContourInteractor->SetEraseOnlySelectedColor(GetEraseOnlySelectedColor());
		m_ContourInteractor->ConnectToDataTreeNode();

		m_InteractorStateHolder->SetSubject( INTERACTOR_ENABLED );

	}coreCatchExceptionsReportAndNoThrowMacro( ROIInteractorHelper::StartInteraction );

}

void ROIInteractorHelper::StopInteraction( bool show )
{
	if ( m_ContourInteractor.IsNull() )
	{
		return;
	}
	
	m_ContourInteractor->DisconnectFromDataTreeNode();
	m_ContourInteractor = NULL;

	m_InteractorStateHolder->SetSubject( INTERACTOR_DISABLED );

}

void ROIInteractorHelper::OnModifiedInput()
{
	try{

		if ( m_InteractorStateHolder->GetSubject() == INTERACTOR_ENABLED )
		{
			StopInteraction();
		}

	}
	coreCatchExceptionsLogAndNoThrowMacro( ROIInteractorHelper::OnModifiedInput );
}

void Core::ROIInteractorHelper::OnModifiedSelected()
{
	try{

		Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
		Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();

		if ( m_SelectedDataHolder.IsNull() )
		{
			m_InputDataHolder->SetSubject( NULL );
			m_maskImageDataHolder->SetSubject( NULL );
			return;
		}

		Core::DataEntity::Pointer selectedDataEntity;
		selectedDataEntity = m_SelectedDataHolder->GetSubject();
		if ( selectedDataEntity.IsNull( ) )
		{
			m_InputDataHolder->SetSubject( NULL );
			m_maskImageDataHolder->SetSubject( NULL );
			return;
		}

		// Data entity is a ROI -> Check for the father
		if ( selectedDataEntity->GetType() & Core::ROITypeId )
		{
			if ( selectedDataEntity->GetFather( ).IsNotNull( ) && 
				selectedDataEntity->GetFather( )->IsImage() )
			{
				m_maskImageDataHolder->SetSubject( selectedDataEntity );
				m_InputDataHolder->SetSubject( selectedDataEntity->GetFather( ) );
			}
		}
		else if ( selectedDataEntity->IsImage() )
		{
			m_InputDataHolder->SetSubject( selectedDataEntity );
		}
		else
		{
			m_maskImageDataHolder->SetSubject( NULL );
			m_InputDataHolder->SetSubject( NULL );
		}

		// Connect subjects. When any of the holders is set to NULL, this
		// will also be set to NULL and the reference will be removed
		if ( m_maskImageDataHolder->GetSubject().IsNotNull( ) )
		{
			list->ConnectInputHolder( m_maskImageDataHolder );
		}
		if ( m_InputDataHolder->GetSubject().IsNotNull( ) )
		{
			list->ConnectInputHolder( m_InputDataHolder );
		}

	}
	coreCatchExceptionsLogAndNoThrowMacro( ROIInteractorHelper::OnModifiedSelected );

}

ROIInteractorHelper::InteractorStateHolderType::Pointer 
Core::ROIInteractorHelper::GetInteractorStateHolder() const
{	
	return m_InteractorStateHolder;
}

void Core::ROIInteractorHelper::OnMaskImageModified()
{
	Core::DataEntity::Pointer maskImage;
	maskImage = m_maskImageDataHolder->GetSubject();
	if ( maskImage.IsNull( ) )
		std::cout << "NULL" <<std::endl;
}

void Core::ROIInteractorHelper::SetColor(float r, float g, float b) {
	// Change the color of the ROI
	if ( m_RenderingTree->IsDataEntityRendered( m_maskImageDataHolder->GetSubject() ) ) {
		mitk::DataTreeNode::Pointer node;
		boost::any anyData = m_RenderingTree->GetNode( m_maskImageDataHolder->GetSubject() );
		Core::CastAnyProcessingData( anyData, node );
		node->SetColor(r,g,b);
	}
}

void Core::ROIInteractorHelper::SetOpacity(float opacity)
{
		// Change the color of the ROI
	if ( m_RenderingTree->IsDataEntityRendered(m_maskImageDataHolder->GetSubject()) )
	{
		m_RenderingTree->SetProperty(m_maskImageDataHolder->GetSubject(), blTag::New( "opacity", opacity ) );
	}
}

bool Core::ROIInteractorHelper::IsNewMaskImage() {
	return bNewMaskImage;
} 

void Core::ROIInteractorHelper::SetNewMaskImage(bool isNew) {
	bNewMaskImage = isNew;
} 

int Core::ROIInteractorHelper::GetNextRoiLevel() {
	return m_nextRoiLevel;
}

void Core::ROIInteractorHelper::SetNextRoiLevel(int n) {
	m_nextRoiLevel = n;
	if ( m_ContourInteractor.IsNotNull() )
	{
		m_ContourInteractor->SetPixelValue(GetNextRoiLevel());
	}
}


std::string Core::ROIInteractorHelper::GetNumMaskImagesString(int n) {
	if (n<0) return ("-" + GetNumMaskImagesString(-1*n));
	if (n==0) return "0";

	std::string ret = "";
	while (n>0) {
		ret = std::string(1,(char)(n%10+'0')) + ret;
		n/=10;
	}

	return ret;
}

void Core::ROIInteractorHelper::CreateMaskImage( 
	Core::DataEntity::Pointer inputDataEntity )
{
	Core::DataEntity::Pointer maskImage;
	maskImage = m_maskImageDataHolder->GetSubject();
	if ( maskImage.IsNull( ) || IsNewMaskImage() )
	{
		std::string maskImageName = "MaskImage";

		if (IsNewMaskImage()) {
			maskImageName = "Level " + GetNumMaskImagesString(GetNextRoiLevel()) + " ROI";
			SetNewMaskImage(false);
		} else {
			SetNextRoiLevel(1);
		}

		std::cout << "ROIInteractorHelper::CreateMaskImage Image is NULL, build it" << std::endl;
		maskImage = Core::DataEntity::New( ); 
		maskImage->SetType( Core::DataEntityType( Core::ImageTypeId | Core::ROITypeId ) );
		maskImage->GetMetadata()->SetName( maskImageName );
		maskImage->Resize( inputDataEntity->GetNumberOfTimeSteps(), typeid( Core::vtkImageDataPtr ) );
		maskImage->SetFather( inputDataEntity );

		m_maskImageDataHolder->SetSubject( maskImage );

		// Create empty image
		Core::vtkImageDataPtr referenceImage; 
		inputDataEntity->GetProcessingData( referenceImage );
		vtkSmartPointer<vtkImageMathematics> operation = vtkSmartPointer<vtkImageMathematics>::New();
		operation->SetInput1(referenceImage);
		operation->SetInput2(referenceImage);
		operation->SetOperationToSubtract();
		operation->Update();

		for ( unsigned i = 0 ; i < inputDataEntity->GetNumberOfTimeSteps() ;i++ )
		{
			Core::vtkImageDataPtr workingImage; 
			maskImage->GetProcessingData( workingImage, i );
			workingImage->DeepCopy( operation->GetOutput() );
		}

	}
	
	//maskImage->UpdateRenderingRepresentation();
}

void Core::ROIInteractorHelper::SetEraseOnlySelectedColor(bool b) {
	m_bEraseOnlySelectedColor = b;
	if ( m_ContourInteractor.IsNotNull() )
	{
		m_ContourInteractor->SetEraseOnlySelectedColor(GetEraseOnlySelectedColor());
	}
}

bool Core::ROIInteractorHelper::GetEraseOnlySelectedColor() {
	return m_bEraseOnlySelectedColor;
}

Core::DataEntityHolder::Pointer Core::ROIInteractorHelper::GetMaskImageDataHolder()
{
	return m_maskImageDataHolder;
}
