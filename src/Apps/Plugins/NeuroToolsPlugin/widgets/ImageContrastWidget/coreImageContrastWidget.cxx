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

#include "coreImageContrastWidget.h"
#include "coreMultiRenderWindow.h"
#include "vtkColorTransferFunction.h"
#include "blMITKUtils.h"
#include "mitkLookupTableProperty.h"
#include "wxMitkRenderWindow.h"
#include "blMitkUnicode.h"
#include "wxMitkSelectableGLWidget.h"
#include "coreDataTreeHelper.h"
#include "coreMultiRenderWindowMITK.h"

#include "Contrast.xpm"


using namespace Core::Widgets;

ImageContrastWidget::ImageContrastWidget(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style)
: coreImageContrastWidgetUI(parent, id, pos, size, style)
{
	SetName( "Image Contrast Widget" );
	//SetBitmap( contrast_xpm );

	m_bmpAdd->SetBitmap( add_xpm );
	m_bmpSubstract->SetBitmap( substract_xpm );

	m_InputDataHolder = Core::DataEntityHolder::New();
	m_InputDataHolder->SetName( _U( GetName() ) + ": Input" );
	m_InputDataHolder->AddObserver( 
		this, 
		&ImageContrastWidget::OnModifiedInput, 
		DH_SUBJECT_MODIFIED );
	m_InputDataHolder->AddObserver( 
		this, 
		&ImageContrastWidget::OnNewInput, 
		DH_NEW_SUBJECT );

	// Init input control widget
	m_AcquireInputWidget = new Core::Widgets::InputControl(
		this, 
		wxID_ANY );
	GetSizer()->Insert(0, m_AcquireInputWidget, 0, wxEXPAND | wxALL, 4);

	this->negativeSlider->Disable();
	this->positiveSlider->Disable();

	m_Tag = 0;
	m_LevelWindowWidget = NULL;
	//Enable(false);

}

ImageContrastWidget::~ImageContrastWidget(void)
{
	if (m_LevelWindowWidget  != NULL )
		m_LevelWindowWidget->GetLevelWindowManager()->RemoveObserver(m_Tag);

	if (m_InputDataHolder.IsNotNull())
		m_InputDataHolder->RemoveObserver(
		this, 
		&ImageContrastWidget::OnModifiedInput );

}

void ImageContrastWidget::SetListBrowser( 
	Core::Widgets::DataEntityListBrowser* dataEntityListBrowser )
{
	BaseWindow::SetListBrowser( dataEntityListBrowser );
	m_AcquireInputWidget->SetDataEntityListBrowser( dataEntityListBrowser ); 
	m_AcquireInputWidget->SetAcquiredInputDataHolder( 
		m_InputDataHolder,
		Core::ImageTypeId );
}


void ImageContrastWidget::SetMultiRenderWindow( 
	Core::Widgets::RenderWindowBase *multiRenderWindow )
{
	BaseWindow::SetMultiRenderWindow( multiRenderWindow );

	mitk::wxMitkLevelWindowWidget* levelWindowWidget = NULL;
	if ( multiRenderWindow )
	{
		Core::Widgets::MultiRenderWindowMITK* mitkRenderWindow;
		mitkRenderWindow = dynamic_cast<Core::Widgets::MultiRenderWindowMITK*> ( multiRenderWindow );
		if ( mitkRenderWindow )
		{
			levelWindowWidget = mitkRenderWindow->GetLevelWindowWidget();
		}
	}

	//Remove previous observer if needed
	if (m_LevelWindowWidget  != NULL )
		m_LevelWindowWidget->GetLevelWindowManager()->RemoveObserver(m_Tag);

	m_LevelWindowWidget = levelWindowWidget;

	// Add an observert to the levelWindowManager, to updtate the widget whenever a new level window is set
	if ( m_LevelWindowWidget )
	{
		m_Command = itk::SimpleMemberCommand< ImageContrastWidget >::New();
		m_Command->SetCallbackFunction( this, &ImageContrastWidget::UpdateNode );
		m_Tag = m_LevelWindowWidget->GetLevelWindowManager()->AddObserver(
			itk::ModifiedEvent(), 
			m_Command);
	}
}


void ImageContrastWidget::OnModifiedInput(void)
{
	if ( m_InputDataHolder->GetSubject().IsNotNull() )
	{
		this->negativeSlider->Enable(true);
		this->positiveSlider->Enable(true);
		UpdateNode( );
	}
	else
	{
		this->negativeSlider->Enable(false);
		this->positiveSlider->Enable(false);
	}

}

void Core::Widgets::ImageContrastWidget::OnNewInput( void )
{
	m_checkEnable->SetValue( false );
}

void ImageContrastWidget::OnSliderNegativeContrast(wxScrollEvent &event)
{
	UpdateNode();
}

void ImageContrastWidget::OnSliderPositiveContrast(wxScrollEvent &event)
{	
	UpdateNode();
}

void ImageContrastWidget::OnChkBoxEnable(wxCommandEvent &event)
{
	if (!m_checkEnable->GetValue() && GetRenderingTree().IsNotNull() )
	{
		Core::DataEntity::Pointer dataEntity;
		dataEntity = m_InputDataHolder->GetSubject();
		if ( dataEntity.IsNull() )
		{
			return;
		}

		mitk::DataTreeNode::Pointer node;
		boost::any anyData = GetRenderingTree()->GetNode( m_InputDataHolder->GetSubject() );
		Core::CastAnyProcessingData( anyData, node );
		if ( node.IsNull() )
		{
			return;
		}

		Core::Widgets::MultiRenderWindowMITK* mitkRenderWindow;
		mitkRenderWindow = dynamic_cast<Core::Widgets::MultiRenderWindowMITK*> ( GetMultiRenderWindow() );
		mitkRenderWindow->Get3D( )->GetMitkRenderWindow( )->EnableColorLegend( 
		false, "", node );

		//Core::DataTreeHelper::ApplyLookupTableToNMImage(node);

		this->negativeSlider->Enable(false);
		this->positiveSlider->Enable(false);

	}
	else if (m_checkEnable->GetValue())
	{
		this->negativeSlider->Enable(true);
		this->positiveSlider->Enable(true);
		UpdateNode();
	}

}

void ImageContrastWidget::UpdateWidget(void)
{
}


Core::BaseProcessor::Pointer Core::Widgets::ImageContrastWidget::GetProcessor()
{
	return NULL;
}

void Core::Widgets::ImageContrastWidget::UpdateNode()
{
	if ( GetRenderingTree().IsNull() || m_LevelWindowWidget == NULL || !IsEnabled() ||
		m_LevelWindowWidget->GetLevelWindowManager()->GetLevelWindowProperty().IsNull() ||
		!m_checkEnable->GetValue())
	{
		return;
	}

	Core::DataEntity::Pointer dataEntity;
	dataEntity = m_InputDataHolder->GetSubject();
	if ( dataEntity.IsNull() )
	{
		return;
	}

	mitk::DataTreeNode::Pointer node;
	boost::any anyData = GetRenderingTree()->GetNode( dataEntity );
	Core::CastAnyProcessingData( anyData, node );
	if ( node.IsNull() )
	{
		return;
	}

	// Get min and max thresholds
	// If you have any question, ask Martin Stegh�fer
	mitk::LevelWindow t;
	node->GetLevelWindow( t );
	float minPixel = t.GetLowerWindowBound();
	float maxPixel = t.GetUpperWindowBound();

	float upperPositiveLimit = std::max( 0.0f, maxPixel );
	float lowerPositiveLimit = std::max( 0.0f, minPixel );
	float upperNegativeLimit = std::min( 0.0f, maxPixel );
	float lowerNegativeLimit = std::min( 0.0f, minPixel );

	float positiveThreshold = negativeSlider->GetValue() * ( upperPositiveLimit - lowerPositiveLimit ) / 100 + lowerPositiveLimit;
	float negativeThreshold = positiveSlider->GetValue() * ( lowerNegativeLimit - upperNegativeLimit ) / 100 + upperNegativeLimit;
	float positiveThresholdIndex = ( positiveThreshold - minPixel ) / (maxPixel - minPixel) * 256;
	float negativeThresholdIndex = ( negativeThreshold - minPixel ) / (maxPixel - minPixel) * 256;
	float centerIndex = ( - minPixel ) / (maxPixel - minPixel) * 256;

	// Create LUT
	vtkSmartPointer<vtkLookupTable> vtkLut;
	vtkLut = vtkSmartPointer<vtkLookupTable>::New();
	vtkLut->SetNumberOfTableValues( 256 );
	vtkLut->SetTableRange( t.GetLowerWindowBound(), t.GetUpperWindowBound() );
	vtkLut->Build();

	// Negative values
	for ( int i = 0 ; i < 256 ; i++ )
	{
		double red = 0;
		double blue = 0;
		if ( i < negativeThresholdIndex )
		{
			blue = - ( i - centerIndex ) / centerIndex;
		}
		else  if ( i > positiveThresholdIndex )
		{
			red = ( i - centerIndex ) / (256 - centerIndex );
		}
		vtkLut->SetTableValue( i, red, 0, blue );
	}

	// Create the property
	mitk::LookupTableProperty::Pointer mitkLutProp;
	mitkLutProp = mitk::LookupTableProperty::New();
	mitk::LookupTable::Pointer mitkLut;
	mitkLut = mitk::LookupTable::New();
	mitkLut->SetVtkLookupTable( vtkLut );
	mitkLutProp->SetLookupTable(mitkLut);

	// Always send the Modified event to update all views
	node->ReplaceProperty( "LookupTable", mitkLutProp );

	Core::Widgets::MultiRenderWindowMITK* mitkRenderWindow;
	mitkRenderWindow = dynamic_cast<Core::Widgets::MultiRenderWindowMITK*> ( GetMultiRenderWindow() );
	mitkRenderWindow->Get3D( )->GetMitkRenderWindow( )->EnableColorLegend( 
		true, "", node );

	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

bool Core::Widgets::ImageContrastWidget::Enable( bool enable /* = true */ )
{
	bool retVal = coreImageContrastWidgetUI::Enable( enable );

	if (retVal && m_checkEnable->GetValue())
		UpdateNode();

	return retVal && m_checkEnable->GetValue();
}
