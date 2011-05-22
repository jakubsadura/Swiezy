#ifndef CORERENDERWINDOW_H
#define CORERENDERWINDOW_H

// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "gmWidgetsWin32Header.h"
#include "CILabNamespaceMacros.h"
#include "coreRenderingTreeMITK.h"
#include "coreDataEntityHolder.h"

#include "mitkDataTree.h"
#include "wxMitkSelectableGLWidget.h"
#include "vtkCornerAnnotation.h"
#include "vtkTextProperty.h"

#include "blMitkScalarBar.h"


namespace Core{
namespace Widgets{

/**
This class is used to render data stored in data entity holder

/**
This class is a  
\ingroup gmWidgets 
\author Jakub Lyko
\author Luigi Carotenuto
05 November 2009
*/


class PLUGIN_EXPORT RenderWindow : public mitk::wxMitkSelectableGLWidget
{
public:
	//!
	RenderWindow(
		wxWindow* parent,
		wxWindowID id,
		mitk::BaseRenderer::StandardMapperSlot mapper = mitk::BaseRenderer::Standard3D
		);

	~RenderWindow();

	//! 
	//void SetDataEntityHolder(Core::DataEntityHolder::Pointer dataEntityHolder);

	//!
	//void AddDataEntity(Core::DataEntityHolder::Pointer dataEntityHolder);

	//! Centralize rendered data.
	void Fit();

	//!
	void SetCornerAnnotation(std::string annotation);

	//!
	Core::RenderingTree::Pointer GetRenderingTree();

	//!
	void SetBackColor(double r, double g, double b);

	//! Set the vtkTextProperties
	void SetTextProperties(vtkTextProperty* textProperties);

	//! Just set the text color
	void SetTextColor(double r, double g, double b);

	void EnableColorLegend(bool enable, mitk::DataTreeNode* node);

private:
	////! Start displaying new data
	//void OnNewDataEntity();

	//!
	//void Update();

private:

	//!
	Core::RenderingTreeMITK::Pointer m_renderingTree;

	//! The input mesh holder
	Core::DataEntityHolder::Pointer m_dataEntityHolder;

	//! The current input data entity 
	Core::DataEntity::Pointer m_currentInputDataEntity;

	//!
	vtkSmartPointer < vtkCornerAnnotation > m_cornerAnnotation; 

	//! 
	vtkSmartPointer < vtkRenderer > m_textRenderer;

	//!
	vtkTextProperty* m_textProperties;

	blMitkScalarBar::Pointer m_ScalarBar;
};

} // namespace Widgets
} //namespace Core

#endif //CORERENDERWINDOW_H
