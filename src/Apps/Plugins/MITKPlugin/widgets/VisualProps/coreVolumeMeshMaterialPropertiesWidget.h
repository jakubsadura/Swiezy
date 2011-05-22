/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreVolumeMeshMaterialPropertiesWidget_H
#define coreVolumeMeshMaterialPropertiesWidget_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include "coreDataHolder.h"
#include <wxMitkMaterialEditorWidget.h>
#include <mitkDataTreeNode.h>
#include "coreVisualProperties.h"

namespace Core
{
namespace Widgets 
{ 
/** 

\ingroup gmWidgets
\author Chiara Riccobene	
\date 02 December 2008
*/
class PLUGIN_EXPORT VolumeMeshMaterialPropertiesWidget 
	: public mitk::wxMitkMaterialEditorWidget, 
	public VisualPropertiesBase
{
public:
	//!
	coreDefineBaseWindowFactory( VolumeMeshMaterialPropertiesWidget );

	coreClassNameMacro(VolumeMeshMaterialPropertiesWidget);

	VolumeMeshMaterialPropertiesWidget(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxBORDER_NONE, 
		const wxString& name = wxT("VolumeMeshMaterialPropertiesWidget"));
	virtual ~VolumeMeshMaterialPropertiesWidget(void);

	void SetInputHolder(Core::DataEntityHolder::Pointer _aInputHolder)
	{
		this->m_InputHolder = _aInputHolder;
		this->m_InputHolder->AddObserver(
			this, 
			&VolumeMeshMaterialPropertiesWidget::OnInputHolderChangeSubject, 
			Core::DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT);
	};
	DataEntityHolder::Pointer GetInputHolder(void) const 
	{ 
		return this->m_InputHolder; 
	};
	//!
	bool IsValidData( Core::DataEntity::Pointer dataEntity )
	{
		return dataEntity->IsVolumeMesh();
	}
	//!
	void SetMultiRenderWindow(Core::Widgets::RenderWindowBase* val);

private:
	void OnInputHolderChangeSubject(void);
	DataEntityHolder::Pointer m_InputHolder;

};

}
}


#endif // VolumeMeshMaterialPropertiesWidget_H
