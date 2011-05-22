/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreITKTransformVisualPropertiesWidget.h"
#include "coreDataEntity.h"
#include "coreAssert.h"
#include "coreRenderingTreeMITK.h"
#include "coreMultiRenderWindowMITK.h"

using namespace Core::Widgets;
using namespace mitk;

//!
ITKTransformVisualPropertiesWidget::ITKTransformVisualPropertiesWidget(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxMitkTransformVisualPropWidget(parent, id, pos, size, style, name)
{
	this->SetAutoLayout(true);
}

//!
ITKTransformVisualPropertiesWidget::~ITKTransformVisualPropertiesWidget(void)
{
}


/** When the input changes, update the widget with the new data */
void ITKTransformVisualPropertiesWidget::OnInputHolderChangeSubject(void)
{
	coreAssertMacro(this->m_InputHolder.IsNotNull() && "The holder may not be null! Have you initialized this widget and its holder properly?");
	Core::DataEntity::Pointer dataEntity = this->m_InputHolder->GetSubject();

	if ( dataEntity.IsNotNull() && dataEntity->IsTransform() &&
		GetMultiRenderWindow() && 
		GetMultiRenderWindow()->GetRenderingTree().IsNotNull() )
	{
		boost::any anyData = GetMultiRenderWindow()->GetRenderingTree()->GetNode( dataEntity );
		mitk::DataTreeNode::Pointer node;
		Core::CastAnyProcessingData( anyData, node );

		this->SetDataTreeNode(node);
	}
	else
	{
		this->SetDataTreeNode(NULL);
	}

}





void Core::Widgets::ITKTransformVisualPropertiesWidget::SetMultiRenderWindow( 
	Core::Widgets::RenderWindowBase* val )
{
	Core::BaseWindow::SetMultiRenderWindow( val );
}
