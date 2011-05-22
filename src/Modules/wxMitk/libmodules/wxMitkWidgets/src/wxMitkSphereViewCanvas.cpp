/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "wxMitkSphereViewCanvas.h"
#include "vtkSphereSource.h"
#include "mitkProperties.h"
#include "mitkMaterialProperty.h"
#include "vtkObject.h"
#include "vtkRenderer.h"
#include "mitkDataTree.h"
#include "wx/event.h"
#include "mitkEllipsoid.h"
#include <mitkVtkPropRenderer.h>
#include "mitkStandaloneDataStorage.h"

#ifdef __WXUNIVERSAL__
#include "wx/univ/renderer.h"
#endif

struct mitk::wxMitkSphereViewCanvas::Pimpl
{
	Pimpl(wxMitkSelectableGLWidget* _glWidget)
	{
		glWidget = _glWidget;
		node = mitk::DataTreeNode::New();

		vtkSphereSource* sphereSource = vtkSphereSource::New();
		sphereSource->SetRadius(1.0);
		sphereSource->SetThetaResolution(24);
		sphereSource->SetPhiResolution(24);
		sphereSource->Update();
		vtkPolyData* spherePd = sphereSource->GetOutput();
		this->surface = mitk::Surface::New();
		this->surface->SetVtkPolyData(spherePd);
		this->node->SetData(this->surface);
		sphereSource->Delete( );

		dataStorage = mitk::StandaloneDataStorage::New();
		dataStorage->Add( this->node );
		glWidget->GetRenderer( )->SetDataStorage( dataStorage );
		glWidget->GetRenderer( )->SetMapperID(mitk::BaseRenderer::Standard3D);

		this->material = mitk::MaterialProperty::New();
		this->material->SetDataTreeNode(node);
		this->node->SetProperty("material", this->material);
	}

	MaterialProperty::Pointer GetMaterial() const
	{
		return this->material;
	}

	void SetMaterial(MaterialProperty::Pointer val)
	{
		this->material = val;
		this->material->SetDataTreeNode(node);
		this->node->ReplaceProperty("material", this->material);
	}

	void Render(void)
	{
		mitk::RenderingManager::GetInstance()->ForceImmediateUpdate(this->glWidget->GetVtkRenderWindow());
	}

	//! Renders this->tree
	wxMitkSelectableGLWidget* glWidget;
	//! The sphere
	mitk::Surface::Pointer surface;
	//! The data tree node containing this->sphere
	mitk::DataTreeNode::Pointer node;
	//! The material of this->node.
	MaterialProperty::Pointer material;
	//!
	mitk::DataStorage::Pointer dataStorage;
};

void mitk::wxMitkSphereViewCanvas::Render(void)
{
	pimpl->Render();
}

mitk::wxMitkSphereViewCanvas::wxMitkSphereViewCanvas( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxMitkSelectableGLWidget(parent, id, name, pos, size, style)
, pimpl(new Pimpl(this))
{

}

mitk::wxMitkSphereViewCanvas::~wxMitkSphereViewCanvas()
{

}

mitk::MaterialProperty::Pointer mitk::wxMitkSphereViewCanvas::GetMaterial() const
{
	return pimpl->GetMaterial();
}

void mitk::wxMitkSphereViewCanvas::SetMaterial( mitk::MaterialProperty::Pointer val )
{
	pimpl->SetMaterial(val);
	Render( );
}
