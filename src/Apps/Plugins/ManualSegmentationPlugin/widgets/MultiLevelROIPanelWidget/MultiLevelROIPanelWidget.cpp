/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "MultiLevelROIPanelWidget.h"

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
#include "vtkImageToImageStencil.h"

using namespace Core::Widgets;

// Event the widget
BEGIN_EVENT_TABLE(MultiLevelROIPanelWidget,MultiLevelROIPanelWidgetUI)
	EVT_COMBOBOX(wxID_CMB_LEVELS, Core::Widgets::MultiLevelROIPanelWidget::OnSelectLevel)
	EVT_BUTTON(wxID_BTN_PROPAGATE, Core::Widgets::MultiLevelROIPanelWidget::OnPropagateBtn)
	EVT_BUTTON(wxID_BTN_UPDATEMEASURES, Core::Widgets::MultiLevelROIPanelWidget::OnUpdateMeasuresBtn)
	EVT_BUTTON(wxID_BTN_CREATESIGNAL, Core::Widgets::MultiLevelROIPanelWidget::OnCreateSignalBtn)
END_EVENT_TABLE()


MultiLevelROIPanelWidget::MultiLevelROIPanelWidget(
						 wxWindow* parent, 
						 int id ,
						 const wxPoint& pos, 
						 const wxSize& size, 
						 long style) :
	MultiLevelROIPanelWidgetUI(parent, id, pos, size, style)
{
	try 
	{
		m_processor = MultiLevelROIProcessor::New();
		m_processor->GetOutputDataEntityHolder( 0 )->AddObserver( 
			this, 
			&MultiLevelROIPanelWidget::OnModifiedOutputPropagatedROI );

		m_processor->GetOutputDataEntityHolder( 1 )->AddObserver( 
			this, 
			&MultiLevelROIPanelWidget::OnModifiedSignal );

		m_processor->GetInputDataEntityHolder(0)->AddObserverOnModified(this,&MultiLevelROIPanelWidget::OnModifiedSelectedDataEntityHolder);

	} 	
	coreCatchExceptionsReportAndNoThrowMacro( 
		"MultiLevelROIPanelWidget::MultiLevelROIPanelWidget" );
}

MultiLevelROIPanelWidget::~MultiLevelROIPanelWidget( ) 
{
}

void MultiLevelROIPanelWidget::OnInit()
{
	m_selectedLevel = -1;
	EnableGUI(false);

}

void MultiLevelROIPanelWidget::EnableGUI(bool enable)
{
	m_comboBoxLevels->Enable(enable);
	m_btnPropagate->Enable(enable);
	label_3->Enable(enable);
	m_spinCtrlLeft->Enable(enable);
	label_8->Enable(enable);
	m_spinCtrlRight->Enable(enable);
	label_4->Enable(enable);
	m_measureMean->Enable(enable);
	label_7->Enable(enable);
	m_measureSTD->Enable(enable);
	label_5->Enable(enable);
	m_measureMin->Enable(enable);
	label_6->Enable(enable);
	label_2->Enable(enable);
	m_measureMax->Enable(enable);
	m_rb3d->Enable(enable);
	m_rbX->Enable(enable);
	m_rbY->Enable(enable);
	m_rbZ->Enable(enable);
	button_1->Enable(enable);
	label_1->Enable(enable);
	m_comboBoxLevels->Enable(enable);
	button_2->Enable(enable);
}

Core::BaseProcessor::Pointer MultiLevelROIPanelWidget::GetProcessor()
{
	return Core::BaseProcessor::Pointer(m_processor);
}


void MultiLevelROIPanelWidget::OnSelectLevel(wxCommandEvent &event)
{
	m_selectedLevel = event.GetInt()+1;
}

void MultiLevelROIPanelWidget::OnModifiedSelectedDataEntityHolder()
{
	Core::DataEntity::Pointer dataEntity = m_processor->GetInputDataEntityHolder(0)->GetSubject();
	//Core::DataEntity::Pointer dataEntity = GetListBrowser()->GetSelectedDataEntityHolder()->GetSubject();

	if (dataEntity.IsNull()) {
		EnableGUI(false);
		return;
	}
	if (dataEntity->GetType() & Core::ROITypeId) {

	} 
	else
	{
		EnableGUI(false);
		return;
	}

	EnableGUI(true);

	if ( dataEntity->GetNumberOfTimeSteps() < 2 )
	{
		m_btnPropagate->Enable(false);
		label_3->Enable(false);
		m_spinCtrlLeft->Enable(false);
		label_8->Enable(false);
		m_spinCtrlRight->Enable(false);
	}

	blTagMap::Pointer levels;
	DataEntityTag::Pointer tag = dataEntity->GetMetadata()->FindTagByName( "MultiROILevel" );
	if ( tag.IsNotNull( ) )
	{
		tag->GetValue( levels );
	}


	blTagMap::Iterator it;

	m_comboBoxLevels->Clear();

	if ( levels.IsNotNull() )
	{
		for ( it = levels->GetIteratorBegin(); it!= levels->GetIteratorEnd(); ++it )
		{
			m_comboBoxLevels->Append(it->second->GetName());
		}
	}

	m_selectedLevel = 1;
	m_comboBoxLevels->SetSelection(0);

}

void MultiLevelROIPanelWidget::OnModifiedOutputPropagatedROI()
{
	try{
		// Publish the output: add it to the rendering tree and show it
		Core::DataTreeHelper::PublishOutput( 
			m_processor->GetOutputDataEntityHolder( 0 ), 
			GetRenderingTree() );
	}
	coreCatchExceptionsLogAndNoThrowMacro( 
		"MultiLevelROIPanelWidget::OnModifiedOutputPropagatedROI")

}

void MultiLevelROIPanelWidget::OnModifiedSignal()
{
	try{
		// Publish the output: add it to the rendering tree and show it
		Core::DataTreeHelper::PublishOutput( 
			m_processor->GetOutputDataEntityHolder( 1 ), 
			GetRenderingTree() );
	}
	coreCatchExceptionsLogAndNoThrowMacro( 
		"MultiLevelROIPanelWidget::OnModifiedSignal")

}

void MultiLevelROIPanelWidget::OnPropagateBtn(wxCommandEvent& event)
{
	m_processor->SetTimeStep( GetTimeStep() );
	m_processor->SetRange(m_spinCtrlLeft->GetValue(),m_spinCtrlRight->GetValue());
	m_processor->Propagate();
}

void MultiLevelROIPanelWidget::OnCreateSignalBtn(wxCommandEvent& event)
{
	if ( m_selectedLevel>=0 )
	{
		m_processor->SetSelectedLevel(m_selectedLevel);
		int domain = m_rb3d->GetValue()*1 + m_rbX->GetValue()*2 + m_rbY->GetValue()*3 + m_rbZ->GetValue()*4;

		int orientation;
        int pos;
		switch (domain)
		{
		case 1:
			{
				orientation = -1;
				pos = 0;
			}
			break;
		case 2:
			{
				orientation = SlicePlane::Sagittal;
			}
			break;
		case 3:
			{
				orientation = SlicePlane::Frontal;
			}
			break;
		case 4:
			{
				orientation = SlicePlane::Transversal;
			}
			break;
		default:
			pos = -1;
			break;
		}


		std::list<Core::SlicePlane::Pointer> slicePlanesPositions;
		GetMultiRenderWindow()->GetSlicePlanesPositions( slicePlanesPositions );
		std::list<SlicePlane::Pointer>::iterator it;
		for ( it = slicePlanesPositions.begin() ; it!= slicePlanesPositions.end() ; it++ )
		{
			if ( (*it)->m_Orientation == orientation )
			{
				pos = (*it)->m_Pos;
			}
		}


		m_processor->SetDomain(domain);
		m_processor->SetPos(pos);
		m_processor->CreateSignal();
		UpdateMeasures();
	}
}

void MultiLevelROIPanelWidget::OnUpdateMeasuresBtn(wxCommandEvent& event)
{
	if ( m_selectedLevel>=0 )
	{
		CalculateMeasures();
		UpdateMeasures();
	}
	
}

void MultiLevelROIPanelWidget::UpdateMeasures()
{
	std::map<std::string,double> measures = m_processor->GetIntensityMeasures();
	
	char buffMean[100]; 
	char buffStd[100]; 
	char buffMin[100]; 
	char buffMax[100]; 

	sprintf(buffMean,"%f", measures["mean"]); 
	sprintf(buffStd,"%f", measures["std"]); 
	sprintf(buffMin,"%f", measures["min"]); 
	sprintf(buffMax,"%f", measures["max"]); 

	std::string meanStr(buffMean);
	std::string stdStr(buffStd);
	std::string minStr(buffMin);
	std::string maxStr(buffMax);

	m_measureMean->SetValue(meanStr);
	m_measureSTD->SetValue(stdStr);
	m_measureMin->SetValue(minStr);
	m_measureMax->SetValue(maxStr);
}

void MultiLevelROIPanelWidget::CalculateMeasures()
{
	m_processor->SetSelectedLevel(m_selectedLevel);		
	int domain = m_rb3d->GetValue()*1 + m_rbX->GetValue()*2 + m_rbY->GetValue()*3 + m_rbZ->GetValue()*4;

	int orientation;
	int pos;
	switch (domain)
	{
	case 1:
		{
			orientation = -1;
			pos = 0;
		}
		break;
	case 2:
		{
			orientation = SlicePlane::Sagittal;
		}
		break;
	case 3:
		{
			orientation = SlicePlane::Frontal;
		}
		break;
	case 4:
		{
			orientation = SlicePlane::Transversal;
		}
		break;
	default:
		pos = -1;
		break;
	}


	std::list<Core::SlicePlane::Pointer> slicePlanesPositions;
	GetMultiRenderWindow()->GetSlicePlanesPositions( slicePlanesPositions );
	std::list<SlicePlane::Pointer>::iterator it;
	for ( it = slicePlanesPositions.begin() ; it!= slicePlanesPositions.end() ; it++ )
	{
		if ( (*it)->m_Orientation == orientation )
		{
			pos = (*it)->m_Pos;
		}
	}

	m_processor->SetDomain(domain);
	m_processor->SetPos(pos);
	m_processor->CalculateIntensityMeasures( GetTimeStep() );	
}

bool MultiLevelROIPanelWidget::Enable( bool enable ) 
{
	bool bReturn = MultiLevelROIPanelWidgetUI::Enable( enable );
	if ( enable ) 
	{
	}
	else
	{
	}
	return bReturn;
}
