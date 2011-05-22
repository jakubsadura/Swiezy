/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "coreImageInfoWidget.h"
#include "CILabBoostMacros.h"
#include "boost/format.hpp"
#include <wx/textctrl.h>
#include "coreReportExceptionMacros.h"
#include "coreDisplayPixelValuePolicy.h"
#include "coreImageDataEntityMacros.h"
#include "coreDataEntityUtilities.h"
#include "coreDataEntityUtilities.txx"
#include "mitkGlobalInteraction.h"

using namespace Core::Widgets;


Core::ImageInfoMediator::ImageInfoMediator()
{
	Core::Initialize(this->m_ImageInformationHolder, Core::ImageInformation::New());
}

Core::ImageInfoMediator::~ImageInfoMediator()
{
	m_InputImageDataEntityHolder->RemoveObserver(
		this,
		&ImageInfoMediator::OnNewImageDataEntity,
		Core::DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT);
	RemoveInteractor( );
}

void Core::ImageInfoMediator::Init( 
	Core::Widgets::ImageInfoWidget* widget, 
	Core::DataEntityHolder::Pointer inputImageDataEntityHolder 
)
{
	widget->Init(this->m_ImageInformationHolder);
	m_InputImageDataEntityHolder = inputImageDataEntityHolder;
	m_InputImageDataEntityHolder->AddObserver
	(
		this,
		&ImageInfoMediator::OnNewImageDataEntity,
		Core::DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT
	);
	this->OnNewImageDataEntity();
}

void Core::ImageInfoMediator::OnNewImageDataEntity()
{
	try
	{
		RemoveInteractor();
		AddInteractor( );
	}
	coreCatchExceptionsReportAndNoThrowMacro("ImageInfoWidget::OnNewImageDataEntity");
}

void Core::ImageInfoMediator::RemoveInteractor()
{
	try
	{
		if ( this->m_MouseTracker.IsNotNull( ) )
		{
			//! is subject is null, detach the interactor
			mitk::GlobalInteraction::GetInstance()->RemoveListener
				(
				static_cast<mitk::StateMachine::Pointer>(this->m_MouseTracker)
				);
			this->m_MouseTracker = NULL;
			this->m_ImageInformationHolder->GetSubject()->coordinateIsInsideImage = false;
			this->m_ImageInformationHolder->NotifyObservers();
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro("Core::ImageInfoMediator::RemoveInteractor");
}

void Core::ImageInfoMediator::AddInteractor()
{
	try
	{
		if( this->m_InputImageDataEntityHolder->GetSubject().IsNotNull() && 
			this->m_InputImageDataEntityHolder->GetSubject()->IsImage() )
		{
			// Get Number of components
			blTagMap::Pointer properties = blTagMap::New( );
			m_InputImageDataEntityHolder->GetSubject()->GetTimeStep( 0 )->GetData( properties );
			if ( properties.IsNotNull() )
			{
				blTag::Pointer tag;
				tag = properties->FindTagByName( "NumberOfComponents" );
				if ( tag.IsNotNull() && tag->GetValueCasted<int>()== 1 ) 
				{
			 			try
			 			{
			 				coreImageDataEntityItkMacro(this->m_InputImageDataEntityHolder->GetSubject(), 
			 							    this->InstantiatePixelPolicy)
			 				this->m_MouseTracker = Core::PointTrackingInteractorWithPolicy::New("PositionTracker",
			 												    this->m_IntensityValuePolicy);
			 
			 				//The interactor must be registered as a listener
			 				mitk::GlobalInteraction::GetInstance()->AddListener(
			 					static_cast<mitk::StateMachine::Pointer>(this->m_MouseTracker)
			 					);
			 			}
			 			catch( Core::Exceptions::UnknownImageTypeException& )
			 			{
			 				// If the image cannot be converted to itk, then do not show any pixel values...
			 				typedef itk::Image<double, 3> itkDoubleImageType;
			 				this->InstantiatePixelPolicy<itkDoubleImageType>(itkDoubleImageType::Pointer(NULL));
			 			}
				}}
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro("Core::ImageInfoMediator::AddInteractor");
}
