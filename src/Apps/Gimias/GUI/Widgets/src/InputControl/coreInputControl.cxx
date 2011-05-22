/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreInputControl.h"
#include "coreDataEntityInfoHelper.h"
#include "coreDataEntityListBrowser.h"
#include "coreDataEntityHelper.h"

using namespace Core::Widgets;

//!
InputControl::InputControl(
	wxWindow* parent, wxWindowID id, bool bAutomaticSelection, bool bSetDefaultDataEntity, 
	const wxPoint& pos, const wxSize& size, 
	long style, const wxString& name)
: BaseInputControl(parent, id, pos, size, style, name, bAutomaticSelection, bSetDefaultDataEntity)
{
	m_Modality = Core::UnknownModality;
	m_AllowedInputDataTypes = Core::UnknownTypeId;

	SetAcquiredInputDataHolder( DataEntityHolder::New() );
}

//!
InputControl::~InputControl(void)
{
}

void Core::Widgets::InputControl::SetAcquiredInputDataHolder( 
	DataHolderType::Pointer val, 
	int type,
	Core::ModalityType modality)
{
	BaseInputControl::SetAcquiredInputDataHolder( val.GetPointer() );
	m_AllowedInputDataTypes = Core::DataEntityType( type );
	m_Modality = modality;
	FillListBox( );
}

Core::DataEntityType 
Core::Widgets::InputControl::GetAllowedInputDataType() const
{
	return m_AllowedInputDataTypes;
}

void Core::Widgets::InputControl::SetAllowedInputDataTypes( 
	int val )
{
	m_AllowedInputDataTypes = Core::DataEntityType( val );
	
	FillListBox( );

	// Refresh the input
	Core::DataEntity::Pointer outputSubject;
	if ( !CheckAllowedInputDataTypes( 
		m_AcquiredInputDataHolder->GetSubject( ) ) )
	{
		m_AcquiredInputDataHolder->SetSubject( NULL );
	}

}

bool Core::Widgets::InputControl::CheckSubjectIsOk(
	Core::DataEntity::Pointer subject )
{
	bool bTypeIsOk = BaseInputControl::CheckSubjectIsOk( subject );
	if ( !bTypeIsOk )
	{
		return false;
	}

	bTypeIsOk = bTypeIsOk & CheckAllowedInputDataTypes( subject );
	if ( !bTypeIsOk )
	{
		return false;
	}

	// Check modality
	bTypeIsOk = bTypeIsOk && CheckModalityIsOk ( subject );
	if ( !bTypeIsOk )
	{
		return false;
	}

	return bTypeIsOk;
}

bool Core::Widgets::InputControl::CheckModalityIsOk(
	Core::DataEntity::Pointer &subject )
{
	bool bTypeIsOk = false;
	if ( subject.IsNotNull( ) )
	{
		bTypeIsOk = 
			m_Modality == Core::UnknownModality ||
			subject->GetMetadata( )->GetModality( ) == m_Modality;
	}

	return bTypeIsOk;
}


bool Core::Widgets::InputControl::CheckAllowedInputDataTypes( 
	Core::DataEntity::Pointer &subject )
{
	bool bTypeIsOk = false;
	if ( subject.IsNull( ) )
	{
		return false;
	}

	// Select the type of data that matches the criteria (e.g. Image == Image)
	bTypeIsOk = 
		m_AllowedInputDataTypes == UnknownTypeId ||
		( subject->GetType( ) & m_AllowedInputDataTypes );

	return bTypeIsOk;
}

bool Core::Widgets::InputControl::SetAcquiredInputData( 
	DataEntity::Pointer &subject )
{
	// We are going to change the acquired input
	if ( !CheckSubjectIsOk( subject ) )
	{
		return false;
	}

	bool ret = false;
	if ( m_AcquiredInputDataHolder.IsNotNull( ) )
	{
		// Always notify observers because when automatic is on, we
		// need to update the views sometimes
		// Modifictaion: In cardiac initialization, when the selection changes
		// and the input is the same, we need to update the observers (volume
		// computation), so we force the notification
		// Xavi: Changed with the new pipeline
		m_AcquiredInputDataHolder->SetSubject( subject );
		ret = true;
	}

	return ret;
}
