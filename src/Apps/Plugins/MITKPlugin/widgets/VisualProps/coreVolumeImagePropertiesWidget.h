/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreVolumeImagePropertiesWidget_H
#define coreVolumeImagePropertiesWidget_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include "coreDataHolder.h"
#include <wxMitkTransferFunctionWidget.h>
#include <mitkDataTreeNode.h>
#include "coreVisualProperties.h"

namespace Core
{
namespace Widgets 
{ 
/** 
\brief This widget forms part of the Appearance Suitecase.
\ingroup gmWidgets
\author Juan Antonio Moya
\date 07 Feb 2008
\sa Core::Widgets::AppearanceSuitcase and wxMitkTransferFunctionWidget
*/
class PLUGIN_EXPORT VolumeImagePropertiesWidget 
	: public mitk::wxMitkTransferFunctionWidget, public VisualPropertiesBase
{
public:
	//!
	coreDefineBaseWindowFactory( VolumeImagePropertiesWidget );

	coreClassNameMacro(VolumeImagePropertiesWidget);

	VolumeImagePropertiesWidget(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxBORDER_NONE, 
		const wxString& name = wxT("VolumeImagePropertiesWidget"));
	virtual ~VolumeImagePropertiesWidget(void);

	void SetInputHolder(DataEntityHolder::Pointer _aInputHolder)
	{
		this->m_InputHolder = _aInputHolder;
		this->m_InputHolder->AddObserver(
			this, 
			&VolumeImagePropertiesWidget::OnInputHolderChangeSubject, 
			Core::DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT);
	};
	DataEntityHolder::Pointer GetInputHolder(void) const 
	{ 
		return this->m_InputHolder; 
	};
	//!
	bool IsValidData( Core::DataEntity::Pointer dataEntity )
	{
		return dataEntity->IsImage();
	}

	//!
	void SetMultiRenderWindow(Core::Widgets::RenderWindowBase* val);

private:
	void OnInputHolderChangeSubject(void);
	DataEntityHolder::Pointer m_InputHolder;

};

}
}

#endif // VolumeImagePropertiesWidget_H
