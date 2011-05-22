/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreVolumeMeshMaterialPropertiesWidget.h"
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreDataEntityHelper.txx"
#include "coreDataEntityHolder.h"
#include "coreAssert.h"
#include "coreVTKUnstructuredGridHolder.h"
#include "coreRenderingTreeMITK.h"
#include "coreMultiRenderWindowMITK.h"

using namespace Core::Widgets;
using namespace mitk;

//!
VolumeMeshMaterialPropertiesWidget::VolumeMeshMaterialPropertiesWidget(
	wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxMitkMaterialEditorWidget(parent, id, pos, size, style, name)
{
	this->SetAutoLayout(true);
}

//!
VolumeMeshMaterialPropertiesWidget::~VolumeMeshMaterialPropertiesWidget(void)
{
}


/** When the input changes, update the widget with the new data */
void VolumeMeshMaterialPropertiesWidget::OnInputHolderChangeSubject(void)
{
	coreAssertMacro(this->m_InputHolder.IsNotNull() && "The holder may not be null! Have you initialized this widget and its holder properly?");
	Core::DataEntity::Pointer dataEntity = this->m_InputHolder->GetSubject();

	if(dataEntity.IsNotNull() && 
		dataEntity->IsVolumeMesh() && 
		GetMultiRenderWindow() && 
		GetMultiRenderWindow()->GetRenderingTree().IsNotNull() )
	{
		boost::any anyData = GetMultiRenderWindow()->GetRenderingTree()->GetNode( dataEntity );
		mitk::DataTreeNode::Pointer node;
		Core::CastAnyProcessingData( anyData, node );
		this->SetDataTreeNode(node);
		this->SetDefaultScalarArray();
	}
	else
		this->SetDataTreeNode(NULL);
}

void Core::Widgets::VolumeMeshMaterialPropertiesWidget::SetMultiRenderWindow( 
	Core::Widgets::RenderWindowBase* val )
{
	Core::BaseWindow::SetMultiRenderWindow( val );

	mitk::wxMitkRenderWindow* renderWindow = NULL;
	MultiRenderWindowMITK* mitkRenderWindow;
	mitkRenderWindow = dynamic_cast<MultiRenderWindowMITK*> ( val );
	if ( mitkRenderWindow && mitkRenderWindow->Get3D( ) )
	{
		renderWindow = mitkRenderWindow->Get3D( )->GetMitkRenderWindow( );
	}

	SetMitkRenderWindow( renderWindow );
}