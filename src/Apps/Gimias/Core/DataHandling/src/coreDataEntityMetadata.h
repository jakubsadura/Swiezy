/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreDataEntityMetadata_H
#define _coreDataEntityMetadata_H

#include "gmDataHandlingWin32Header.h"
#include "coreObject.h"
#include "blTagMap.h"

namespace Core{

//! Enumeration field that holds the different image modalities 
enum ModalityType { UnknownModality = 0, US, MRI, CT, NM, XA, THREE_D_RA };

//! Default tags
enum DE_TAG_TYPE{
	DE_TAG_UNKNOWN,
	DE_TAG_MODALITY,
	DE_TAG_NAME,
	DE_TAG_PATIENT_NAME,
	DE_TAG_PATIENT_SEX,
	DE_TAG_PATIENT_BIRTH_DATE,
	DE_TAG_ED,
	DE_TAG_ES
};

typedef blTag DataEntityTag;

/** 
\brief Information about a DataEntity like Modality, patient name, ...

Is prefered to use the name of the tag instead of the tag ID:
- "Modality": Core::ModalityType
- "Name": std::string
- "Patient name": std::string
- "Patient sex": std::string
- "Patient birth date": std::string
- "ED flag": std::string
- "ES flag": std::string

\author Xavi Planes
\date 28 July 2009
\ingroup gmDataHandling
*/
class GMDATAHANDLING_EXPORT DataEntityMetadata : public blTagMap
{
public:
	coreDeclareSmartPointerClassMacro1Param(
		DataEntityMetadata, 
		Core::SmartPointerObject,
		unsigned int );

public:
	//! DE_TAG_NAME
	std::string GetName();
	
	//! DE_TAG_NAME
	void SetName(const std::string& name);

	//! DE_TAG_MODALITY
	ModalityType GetModality();

	//! DE_TAG_MODALITY
	void SetModality(ModalityType modality);

	//! ID of data entity
	unsigned int GetDataEntityId() const;

	//! DE_TAG_ED
	std::string GetEDTimeStep();

	//! Set the DE_TAG_ED value
	void SetEDTimeStep(size_t edTimeStep); 

	//! DE_TAG_ES
	std::string GetESTimeStep();

	//! Set the DE_TAG_ES value
	void SetESTimeStep(size_t timeStep); 


protected:
	//!
	DataEntityMetadata( unsigned int dataEntityId );

	//!
	virtual ~DataEntityMetadata();

private:

	//!
	unsigned int m_DataEntityId;

};

} // end namespace Core


#endif // _coreDataEntityMetadata_H

