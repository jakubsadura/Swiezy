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

#include "wx/string.h"

#include <blMitkUnicode.h>

#include "blVTKHelperTools.h"
#include "blMITKUtils.h"

#include "coreSurfaceSelectorWidget.h"
#include "coreKernel.h"
#include "coreDataEntityHelper.h"
#include "coreMultiRenderWindow.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreInputControl.h"
#include "coreDataEntityListBrowser.h"
#include "coreReportExceptionMacros.h"
#include "coreDataTreeHelper.h"
#include "coreSurfaceInteractorTriangleSelect.h"
#include "coreSurfaceInteractorSphereSelect.h"
#include "coreSurfaceInteractorBrushSelect.h"

#include "vtkStringArray.h"
#include "vtkPointData.h"

#include "wxMitkRenderWindow.h"
#include "mitkRenderingManager.h"

#include "vtkCellData.h"

#include "mitkMaterialProperty.h"

using namespace Core::Widgets;
using namespace mitk;


// Event the widget
BEGIN_EVENT_TABLE(SurfaceSelectorWidget, wxPanel)
  EVT_CHECKBOX(wxID_REMOVE_CELLS, SurfaceSelectorWidget::OnRemoveCells)
  EVT_CHECKBOX(wxID_LOCAL_REFINER, SurfaceSelectorWidget::OnLocalRefiner)
//  EVT_CHECKBOX(wxID_LOOP_SUBDIV, SurfaceSelectorWidget::OnLoopSubdivision)
  EVT_CHECKBOX(wxID_TAUBIN_SMOOTH, SurfaceSelectorWidget::OnTaubinSmooth)
//  EVT_CHECKBOX(wxID_EDGE_SWAP, SurfaceSelectorWidget::OnEdgeSwap)
  EVT_TOGGLEBUTTON(wxID_EnableSelection,SurfaceSelectorWidget::OnEnableSelection)
  EVT_CHECKBOX(wxID_SELECTCHKBOX,SurfaceSelectorWidget::OnCheckBoxSelect)
  EVT_CHECKBOX(wxID_UNSELECTCHKBOX,SurfaceSelectorWidget::OnCheckBoxUnselect)
  EVT_RADIOBUTTON(wxID_TRIANGLE, SurfaceSelectorWidget::OnRadBoxInteractorType)
  EVT_RADIOBUTTON(wxID_SPHERE, SurfaceSelectorWidget::OnRadBoxInteractorType)
  EVT_RADIOBUTTON(wxID_BRUSH, SurfaceSelectorWidget::OnRadBoxInteractorType)
  EVT_SPINCTRL(wxID_SIZESPINCTRL, SurfaceSelectorWidget::OnBrushSizeChanged)
  EVT_TEXT(wxID_SIZESPINCTRL, SurfaceSelectorWidget::OnBrushSizeChangedEvent)
END_EVENT_TABLE()

//!
SurfaceSelectorWidget::SurfaceSelectorWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style)
: coreSurfaceSelectorWidgetUI(parent, id, pos, size, style)
{
	SetName( wxT("Surface selector") );

	// Init input control widget
	m_AcquireInputWidget = new Core::Widgets::InputControl(
		this, 
		wxID_ANY,
		false);
	GetSizer()->Insert(0, m_AcquireInputWidget, 0, wxEXPAND | wxALL, 4);

	m_InteractorStateHolder = InteractorStateHolderType::New();
	
	m_InteractorStateHolder->AddObserver(this,
		&SurfaceSelectorWidget::OnModifiedInteractorState );

	m_InteractorStateHolder->SetSubject( INTERACTOR_DISABLED );
	m_interactorType = Core::SurfaceInteractor::TRIANGLE;

	m_removeArray = true;

	m_selectedPoint = Core::DataEntityHolder::New();
	m_selectionSphereHolder = Core::DataEntityHolder::New();

	m_Processor = Core::SurfaceInteractorProcessor::New();
	m_Processor->GetOutputDataEntityHolder(0)->AddObserver( 
		this, 
		&SurfaceSelectorWidget::OnModifiedOutputDataEntity );

	m_Processor->GetInputDataEntityHolder(0)->AddObserver( 
		this, 
		&SurfaceSelectorWidget::OnModifiedInput );

	radius = 5;
	UpdateWidgets();
}

//!
SurfaceSelectorWidget::~SurfaceSelectorWidget(void)
{
}

bool Core::Widgets::SurfaceSelectorWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = coreSurfaceSelectorWidgetUI::Enable( enable );

	if ( !enable &&
		m_SurfaceInteractor.IsNotNull())
	{
		StopInteraction();
	}

	return bReturn;
}

void Core::Widgets::SurfaceSelectorWidget::SetListBrowser( 
	Core::Widgets::DataEntityListBrowser* listBrowser )
{
	BaseWindow::SetListBrowser( listBrowser );

	m_AcquireInputWidget->SetDataEntityListBrowser( listBrowser ); 
	m_AcquireInputWidget->SetAcquiredInputDataHolder( m_Processor->GetInputDataEntityHolder(0) );
	m_AcquireInputWidget->SetHeaderText( "Input data" );

	SetDefaultAllowedInputDataTypes();
}

void Core::Widgets::SurfaceSelectorWidget::StartInteractor()
{
	Core::DataEntity::Pointer inputDataEntity; 
	inputDataEntity = m_Processor->GetInputDataEntity(0);
	if ( inputDataEntity.IsNull( ) )
	{
		return;
	}

	// Add input data entity to the rendering tree before creating the Surfaces
	if ( !GetRenderingTree( )->IsDataEntityRendered( inputDataEntity ) )
	{
		GetRenderingTree( )->Add( inputDataEntity );
	}

	//CreateSphere(inputDataEntity);
	
	CreateInteractor();

	// Disable scalar rendering
	mitk::DataTreeNode::Pointer node;
	boost::any anyData = GetRenderingTree()->GetNode( inputDataEntity );
	Core::CastAnyProcessingData( anyData, node );
	blMITKUtils::SetScalarMode( node, "", 2 );
}

void Core::Widgets::SurfaceSelectorWidget::StopInteraction()
{
    if (m_SurfaceInteractor.IsNotNull()) {
        m_SurfaceInteractor->DisconnectFromDataTreeNode();
    }
    m_SurfaceInteractor = NULL;	// remove data entities used for selection
 	Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
 	Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();
	if (m_selectionSphereHolder->GetSubject().IsNotNull())
		list->Remove( m_selectionSphereHolder->GetSubject() );

	if (m_Processor->GetInputDataEntity(0).IsNotNull() &&
		m_removeArray )
	{
		Core::vtkPolyDataPtr  polydataInput;
		m_Processor->GetProcessingData(0,polydataInput);
		if (polydataInput != NULL &&
			polydataInput->GetCellData()->HasArray("select"))
			polydataInput->GetCellData()->RemoveArray("select");
	}

	mitk::RenderingManager::GetInstance()->RequestUpdateAll();

	m_InteractorStateHolder->SetSubject( INTERACTOR_DISABLED );
}

Core::Widgets::SurfaceSelectorWidget::InteractorStateHolderType::Pointer 
Core::Widgets::SurfaceSelectorWidget::GetInteractorStateHolder() const
{
	return m_InteractorStateHolder;
}

void Core::Widgets::SurfaceSelectorWidget::UpdateWidgets()
{
	

	Core::DataEntity::Pointer inputDataEntity; 
	inputDataEntity = m_Processor->GetInputDataEntity(0);

	if ( inputDataEntity.IsNotNull())
	{
		radio_btn_1->SetValue( m_interactorType == SurfaceInteractor::TRIANGLE );
		radio_btn_2->SetValue( m_interactorType == SurfaceInteractor::SPHERE );
		radio_btn_3->SetValue( m_interactorType == SurfaceInteractor::BRUSH );
	}
	
	if (m_SurfaceInteractor.IsNotNull())
		radius = m_SurfaceInteractor->GetValue();
	else 
		radius = 5;

}

void Core::Widgets::SurfaceSelectorWidget::UpdateData()
{
	m_SurfaceInteractor->SetValue(radius);
}

void SurfaceSelectorWidget::OnModifiedInput()
{
	try{

		UpdateWidgets();

		if ( !IsEnabled() && m_SurfaceInteractor.IsNotNull()  )
		{
			StopInteraction();
			return;
		}

		// If the input changed -> Stop interaction
		if ( m_InteractorStateHolder->GetSubject() == INTERACTOR_ENABLED )
		{
			StopInteraction();
		}

	}
	coreCatchExceptionsLogAndNoThrowMacro( SurfaceSelectorWidget::OnModifiedInput );
}

void Core::Widgets::SurfaceSelectorWidget::SetTimeStep( int time )
{
	try
	{
		UpdateWidgets( );
	}
	coreCatchExceptionsLogAndNoThrowMacro( 
		SurfaceSelectorWidget::OnModifiedTimeStep );
}

void Core::Widgets::SurfaceSelectorWidget::OnSelectedSurface()
{
	try
	{
		UpdateWidgets();
	}
	coreCatchExceptionsLogAndNoThrowMacro( 
		SurfaceSelectorWidget::OnSelectedSurface );
}

void Core::Widgets::SurfaceSelectorWidget::CreateInteractor()
{
	Core::DataEntity::Pointer inputDataEntity; 
	inputDataEntity = m_Processor->GetInputDataEntity(0);

	if (m_SurfaceInteractor.IsNotNull())
	{
		m_removeArray = false;
		StopInteraction();
		m_removeArray = true;
	}

	CreateSphere(inputDataEntity);

	// Create interactor
	switch ( m_interactorType )
	{

	// Triangle
	case Core::SurfaceInteractor::TRIANGLE:
		m_SurfaceInteractor = Core::SurfaceInteractorTriangleSelect::New( 
			GetRenderingTree( ), 
			m_selectedPoint,
			m_Processor->GetInputDataEntityHolder(0) );
		break;

	// Sphere
	case Core::SurfaceInteractor::SPHERE:
		{
			Core::SurfaceInteractorSphereSelect::Pointer interactor;
			 interactor = Core::SurfaceInteractorSphereSelect::New( 
				GetRenderingTree( ), 
				m_selectedPoint,
				m_Processor->GetInputDataEntityHolder(0));
			 interactor->SetSelectionSphereHolder(m_selectionSphereHolder);
			m_SurfaceInteractor = interactor ;
		}
		break;
		// Brush
	case Core::SurfaceInteractor::BRUSH:
		{
			Core::SurfaceInteractorBrushSelect::Pointer surfaceinter= Core::SurfaceInteractorBrushSelect::New( 
				GetRenderingTree( ), 
				m_selectedPoint,
				m_Processor->GetInputDataEntityHolder(0));
			m_SurfaceInteractor  = surfaceinter;
			m_SurfaceInteractor->SetValue(radius);
						}
		break;
	}


	m_SurfaceInteractor->ConnectToDataTreeNode();
	m_SurfaceInteractor->SetFlagSelection(checkbox_1->GetValue());
	m_InteractorStateHolder->SetSubject( INTERACTOR_ENABLED );

}

void Core::Widgets::SurfaceSelectorWidget::OnRadBoxInteractorType( 
	wxCommandEvent& event )
{
	if (radio_btn_1->GetValue())
		m_interactorType = Core::SurfaceInteractor::TRIANGLE;
	else if (radio_btn_2->GetValue())
		m_interactorType = Core::SurfaceInteractor::SPHERE;
	else if (radio_btn_3->GetValue())
	{
		m_interactorType = Core::SurfaceInteractor::BRUSH;
	}

	// If the input changed -> Create new interaction
	if ( m_InteractorStateHolder->GetSubject() == INTERACTOR_ENABLED )
	{
		CreateInteractor();
	}	
}

void Core::Widgets::SurfaceSelectorWidget::OnModifiedOutputDataEntity()
{

	//set input representation to wireframe
	Core::DataEntity::Pointer inputDataEntity;
	inputDataEntity = m_Processor->GetInputDataEntity( 0 );

	// Hide input if is different from output and output is not empty
	if ( m_Processor->GetOutputDataEntity( 0 ).IsNotNull() && 
		m_Processor->GetOutputDataEntity( 0 ) != inputDataEntity )
	{
		GetRenderingTree( )->Show( inputDataEntity, false );
	/*	mitk::DataTreeNode::Pointer node = GetRenderingTree()->GetNode(inputDataEntity);
		node->SetOpacity(0.8);
		mitk::MaterialProperty* material = dynamic_cast<mitk::MaterialProperty*>(
			node->GetProperty("material"));
		material->SetRepresentation(mitk::MaterialProperty::Wireframe);
		material->SetDiffuseColor(1.0,0.0,0.0);
		node->ReplaceProperty("material",material);*/
	}

	Core::DataTreeHelper::PublishOutput( 
			m_Processor->GetOutputDataEntityHolder( 0 ), 
			GetRenderingTree( ) );
}

void Core::Widgets::SurfaceSelectorWidget::SetAllowedInputDataTypes( 
	Core::DataEntityType val )
{
	m_AcquireInputWidget->SetAllowedInputDataTypes( val );
}

void Core::Widgets::SurfaceSelectorWidget::SetDefaultAllowedInputDataTypes()
{
	m_AcquireInputWidget->SetAllowedInputDataTypes(Core::SurfaceMeshTypeId);
/*		Core::SurfaceInteractorPointSet::GetInputTypes() |
		Core::SurfaceInteractorPointSelect::GetInputTypes()  );*/
}

void Core::Widgets::SurfaceSelectorWidget::SetInputDataEntity( Core::DataEntity::Pointer val )
{
	m_Processor->GetInputDataEntityHolder(0)->SetSubject( val );
}

Core::DataEntity::Pointer Core::Widgets::SurfaceSelectorWidget::GetInputDataEntity( )
{
	return m_Processor->GetInputDataEntity(0);
}


void Core::Widgets::SurfaceSelectorWidget::SetInteractorType( 
		Core::SurfaceInteractor::INTERACTOR_TYPE interactorType )
{
	m_interactorType = interactorType;
}

Core::BaseProcessor::Pointer Core::Widgets::SurfaceSelectorWidget::GetProcessor()
{
	return NULL;
}



void Core::Widgets::SurfaceSelectorWidget::CreateSphere( 
	Core::DataEntity::Pointer dataEntity )
{

	Core::vtkPolyDataPtr pointSet = Core::vtkPolyDataPtr::New( );
	Core::DataEntity::Pointer point = m_selectedPoint->GetSubject();
	if (point.IsNull())
	{
		point = Core::DataEntity::New( Core::PointSetTypeId ); 
		point->GetMetadata()->SetName( "center point" );
		point->AddTimeStep( pointSet );
		m_selectedPoint->SetSubject( point );
	}

	if (m_interactorType == Core::SurfaceInteractor::SPHERE)
	{
		Core::DataEntity::Pointer Sphere;
		Sphere = m_selectionSphereHolder->GetSubject();
		if ( Sphere.IsNull( ) )
		{
			Sphere = Core::DataEntity::New( Core::SurfaceMeshTypeId );
			Sphere->GetMetadata()->SetName( "SelectionSphere" );
			Sphere->Resize( 1, typeid( Core::vtkPolyDataPtr ) );
			Sphere->SetFather( dataEntity );

			m_selectionSphereHolder->SetSubject( Sphere );

			// Create empty image
			Core::vtkPolyDataPtr referenceSphere; 
			Sphere->GetProcessingData( referenceSphere );
			vtkSmartPointer<vtkSphereSource> selSphere = vtkSmartPointer<vtkSphereSource>::New();
			selSphere->SetThetaResolution(50);
			selSphere->SetPhiResolution(50);
			selSphere->SetRadius(0.1);
			selSphere->Update();
			referenceSphere->DeepCopy(selSphere->GetOutput());
		}

		Core::DataEntityHelper::AddDataEntityToList( m_selectionSphereHolder, false );

	}
	
}

void Core::Widgets::SurfaceSelectorWidget::OnModifiedInteractorState()
{
	if ( m_InteractorStateHolder->GetSubject() == INTERACTOR_ENABLED)
	{
		checkbox_1->Enable();
		checkbox_2->Enable();
		sizer_2_staticbox->Enable();
		sizer_6_staticbox->Enable();
		radio_btn_1->Enable();
		radio_btn_2->Enable();
		radio_btn_3->Enable();
		label_3->Enable(radio_btn_3->GetValue());
		spin_ctrl_1->Enable(radio_btn_3->GetValue());
		m_removeCellsCkbox->Enable();
		m_LocalRefinerCkbox->Enable();
		//m_LoopSubdivisionCkbox->Enable();
		m_TaubinSmoothCkbox->Enable();
		//m_EdgeSwapCkbox->Enable();
		m_btnEnableSelection->SetLabel("Disable Selection");
	}
	else if (m_InteractorStateHolder->GetSubject() == INTERACTOR_DISABLED)
	{
		checkbox_1->Disable();
		checkbox_2->Disable();
		sizer_2_staticbox->Disable();
		sizer_6_staticbox->Disable();
		radio_btn_1->Disable();
		radio_btn_2->Disable();
		radio_btn_3->Disable();
		label_3->Disable();
		spin_ctrl_1->Disable();
		m_removeCellsCkbox->Disable();
		m_LocalRefinerCkbox->Disable();
		//m_LoopSubdivisionCkbox->Disable();
		m_TaubinSmoothCkbox->Disable();
		//m_EdgeSwapCkbox->Disable();
		m_btnEnableSelection->SetLabel("Enable Selection");
	}

	m_btnEnableSelection->SetValue( IsSelectionEnabled());
}

void Core::Widgets::SurfaceSelectorWidget::OnRemoveCells( wxCommandEvent& event )
{
	if (m_removeCellsCkbox->GetValue() &&
		m_InteractorStateHolder->GetSubject()==INTERACTOR_ENABLED)
	{
		m_Processor->SetProcessorType(Core::SurfaceInteractorProcessor::RemoveCells);
		try
		{
			m_Processor->Update();
		}
		catch(...)
		{
			m_removeCellsCkbox->SetValue(false);
			throw;
		}
		m_removeCellsCkbox->SetValue(false);
		
	}
	StopInteraction();
}

void Core::Widgets::SurfaceSelectorWidget::OnLocalRefiner( wxCommandEvent& event )
{
	if (m_LocalRefinerCkbox->GetValue() &&
		m_InteractorStateHolder->GetSubject()==INTERACTOR_ENABLED)
	{
		m_Processor->SetProcessorType(Core::SurfaceInteractorProcessor::LocalRefiner);
		try
		{
			m_Processor->Update();
		}
		catch(...)
		{
			m_LocalRefinerCkbox->SetValue(false);
			throw;
		}

		m_LocalRefinerCkbox->SetValue(false);
	}
	StopInteraction();
}

void Core::Widgets::SurfaceSelectorWidget::OnLoopSubdivision( wxCommandEvent& event )
{
	/*if (m_LoopSubdivisionCkbox->GetValue() &&
		m_InteractorStateHolder->GetSubject()==INTERACTOR_ENABLED)
	{
		m_Processor->SetProcessorType(Core::SurfaceInteractorProcessor::LoopSubdivision);
		try
		{
			m_Processor->Update();
		}
		catch(...)
		{
			m_LoopSubdivisionCkbox->SetValue(false);
			throw;
		}

		m_LoopSubdivisionCkbox->SetValue(false);
	}
	StopInteraction();*/
}

void Core::Widgets::SurfaceSelectorWidget::OnEdgeSwap( wxCommandEvent& event )
{
	/*if (m_EdgeSwapCkbox->GetValue() &&
		m_InteractorStateHolder->GetSubject()==INTERACTOR_ENABLED)
	{
		m_Processor->SetProcessorType(Core::SurfaceInteractorProcessor::EdgeSwap);
		try
		{
			m_Processor->Update();
		}
		catch(...)
		{
			m_EdgeSwapCkbox->SetValue(false);
			throw;
		}

		m_EdgeSwapCkbox->SetValue(false);
	}
	StopInteraction();*/
}

void Core::Widgets::SurfaceSelectorWidget::OnTaubinSmooth( wxCommandEvent& event )
{
	if (m_TaubinSmoothCkbox->GetValue() &&
		m_InteractorStateHolder->GetSubject()==INTERACTOR_ENABLED)
	{
		m_Processor->SetProcessorType(Core::SurfaceInteractorProcessor::TaubinSmooth);
		try
		{
			m_Processor->Update();
		}
		catch(...)
		{
			m_TaubinSmoothCkbox->SetValue(false);
			throw;
		}

		m_TaubinSmoothCkbox->SetValue(false);
	}
	StopInteraction();
}

bool Core::Widgets::SurfaceSelectorWidget::IsSelectionEnabled()
{
	return m_InteractorStateHolder->GetSubject() == INTERACTOR_ENABLED;
}

void Core::Widgets::SurfaceSelectorWidget::OnEnableSelection( wxCommandEvent &event )
{
	if (m_btnEnableSelection->GetValue() )
	{
		StartInteractor();
		if (GetInputDataEntity().IsNull())
			m_btnEnableSelection->SetValue(false);
	}
	else
	{
		if (m_SurfaceInteractor.IsNotNull())
			StopInteraction();
		m_btnEnableSelection->SetLabel("Enable Selection");
	}
}

void Core::Widgets::SurfaceSelectorWidget::OnCheckBoxSelect( wxCommandEvent &event )
{ 
	if (m_SurfaceInteractor.IsNotNull() && checkbox_1->GetValue())
		m_SurfaceInteractor->SetFlagSelection(true);
	
	checkbox_2->SetValue(false);
}
void Core::Widgets::SurfaceSelectorWidget::OnCheckBoxUnselect( wxCommandEvent &event )
{
	if (m_SurfaceInteractor.IsNotNull() && checkbox_2->GetValue())
		m_SurfaceInteractor->SetFlagSelection(false);
	
	checkbox_1->SetValue(false);
}

void Core::Widgets::SurfaceSelectorWidget::OnBrushSizeChanged(wxSpinEvent &event )
{
	if (m_SurfaceInteractor.IsNotNull() && radio_btn_3->GetValue())
		m_SurfaceInteractor->SetValue(spin_ctrl_1->GetValue());
}

void Core::Widgets::SurfaceSelectorWidget::OnBrushSizeChangedEvent( wxCommandEvent &event )
{
	if (m_SurfaceInteractor.IsNotNull() && radio_btn_3->GetValue())
		m_SurfaceInteractor->SetValue(spin_ctrl_1->GetValue());
}