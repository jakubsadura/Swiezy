/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef WXMITKSPHEREVIEWCANVAS_H
#define WXMITKSPHEREVIEWCANVAS_H

#include "CILabNamespaceMacros.h"
#include "boost/shared_ptr.hpp"
#include "wxMitkSelectableGLWidget.h"
#include <mitkMaterialProperty.h>
#include "wxMitkWidgetsWin32Header.h"

CILAB_BEGIN_NAMESPACE(mitk)

/**
This class displays a sphere in a canvas. The sphere is used to visualize 
material properties.

\ingroup wxMitkWidgets
\author Maarten Nieber
\date 12 Dec 2007
*/

class WXMITKWIDGETS_EXPORT wxMitkSphereViewCanvas 
	: public wxMitkSelectableGLWidget // public wxWindow
{
public:
	//!
	wxMitkSphereViewCanvas(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = 0, 
		const wxString& name = wxT("wxMitkSphereViewCanvas"));
	//!
	~wxMitkSphereViewCanvas();
	//! Get material that is displayed.
	MaterialProperty::Pointer GetMaterial() const;
	/**
	Get material that is displayed.
	*/
	void SetMaterial(MaterialProperty::Pointer val);
	//! Calls an Update mechanism that renders the sphere with the current material
	void Render(void);

private:
	//@{ 
	//! \name Private implementation pattern.
	struct Pimpl;
	boost::shared_ptr<Pimpl> pimpl;
	//@}
};

CILAB_END_NAMESPACE(mitk)

#endif //WXMITKSPHEREVIEWCANVAS_H
