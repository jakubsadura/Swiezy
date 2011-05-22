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

// Core
#include "coreDataEntityInformation.h"
#include "coreReportExceptionMacros.h"
#include "coreDataEntity.h"
#include "coreInputControl.h"
#include "coreDataEntityHelper.h"
#include "coreDataContainer.h"
#include "coreKernel.h"
#include "coreDataEntityMetadata.h"
#include "coreBaseWindowFactories.h"
#include "coreWxMitkGraphicalInterface.h"

#include "Information.xpm"

using namespace Core::Widgets;

//!
DataEntityInformation::DataEntityInformation(wxWindow* parent, 
											wxWindowID id, 
											const wxPoint& pos, 
											const wxSize& size, 
											long style, 
											const wxString& name)
: wxScrolledWindow(parent, id, pos, size, style, name)
{
	SetBitmap( information_xpm );

	// Create the widgets
	m_StackControl = new wxWidgetStackControl(this, wxID_ANY);
	m_AcquireInput =  new InputControl(
						this, wxID_ANY,	true);
	m_StackControlMetadata = new wxWidgetStackControl(this, wxID_ANY);
	m_MetadataWidget = new Core::Widgets::MetadataWidget(m_StackControlMetadata,wxID_ANY);
	m_NumericDataWidget = new Core::Widgets::NumericDataWidget(m_StackControl,wxID_ANY);

	//m_AcquireInput->Hide();
	m_EmptyLabel = new wxStaticText(
		m_StackControl, 
		wxID_ANY, 
		wxT("\n\nThere are not available informations \nfor the current input data entity"), 
		wxDefaultPosition, 
		wxDefaultSize );

	// Add the widgets to the stackControl
	m_StackControl->Add(m_EmptyLabel);

	//Initialize Metadata Window
	m_EmptyLabelMetadata = new wxStaticText(
		m_StackControl, 
		wxID_ANY, 
		wxT("\n\nThere are not available metadata informations "), 
		wxDefaultPosition, 
		wxDefaultSize );
	m_StackControlMetadata->Add(m_EmptyLabelMetadata);
	m_StackControlMetadata->Add(m_MetadataWidget);
	m_StackControl->Add(m_NumericDataWidget);
	

	m_lblDataEntityID = new wxStaticText(this, wxID_ANY, wxT("ID"));
	m_txtDataEntityID = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxNO_BORDER);
	m_SizerDataEntityID = new wxBoxSizer(wxHORIZONTAL);
	m_SizerDataEntityID->Add(m_lblDataEntityID, 1, wxEXPAND, 0);
	m_SizerDataEntityID->Add(m_txtDataEntityID, 1, wxEXPAND, 0);

	// Init holders
	SetInputHolder(Core::DataEntityHolder::New());
	m_AcquireInput->SetAcquiredInputDataHolder( 
		m_InputHolder,
		Core::UnknownTypeId );
	
	set_properties( );
	do_layout( );

	m_StackControl->Raise(m_EmptyLabel);
	m_StackControlMetadata->Raise(m_EmptyLabelMetadata);
	//m_StackControlMetadata->Raise(m_NumericDataWidget);

	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );
	baseWindowFactory->GetFactoriesHolder()->AddObserver(
		this,
		&DataEntityInformation::UpdateRegisteredWindows );
	UpdateRegisteredWindows( );

}

void DataEntityInformation::set_properties()
{
	SetScrollRate(10, 10);
}

void DataEntityInformation::do_layout()
{
	// layout them
	wxBoxSizer* vlayout = new wxBoxSizer(wxVERTICAL);
	vlayout->Add(m_AcquireInput, 0, wxEXPAND | wxALL, 4);
	vlayout->Add(m_SizerDataEntityID, 0, wxEXPAND | wxALL, 4);
	vlayout->Add(m_StackControl, 0, wxEXPAND | wxALL, 4);
	vlayout->Add(m_StackControlMetadata, 0, wxEXPAND | wxALL, 4);


	SetSizer(vlayout);
	vlayout->Fit(this);

}
//!
DataEntityInformation::~DataEntityInformation(void)
{
	m_InputHolder->SetSubject( NULL );
	if ( m_InputHolder.IsNotNull() )
	{
		m_InputHolder->RemoveObserver(
			this, 
			&DataEntityInformation::OnInputHolderChanged, 
			Core::DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT);
	}
}

//!
void DataEntityInformation::SetListBrowser(DataEntityListBrowser* listBrowser)
{
	BaseWindow::SetListBrowser( listBrowser );
	m_AcquireInput->SetDataEntityListBrowser( listBrowser ); 
}

//!
void DataEntityInformation::OnInputHolderChanged(void)
{
	Core::DataEntity::Pointer dataEntity = m_InputHolder->GetSubject();

	// Clear old data
	ClearOldData();
		
	m_MetadataWidget->SetDataEntity(dataEntity);

	if(dataEntity.IsNotNull())
	{
		m_txtDataEntityID->SetValue( wxString::Format( "%d", dataEntity->GetId() ) );

		dataEntity->GetMetadata()->AddObserverOnChangedTag<DataEntityInformation>(
			this, 
			&DataEntityInformation::OnChangedTag);

		for ( int i = 0 ; i < m_StackControl->GetNumberOfWidgets( ) ; i++ )
		{
			wxWindow* win = m_StackControl->GetWidgetByIndex( i );
			DataInformationWidgetBase* info = dynamic_cast<DataInformationWidgetBase*> ( win );
			if ( info && info->IsValidData( dataEntity ) )
			{
				info->SetInputDataEntity( dataEntity );
				m_StackControl->Raise( win );
			}
		}

		// Check the child types first
		if ( dataEntity->IsImage() )
		{
			m_MetadataWidget->UpdateWidget();
			m_StackControlMetadata->Raise(m_MetadataWidget);
		}
		else if ( dataEntity->IsSurfaceMesh( ) )
		{
			if (dataEntity->GetNumberOfTimeSteps()>1)
			{
				m_MetadataWidget->UpdateWidget();
				m_StackControlMetadata->Raise(m_MetadataWidget);
			}
			else
				m_StackControlMetadata->Raise(m_EmptyLabelMetadata);

		}
		else if ( dataEntity->IsVolumeMesh( ) )
		{
			if (dataEntity->GetNumberOfTimeSteps()>1)
			{
				m_MetadataWidget->UpdateWidget();
				m_StackControlMetadata->Raise(m_MetadataWidget);
			}
		}
		else if ( dataEntity->IsSignal( ) )
		{
			m_MetadataWidget->UpdateWidget();
			m_StackControlMetadata->Raise(m_MetadataWidget);
		}
		else if ( dataEntity->IsNumericData() )
		{
			m_StackControl->Raise(m_NumericDataWidget);
			std::vector<blTagMap::Pointer> tagMapVector;
			Core::DataEntityHelper::GetProcessingData(m_InputHolder, tagMapVector);
			m_NumericDataWidget->UpdateWidget(tagMapVector);
		}
		else if ( dataEntity->IsVectorField() )
		{
			m_StackControl->Raise(m_EmptyLabel);
		}
		else if ( dataEntity->IsTransform() )
		{
			m_StackControl->Raise(m_EmptyLabel);
		}
		else if ( dataEntity->IsContour() )
		{
			m_StackControl->Raise(m_EmptyLabel);
		}
		else if ( dataEntity->IsMITKData() )
		{
			m_StackControl->Raise(m_EmptyLabel);
			m_StackControlMetadata->Raise(m_EmptyLabelMetadata);
		}
		else
		{
			m_StackControl->Raise(m_EmptyLabel); 
			m_StackControlMetadata->Raise(m_EmptyLabelMetadata);
		}
	}
	else
	{
		m_StackControl->Raise(m_EmptyLabel);
		m_StackControlMetadata->Raise(m_EmptyLabelMetadata);
	}

	FitInside( );
}

void Core::Widgets::DataEntityInformation::SetInputHolder( Core::DataEntityHolder::Pointer inputHolder )
{
	if ( m_InputHolder.IsNotNull() )
	{
		m_InputHolder->RemoveObserver(
			this, 
			&DataEntityInformation::OnInputHolderChanged, 
			Core::DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT);
	}
	m_InputHolder = inputHolder;
	m_InputHolder->SetName( _U( GetName() ) + ": Input" );

	m_InputHolder->AddObserver(
		this, 
		&DataEntityInformation::OnInputHolderChanged, 
		Core::DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT);
}


bool Core::Widgets::DataEntityInformation::Show( bool show /* = true */ )
{
	return wxPanel::Show( show );
}

void Core::Widgets::DataEntityInformation::OnChangedTag(
	blTagMap* renamedDataEntity,
	unsigned long)
{
	m_MetadataWidget->UpdateWidget(false); //do not reset modality name..otherwise changes to modality will be invalidated
}

void Core::Widgets::DataEntityInformation::ClearOldData()
{
	for ( int i = 0 ; i < m_StackControl->GetNumberOfWidgets( ) ; i++ )
	{
		wxWindow* win = m_StackControl->GetWidgetByIndex( i );

		DataInformationWidgetBase* info;
		info = dynamic_cast<DataInformationWidgetBase*> ( win );

		if ( info )
		{
			info->Clear();
		}
	}
}

Core::BaseProcessor::Pointer Core::Widgets::DataEntityInformation::GetProcessor()
{
	return NULL;
}

void Core::Widgets::DataEntityInformation::UpdateRegisteredWindows()
{
	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );
	
	std::list<std::string> windowsList;
	windowsList = baseWindowFactory->GetWindowsList( WIDGET_TYPE_DATA_INFORMATION );
	std::list<std::string>::iterator it;
	for ( it = windowsList.begin( ) ; it != windowsList.end() ; it++ )
	{
		std::list<std::string>::iterator itFound;
		itFound = std::find( m_RegisteredFactoryNameList.begin(), m_RegisteredFactoryNameList.end(), *it );
		if ( itFound == m_RegisteredFactoryNameList.end() )
		{
			BaseWindow *baseWin = baseWindowFactory->CreateWindow( *it, m_StackControl );
			wxWindow* win = dynamic_cast<wxWindow*> ( baseWin );

			DataInformationWidgetBase* widget = dynamic_cast<DataInformationWidgetBase*> ( win );
			if ( widget == NULL )
			{
				throw Core::Exceptions::Exception( 
					"VisualProperties::UpdateRegisteredWindows",
					"A Data Information window should derive from DataInformationWidgetBase" );
			}

			m_StackControl->Add( win );
			m_RegisteredFactoryNameList.push_back( *it );
		}
	}
}

/*
//! TODO if you want to take into account the changes of the metadata and to put it in the 
data information widget
you should also add the observer to changed tag like in the data entity list browser

dataEntity->GetMetadata( )->AddObserverOnChangedTag<DataEntityInformation>(
			this, 
			&DataEntityInformation::OnChangedTag);
void Core::Widgets::DataEntityInformation::OnChangedTag(
	blTagMap* tagMap,
	unsigned long tagId )
{
	if ( tagId != Core::DE_TAG_ED ||
		 tagId != Core::DE_TAG_ES)
	{
		return;
	}
	Core::DataEntityMetadata* metaData = dynamic_cast<Core::DataEntityMetadata*> (tagMap);
	if ( metaData == NULL )
	{
		return;
	}
}*/
