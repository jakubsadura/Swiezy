/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "ManualSegmentationPanelWidget.h"

// GuiBridgeLib
#include "gblWxBridgeLib.h"
#include "gblWxButtonEventProxy.h"

// Core
#include "coreVTKPolyDataHolder.h"
#include "coreDataEntityHelper.h"
#include "coreUserHelperWidget.h"
#include "coreRenderingTree.h"
#include "coreDataEntityListBrowser.h"
#include "coreReportExceptionMacros.h"
#include "corePluginTab.h"
#include "coreDataEntity.h"
#include "coreDataEntityList.h"
#include "coreDataEntityList.txx"
#include "coreProcessorInputWidget.h"

// STD
#include <limits>
#include <cmath>
#include <algorithm>
#include <sstream>

// Core
#include "coreDataTreeHelper.h"

#include "mitkLookupTableProperty.h"
#include "vtkPointData.h"


using namespace Core::Widgets;

// Event the widget
BEGIN_EVENT_TABLE(ManualSegmentationPanelWidget,ManualSegmentationPanelWidgetUI)
EVT_CHECKBOX(wxID_CHK_COLOR,Core::Widgets::ManualSegmentationPanelWidget::OnChkColorChanged)
	EVT_TEXT_ENTER(wxID_COMBO_LEVEL,Core::Widgets::ManualSegmentationPanelWidget::OnNewLabel)
	EVT_COMBOBOX(wxID_COMBO_LEVEL,Core::Widgets::ManualSegmentationPanelWidget::OnNewComboSelection)
	EVT_COMBOBOX(wxID_COMBO_LUT,Core::Widgets::ManualSegmentationPanelWidget::OnNewComboLUTSelection)
	EVT_BUTTON(wxID_BTN_ADD,Core::Widgets::ManualSegmentationPanelWidget::OnAddBtn)
	EVT_TOGGLEBUTTON(wxID_EnableInteraction, Core::Widgets::ManualSegmentationPanelWidget::OnEnableInteraction)
END_EVENT_TABLE()


ManualSegmentationPanelWidget::ManualSegmentationPanelWidget(
							  wxWindow* parent, 
							  int id,
							  const wxPoint& pos, 
							  const wxSize& size, 
							  long style) :
	ManualSegmentationPanelWidgetUI(parent, id, pos, size, style)
{
	try 
	{
		int nRI = ManualSegmentationProcessor::REFERENCE_IMAGE;
		int nIR = ManualSegmentationProcessor::INPUT_ROI;

		m_processor = ManualSegmentationProcessor::New();		
		m_processor->GetInputDataEntityHolder( nRI )->AddObserver(
			this,
			&ManualSegmentationPanelWidget::OnModifiedSelectedReferenceImageDataEntityHolder);
		m_processor->GetInputDataEntityHolder( nIR )->AddObserver(
			this,
			&ManualSegmentationPanelWidget::OnModifiedSelectedInputROIDataEntityHolder,
			Core::DH_SUBJECT_MODIFIED);
		m_processor->GetInputDataEntityHolder( nIR )->AddObserver(
			this,
			&ManualSegmentationPanelWidget::OnNewSelectedInputROIDataEntityHolder,
			Core::DH_NEW_SUBJECT);

		OnModifiedSelectedReferenceImageDataEntityHolder( );
		m_btnEnableInteraction->SetValue( false );

		//m_OutputPort = Core::BaseFilterOutputPort::New();
		m_ROIInteractorHelper = Core::ROIInteractorHelper::New();
		m_ROIInteractorHelper->SetNextRoiLevel(0);

		EnableGUI(false);
		m_roiColorSelector->Enable(false);
		m_chkEraseOnlySelectedColor->SetValue(true);

		SetName("Manual Segmentation");
	}
	coreCatchExceptionsReportAndNoThrowMacro( 
		"ManualSegmentationPanelWidget::ManualSegmentationPanelWidget" );

}

ManualSegmentationPanelWidget::~ManualSegmentationPanelWidget( ) 
{
	if ( m_ROIInteractorHelper.IsNotNull() ) m_ROIInteractorHelper->StopInteraction();
}

void ManualSegmentationPanelWidget::EnableGUI(bool bEnable)
{
	m_txtAdd->Enable(bEnable);
	m_btnAdd->Enable(bEnable);
	m_chkEraseOnlySelectedColor->Enable(bEnable);
	m_ComboLevel->Enable(bEnable);
	m_ComboLUT->Enable(bEnable);
}

void ManualSegmentationPanelWidget::OnModifiedSelectedReferenceImageDataEntityHolder()
{
	Core::DataEntity::Pointer dataEntity = m_processor->GetInputDataEntityHolder(ManualSegmentationProcessor::REFERENCE_IMAGE)->GetSubject();

	if ( dataEntity.IsNotNull() )
	{
		m_btnEnableInteraction->Enable(true);
	}
	else
	{
		m_btnEnableInteraction->Enable(false);
	}
}


void ManualSegmentationPanelWidget::OnModifiedSelectedInputROIDataEntityHolder()
{
	UpdateWidget();
}

void Core::Widgets::ManualSegmentationPanelWidget::OnNewSelectedInputROIDataEntityHolder()
{
	Core::DataEntity::Pointer dataEntity = m_processor->GetInputDataEntityHolder(
		ManualSegmentationProcessor::INPUT_ROI)->GetSubject();

	if ( dataEntity.IsNotNull() )
	{
		EnableGUI(true);
	}
	else
	{
		EnableGUI(false);
	}

	bool wasEnabled = IsSelectionEnabled();

	// If the data entity has changed->Stop current interaction
	if ( m_ROIInteractorHelper->GetMaskImageDataHolder()->GetSubject() !=
		m_processor->GetInputDataEntity( ManualSegmentationProcessor::INPUT_ROI ) ) 
	{
		StopInteraction();
	}

	// If it was enabled before, enable it again
	if ( wasEnabled && !IsSelectionEnabled( ) ) 
	{
		// Set new input -> ROIInteractor helper
		m_ROIInteractorHelper->GetMaskImageDataHolder()->SetSubject( 
			m_processor->GetInputDataEntity( ManualSegmentationProcessor::INPUT_ROI ) );

		StartInteractor();
	}

	UpdateWidget( );
}

void ManualSegmentationPanelWidget::OnInit()
{
	//Colors
	InitComboLUT();

	//
	m_bEraseOnlySel = true;
	m_countLabels = 0;
	m_Selection = -1;

	setNextRoiColor(  );

	m_ROIInteractorHelper->SetRenderingTree( GetRenderingTree() );
	if ( GetListBrowser() )
	{
		m_ROIInteractorHelper->SetSelectedDataHolder( GetListBrowser()->GetSelectedDataEntityHolder() );
		m_ROIInteractorHelper->OnModifiedSelected();
	}

	GetInputWidget(ManualSegmentationProcessor::REFERENCE_IMAGE)->Show(false);

}

std::string ManualSegmentationPanelWidget::RemoveSpaces(std::string str) 
{
	std::string ret = "";

	for ( int i = 0; i < str.size(); i++ )
	{
		if ( str[i] != ' ') ret += str[i];
	}

	return ret;
}

void ManualSegmentationPanelWidget::OnAddBtn(wxCommandEvent& event) 
{
	std::string strToAdd = m_txtAdd->GetValue().c_str( );
	if (RemoveSpaces(strToAdd).size()<1)
	{
		throw Exception( "You must specify a name for the level" );
	}

	m_ComboLevel->Append(strToAdd);
	m_Selection=m_countLabels;
	m_countLabels++;
	UpdateData();
}

void ManualSegmentationPanelWidget::OnNewLabel(wxCommandEvent& event) 
{
	std::string strToEdit = event.GetString().c_str( );
	m_ComboLevel->SetString(m_Selection,strToEdit);
	UpdateData();
}

void ManualSegmentationPanelWidget::OnNewComboSelection(wxCommandEvent& event) 
{
	m_Selection = event.GetInt();

	// Update data
	m_ROIInteractorHelper->SetNextRoiLevel( m_Selection+1 );

	UpdateWidget();
}

void ManualSegmentationPanelWidget::OnChkColorChanged(wxCommandEvent& event) 
{
	m_bEraseOnlySel = m_chkEraseOnlySelectedColor->GetValue();

	// Update data
	m_ROIInteractorHelper->SetEraseOnlySelectedColor( m_bEraseOnlySel );

	UpdateWidget();
}


void ManualSegmentationPanelWidget::UpdateWidget() 
{
	Core::DataEntity::Pointer dataEntity = m_processor->GetInputDataEntity( ManualSegmentationProcessor::INPUT_ROI );
	if (dataEntity.IsNull()) return;
	blTagMap::Pointer labelMap;
	DataEntityTag::Pointer tag = dataEntity->GetMetadata()->FindTagByName( "MultiROILevel" );
	if ( tag.IsNotNull( ) )
	{
		tag->GetValue( labelMap );
	}

	m_countLabels = 0;
	m_ComboLevel->Clear();
	if ( labelMap.IsNotNull() )
	{
		blTagMap::Iterator it = labelMap->GetIteratorBegin();
		for ( ; it!= labelMap->GetIteratorEnd(); it++ )
		{
			m_ComboLevel->Append(it->second->GetName());
			m_countLabels++;
		}

		//update selection
		m_ComboLevel->SetSelection(m_Selection);
	}

	//update color
	setNextRoiColor();

	// Init LUT colors
	if ( GetROINode() )
	{
		int lutType;
		GetROINode()->GetIntProperty( "blLookupTablesType", lutType );
		m_ComboLUT->SetSelection( lutType );
	}
}

//!
Core::BaseProcessor::Pointer ManualSegmentationPanelWidget::GetProcessor( )
{
	return m_processor.GetPointer( );
}

//!
void ManualSegmentationPanelWidget::StartInteractor( )
{
	m_ROIInteractorHelper->SetNextRoiLevel( m_Selection+1 );
	m_ROIInteractorHelper->SetEraseOnlySelectedColor( m_bEraseOnlySel );
	m_ROIInteractorHelper->StartInteraction();
	if ( m_ROIInteractorHelper->GetInteractorStateHolder( )->GetSubject() == Core::ROIInteractorHelper::INTERACTOR_DISABLED )
	{
		return;
	}

	m_processor->SetInputDataEntity( 
		ManualSegmentationProcessor::INPUT_ROI, 
		m_ROIInteractorHelper->GetMaskImageDataHolder()->GetSubject() );

	if ( !GetMultiRenderWindow() )
	{
		return ;
	}

	GetMultiRenderWindow()->GetMetadata()->AddTag( "AxisLocked", true );
	m_LevelWindowInteractorState = 
		GetMultiRenderWindow()->GetMetadata()->GetTagValue<bool>( "LevelWindowInteractor" );
	GetMultiRenderWindow()->GetMetadata()->AddTag( "LevelWindowInteractor", false );
	GetMultiRenderWindow()->GetMetadataHolder()->NotifyObservers();
	m_btnEnableInteraction->SetLabel("Disable");
	m_btnEnableInteraction->SetValue( true );

	UpdateWidget( );
}

//!
void ManualSegmentationPanelWidget::StopInteraction( )
{
	if ( m_ROIInteractorHelper.IsNotNull() )
	{
		m_ROIInteractorHelper->StopInteraction();
	}
	if ( GetMultiRenderWindow() )
	{
		GetMultiRenderWindow()->GetMetadata()->AddTag( "AxisLocked", false );
		GetMultiRenderWindow()->GetMetadata()->AddTag( "LevelWindowInteractor", m_LevelWindowInteractorState );
		GetMultiRenderWindow()->GetMetadataHolder()->NotifyObservers();
	}
	m_btnEnableInteraction->SetLabel("Enable");
	m_txtAdd->SetValue( "" );
	m_countLabels = 0;
	m_Selection = 0;
}

//!
bool ManualSegmentationPanelWidget::IsSelectionEnabled( )
{
	if ( m_ROIInteractorHelper.IsNull() )
	{
		return false;
	}
	return m_ROIInteractorHelper->GetInteractorStateHolder()->GetSubject() == 
		Core::ROIInteractorHelper::INTERACTOR_ENABLED;
}


bool ManualSegmentationPanelWidget::Enable( bool enable ) 
{
	bool bReturn = ManualSegmentationPanelWidgetUI::Enable( enable );
	if ( !enable ) 
	{
		StopInteraction( );
	}

	return bReturn;
}

void ManualSegmentationPanelWidget::InitComboLUT() 
{
	m_ComboLUT->Clear();
	for ( int i = 0 ; i < blLookupTables::GetNumberOfLuts(); i++ )
	{
		blLookupTables::LUT_TYPE lutType = blLookupTables::LUT_TYPE( i );
		m_ComboLUT->Append(wxT( blLookupTables::GetLUTName( lutType ) ));
	}
	m_ComboLUT->SetSelection(blLookupTables::LUT_TYPE_RAINBOW_HUE);
}

void ManualSegmentationPanelWidget::OnNewComboLUTSelection(wxCommandEvent& event) 
{
	blLookupTables::LUT_TYPE lutType;
	int currentSelection = m_ComboLUT->GetCurrentSelection();
	lutType = blLookupTables::LUT_TYPE( currentSelection );
	
	Core::DataEntityHolder::Pointer dataEntityHolder = m_processor->GetInputDataEntityHolder(ManualSegmentationProcessor::INPUT_ROI);
	if ( dataEntityHolder.IsNull() )
	{
		return;
	}

	Core::DataEntity::Pointer dataEntity = dataEntityHolder->GetSubject();
	if ( dataEntity.IsNull() ) 
	{
		return;
	}

	mitk::DataTreeNode::Pointer node;
	boost::any anyData = GetRenderingTree()->GetNode( dataEntity );
	Core::CastAnyProcessingData( anyData, node );
	blMITKUtils::ApplyLookupTableToImage(true,node,lutType);
	mitk::RenderingManager::GetInstance()->RequestUpdateAll( );

	UpdateWidget();
}

void ManualSegmentationPanelWidget::getTableColor(int i, double col[3]) 
{
	col[0] = 1;
	col[1] = 1;
	col[2] = 1;

	vtkLookupTable* vtkLut = GetLUT( );
	if ( vtkLut == NULL )
	{
		return;
	}

	vtkLut->GetColor( i, col );
}

void ManualSegmentationPanelWidget::setNextRoiColor() 
{
	// GetUpperWindowBound( ) minimum value is 16 
	if ( GetROINode().IsNotNull() )
	{
		mitk::LevelWindow levelWindow;
		GetROINode()->GetLevelWindow( levelWindow );

		double range[2];
		range[ 0 ] = levelWindow.GetLowerWindowBound();
		range[ 1 ] = levelWindow.GetUpperWindowBound();
		if ( GetLUT() )
		{
			GetLUT()->SetRange( range );
		}
	}

	double nextColor[ 3 ];
	getTableColor( m_Selection + 1, nextColor );
	m_roiColorSelector->SetRGBColour( nextColor );
}

void Core::Widgets::ManualSegmentationPanelWidget::OnEnableInteraction( wxCommandEvent& event )
{
	if (m_btnEnableInteraction->GetValue() == true)
	{
		StartInteractor();
	}
	else 
	{
		StopInteraction( );
	}
}

void Core::Widgets::ManualSegmentationPanelWidget::UpdateData()
{
	// Update ROI Interactor
	m_ROIInteractorHelper->SetNextRoiLevel( m_Selection+1 );
	m_ROIInteractorHelper->SetEraseOnlySelectedColor( m_bEraseOnlySel );

	Core::DataEntity::Pointer dataEntity;
	dataEntity = m_processor->GetInputDataEntity(ManualSegmentationProcessor::INPUT_ROI);
	if (dataEntity.IsNull()) return;

	wxArrayString arrayString = m_ComboLevel->GetStrings();
	int nItems = arrayString.GetCount();

	// Add metadata tags
	blTagMap::Pointer ROIlevels = blTagMap::New();
	for ( int i = 0; i < nItems; i++ )
	{
		std::string itemString = std::string(arrayString[i].c_str());
		ROIlevels->AddTag( itemString, i );
	}
	dataEntity->GetMetadata( )->AddTag( "MultiROILevel", ROIlevels );

	// Update image and VTK scalar range to update the legend
	if ( GetROINode().IsNotNull() )
	{
		mitk::Image::Pointer image;
		image = static_cast<mitk::Image*>( GetROINode()->GetData() );
		if (image.IsNotNull()) 
		{
			vtkImageData* vtkImage = image->GetVtkImageData( );
			if ( vtkImage )

			{
				vtkImage->GetPointData()->GetScalars()->Modified();
				vtkImage->Modified();
			}
		}

		// GetUpperWindowBound( ) minimum value is 16 
		mitk::LevelWindow levelWindow;
		GetROINode()->GetLevelWindow( levelWindow );

		double range[2];
		range[ 0 ] = levelWindow.GetLowerWindowBound();
		range[ 1 ] = levelWindow.GetUpperWindowBound();
		GetLUT()->SetRange( range );
	}


	// Notify observers
	dataEntity->Modified();
}

mitk::DataTreeNode::Pointer Core::Widgets::ManualSegmentationPanelWidget::GetROINode()
{
	if ( GetRenderingTree().IsNull() )
	{
		return NULL;
	}

	mitk::DataTreeNode::Pointer node;
	boost::any anyData = GetRenderingTree()->GetNode(
		m_processor->GetInputDataEntity(ManualSegmentationProcessor::INPUT_ROI) );
	Core::CastAnyProcessingData( anyData, node );
	return node;
}

vtkLookupTable* Core::Widgets::ManualSegmentationPanelWidget::GetLUT()
{
	mitk::DataTreeNode::Pointer node;
	node = GetROINode( );

	if ( node.IsNull() )
	{
		return NULL;
	}

	mitk::LookupTableProperty::Pointer mitkLutProp;
	node->GetProperty( mitkLutProp, "LookupTable" );
	mitk::LookupTable::Pointer mitkLut;
	if ( mitkLutProp.IsNull() )
	{
		return NULL;
	}
	mitkLut = mitkLutProp->GetLookupTable();

	vtkLookupTable *vtkLut = mitkLut->GetVtkLookupTable();
	return vtkLut;
}

void Core::Widgets::ManualSegmentationPanelWidget::SetRenderingTree( RenderingTree::Pointer tree )
{
	Core::BaseWindow::SetRenderingTree( tree );
	m_ROIInteractorHelper->SetRenderingTree( GetRenderingTree() );
}

