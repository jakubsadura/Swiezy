/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreBaseFilterInputPort_H
#define _coreBaseFilterInputPort_H

// CoreLib
#include "coreObject.h"
#include "coreBaseFilterPort.h"

namespace Core{

/**
\brief Input port for a filter

Set restrictions for input data based on:
- Type
- Metadata tags

\section Examples

To allow all kind of images:
SetDataEntityType( Core::ImageTypeId )

To allow only MaskImages:
SetDataEntityType( Core::ROITypeId )

To allow all images except MaskImages:
SetDataEntityType( Core::ImageTypeId )
SetNotValidDataEntityType( Core::ROITypeId )

To allow only MRI images:
SetModality( Core::MRI )

To allow only images with Male sex:
AddValidDataEntityTag( blTag::New( "Patient sex", "Male" ) )

\ingroup gmDataHandling
\author Xavi Planes
\date 06 11 09
*/
class GMDATAHANDLING_EXPORT BaseFilterInputPort : public Core::BaseFilterPort{
public:
	coreDeclareSmartPointerClassMacro(BaseFilterInputPort, Core::BaseFilterPort);

	//!
	void SetDataEntityType(int val);

	//!
	Core::ModalityType GetModality() const;
	void SetModality(Core::ModalityType val);

	//! Redefined
	bool CheckDataEntityRestrictions( Core::DataEntity::Pointer val );

	//! Set and get not valid data entity types
	Core::DataEntityType GetNotValidDataEntityType() const;
	void SetNotValidDataEntityType(int val);

	//!
	void AddValidDataEntityTag( DataEntityTag::Pointer tag );

private:
	//!
	BaseFilterInputPort( );

	//!
	~BaseFilterInputPort( );

	//!
	bool CheckAllowedInputDataTypes( Core::DataEntity::Pointer val );

private:
	//!
	Core::ModalityType m_Modality;
	//! Not valid data Entity types
	Core::DataEntityType m_NotValidDataEntityType;
	//!
	std::list<DataEntityTag::Pointer> m_ValidTagList;
};

} // namespace Core{

#endif // _coreBaseFilterInputPort_H


