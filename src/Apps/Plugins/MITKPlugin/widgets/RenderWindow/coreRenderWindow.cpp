// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "coreRenderWindow.h"

#include "mitkVtkLayerController.h"
#include "mitkRenderingManager.h"
#include <mitkTransferFunctionProperty.h>
#include "vtkRenderer.h"
#include "wxMitkVTKWindow.h"

using namespace Core::Widgets;

RenderWindow::RenderWindow(
	wxWindow* parent,
	wxWindowID id,
	mitk::BaseRenderer::StandardMapperSlot mapper)
: mitk::wxMitkSelectableGLWidget(parent, id, wxT("RenderWindow"))
, m_renderingTree(Core::RenderingTreeMITK::New())
, m_cornerAnnotation( vtkSmartPointer< vtkCornerAnnotation >::New() )
, m_textRenderer( vtkSmartPointer< vtkRenderer >::New() )
{
	this->GetRenderer()->SetMapperID(mapper);
	
	m_textProperties = vtkTextProperty::New();
	//m_cornerAnnotation->SetPosition(10, 10);
	//m_cornerAnnotation->SetMaximumFontSize (16);

	m_ScalarBar = blMitkScalarBar::New();
}

RenderWindow::~RenderWindow()
{
	m_ScalarBar = NULL;
}


//void RenderWindow::SetDataEntityHolder( 
//Core::DataEntityHolder::Pointer dataEntityHolder 
//)
//{
//	this->m_dataEntityHolder = dataEntityHolder;
//	//this->m_dataEntityHolder->AddObserver(this, &RenderWindow::OnAddedDataEntity);
//	//this->AddDataEntity(m_dataEntityHolder,m_renderProperty);
//}

//void RenderWindow::AddDataEntity(Core::DataEntityHolder::Pointer dataEntityHolder)
//{
//	//RenderingData rd = { dataEntityHolder->GetSubject(), renderProperty};
//	//m_renderingDataVector.push_back(rd);
//	////m_renderingDataMap.insert( std::make_pair(dataEntity, renderProperty) );
//	//SetDataEntityHolder(dataEntityHolder);
//	Update();
//}


void RenderWindow::SetBackColor(double r, double g, double b)
{
//TODO: also change the text color to the maximum contrast?!

	vtkSmartPointer<vtkRenderer> vtkrenderer;
	vtkRenderWindow *renWin;
	renWin= this->GetVtkRenderWindow();
	vtkrenderer = mitk::BaseRenderer::GetInstance(renWin)->GetVtkRenderer();
	vtkrenderer->SetBackground(r,g,b);

	////change the color of the (damned) m_textRender current rendering window
	m_textRenderer->SetBackground(r,g,b);
}


void RenderWindow::Fit()
{
	this->GetRenderer()->SetDataStorage( m_renderingTree->GetDataStorage() );
	this->GetRenderer()->ForceImmediateUpdate();
}

void RenderWindow::SetCornerAnnotation( std::string annotation )
{
	m_cornerAnnotation->ClearAllTexts();
	m_textRenderer->Clear();
	m_textRenderer->AddActor2D(m_cornerAnnotation);
	m_cornerAnnotation->SetText(2, annotation.c_str()); //2 = left upper corner id
	
	mitk::VtkLayerController::GetInstance(this->GetVtkRenderWindow())->InsertBackgroundRenderer(m_textRenderer, true);
	mitk::RenderingManager::GetInstance( )->RequestUpdate( this->GetVtkRenderWindow());
}

//void RenderWindow::Update()
//{
	//for(RenderingDataMapType::const_iterator it = data.begin(); it != m_renderingDataMap.end(); ++it)
	//for(int i=0; i<m_renderingDataVector.size(); i++)
	//{
	//	//mitk::DataTreeNode::Pointer node = this->m_renderingTree->Add(m_renderingDataVector.at(i).m_dataEntity);
	//	mitk::DataTreeNode::Pointer node = this->m_renderingTree->GetNode(m_renderingDataVector.at(i).m_dataEntity);
	//	if(node.IsNull())
	//		node = this->m_renderingTree->Add(m_renderingDataVector.at(i).m_dataEntity);

		//if( node.IsNotNull() )
		//{
		//	switch(m_renderingDataVector.at(i).m_renderProperty)
		//	{
		//	case VOLUME_RENDERING:
		//		//node->SetProperty( "volumerendering", new mitk::BoolProperty(true) );
		//		break;
		//	case XRAY_ANGIOGRAPHY:
		//		//mitk::RenderingManager::GetInstance( )->SetCurrentLOD(0);
		//		//mitk::RenderingManager::GetInstance( )->AbortRendering(this->GetVtkRenderWindow());
		//		//// change to virtual xray rendering after it will available in mitk
		//		//// Enable xray volume rendering property
		//		//mitk::BoolProperty* xRayVolumeRenderingProp;
		//		//xRayVolumeRenderingProp = static_cast<mitk::BoolProperty*>(node->GetProperty("xrayvolumerendering"));
		//		//if(xRayVolumeRenderingProp==NULL)
		//		//{
		//		//	xRayVolumeRenderingProp = new mitk::BoolProperty(true);
		//		//	node->SetProperty("xrayvolumerendering", xRayVolumeRenderingProp);
		//		//}

		//		//// Check the xray volume rendering checkbox
		//		//xRayVolumeRenderingProp->SetValue(true);

		//		//node->SetProperty( "volumerendering", new mitk::BoolProperty(true) );
		//		//node->SetProperty( "xrayvolumerendering" ,  new::mitk::BoolProperty(true) );
		//		break;
		//	
		//	case STANDARD:
		//	default:
		//		break;
		//	}
		//}
	//}
	//this->Fit();
//}

Core::RenderingTree::Pointer RenderWindow::GetRenderingTree()
{
	return m_renderingTree.GetPointer();
}

void RenderWindow::SetTextProperties(vtkTextProperty* textProperties)
{
	if(textProperties!=NULL)
	{
		m_textProperties = textProperties;
		m_cornerAnnotation->SetTextProperty(m_textProperties);
	}
}

void RenderWindow::SetTextColor(double r, double g, double b)
{
	m_textProperties->SetColor(r,g,b);
	m_cornerAnnotation->SetTextProperty(m_textProperties);
}

void RenderWindow::EnableColorLegend(bool enable, mitk::DataTreeNode* node /*= NULL */ ) {
	m_ScalarBar->SetRenderWindow( this->GetVtkRenderWindow() );
	m_ScalarBar->SetDataTreeNode( node );
	m_ScalarBar->Enable( enable );
}
