/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreMultiRenderWindowFocus.h"
#include "coreMultiRenderWindowMITK.h"
#include "coreDataEntity.h"

#include "blMITKUtils.h"


Core::Widgets::MultiRenderWindowFocus::MultiRenderWindowFocus()
{
	m_MultiRenderWindow = NULL;
}

Core::Widgets::MultiRenderWindowFocus::~MultiRenderWindowFocus()
{
	if ( m_SelectedDataEntity.IsNotNull() )
	{
		m_SelectedDataEntity->RemoveObserver( 
			this, 
			&MultiRenderWindowFocus::OnSelectedModified );
	}
}

void Core::Widgets::MultiRenderWindowFocus::Init( 
	Core::Widgets::MultiRenderWindowMITK* multiRenderWindow, 
	Core::DataEntityHolder::Pointer selectedDataEntity )
{
	m_MultiRenderWindow = multiRenderWindow;
	m_SelectedDataEntity = selectedDataEntity;
	m_SelectedDataEntity->AddObserver( 
		this, 
		&MultiRenderWindowFocus::OnSelectedModified );
}

void Core::Widgets::MultiRenderWindowFocus::OnSelectedModified()
{
	if ( m_SelectedDataEntity.IsNull() ||
		 m_SelectedDataEntity->GetSubject().IsNull( ) ||
		 !m_MultiRenderWindow->IsEnabled() )
	{
		return;
	}

	Core::DataEntity::Pointer dataEntity;
	dataEntity = m_SelectedDataEntity->GetSubject();

	if ( dataEntity->IsROI( ) )
	{
		// Center the 2D views to the first point of the mask image
		mitk::BaseData::Pointer mitkData;
		boost::any anyData = dataEntity->GetRenderingData( typeid( mitkData ) );
		Core::CastAnyProcessingData( anyData, mitkData );
		mitk::Image* image;
		image = dynamic_cast<mitk::Image*>(mitkData.GetPointer()); 
		if ( image != NULL )
		{
			int iTimePos = m_MultiRenderWindow->GetTimeNavigationController()->GetTime( )->GetPos();
			mitk::Point3D point;
			bool found;
			found = blMITKUtils::ExtractImagePoint( image, iTimePos, point );
			if ( found )
			{
				m_MultiRenderWindow->SelectSliceByPoint( point );
			}
		}
	}


}

