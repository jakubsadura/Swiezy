/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreVolumeImagePropertiesWidget.h"
#include "coreDataEntity.h"
#include "coreAssert.h"
#include "coreRenderingTreeMITK.h"
#include "coreMultiRenderWindowMITK.h"

using namespace Core::Widgets;
using namespace mitk;

//!
VolumeImagePropertiesWidget::VolumeImagePropertiesWidget(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxMitkTransferFunctionWidget(parent, id, pos, size, style, name)
{
	this->SetAutoLayout(true);
}

//!
VolumeImagePropertiesWidget::~VolumeImagePropertiesWidget(void)
{
}


/** When the input changes, update the widget with the new data */
void VolumeImagePropertiesWidget::OnInputHolderChangeSubject(void)
{
	coreAssertMacro(this->m_InputHolder.IsNotNull() && "The holder may not be null! Have you initialized this widget and its holder properly?");
	Core::DataEntity::Pointer dataEntity = this->m_InputHolder->GetSubject();

	if( dataEntity.IsNotNull() && 
		dataEntity->IsImage() &&
		GetMultiRenderWindow() && 
		GetMultiRenderWindow()->GetRenderingTree().IsNotNull() )
	{
		DisableVolumeRendering( dataEntity->IsROI( ) );

		boost::any anyData = GetMultiRenderWindow()->GetRenderingTree()->GetNode( dataEntity );
		mitk::DataTreeNode::Pointer node;
		Core::CastAnyProcessingData( anyData, node );
		this->SetDataTreeNode(node.GetPointer());
	}
	else
		this->SetDataTreeNode(NULL);
}

void Core::Widgets::VolumeImagePropertiesWidget::SetMultiRenderWindow( 
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
