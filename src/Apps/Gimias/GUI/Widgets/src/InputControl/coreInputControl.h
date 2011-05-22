/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreAcquireDataEntityInputControl_H
#define coreAcquireDataEntityInputControl_H

#include "gmWidgetsWin32Header.h"
#include "coreBaseInputControl.h"
#include "coreDataEntity.h"

#include <vector>
#include <string>
#include <map>

namespace Core{ namespace Widgets{ class DataEntityListBrowser; } }

namespace Core
{
namespace Widgets 
{ 
/** 
Specialization of the AcquireInputControl that accepts DataEntities and 
its holders, so a plugin developer can use this control to catch the 
current selected data entity of the data entity list, and hold it as an input
for any process he may want (like running an API procedure or any algorithm).

You may give a DataHolder<Core::DataEntity::Pointer> to this class using 
SetAcquiredInputDataHolder(), and it will use and change that holder 
whenever the user tells it to change the input.

If you set bAutomaticSelection to true, the selected data entity form the
DataList will be selected automatically, else, a button will appear
to allow the user to set it as selected.

\sa AcquireInputControl, DataEntity
\ingroup gmWidgets
\author Juan Antonio Moya
\date 06 Feb 2008
*/
class GMWIDGETS_EXPORT InputControl 
	: public BaseInputControl
{
public:
	coreClassNameMacro(InputControl);

	typedef Core::DataEntityHolder DataHolderType;

	InputControl(
		wxWindow* parent, 
		wxWindowID id, 
		bool bAutomaticSelection = false,
		bool bSetDefaultDataEntity = true,
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxBORDER_NONE, 
		const wxString& name = wxT("AcquireDataEntityInputControl"));

	virtual ~InputControl(void);

	/**
	\brief Set input data holder to change the subject when the selected
	data holder is modified.
	\param [in] val Data holder to modify
	\param [in] type Type of the input DataEntity. If the input is not
		of this type, the subject will not be changed
	*/
	void SetAcquiredInputDataHolder( 
		DataHolderType::Pointer val,
		int type = Core::UnknownTypeId,
		Core::ModalityType modality = Core::UnknownModality);

	//! Allowed input data types
	Core::DataEntityType GetAllowedInputDataType() const;
	void SetAllowedInputDataTypes(int val);

	//! 
	bool SetAcquiredInputData( Core::DataEntity::Pointer &subject );

protected:

	//! Checks if the subject is Ok
	bool CheckSubjectIsOk( Core::DataEntity::Pointer subject );

	//! Check if modality is ok to be selected or not
	bool CheckModalityIsOk( Core::DataEntity::Pointer &subject );

	//! 
	bool CheckFatherIsOk( const Core::DataEntity::Pointer &subject );

	/** Check the input data type with m_AllowedInputDataTypes
	Also check for any children and return the output in outputSubject
	if any found
	*/
	bool CheckAllowedInputDataTypes( 
		Core::DataEntity::Pointer &subject );


protected:

	//! Set type of the dataEntity to be selected
	Core::DataEntityType m_AllowedInputDataTypes;

	//!
	Core::ModalityType m_Modality;
};

}
}


#endif // AcquireDataEntityInputControl_H
