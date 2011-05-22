/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreBaseFilterPort_H
#define _coreBaseFilterPort_H

// CoreLib
#include "coreObject.h"
#include "coreDataEntityHolder.h"

namespace Core{

/**
\brief Base port for a filter

\ingroup gmDataHandling
\author Xavi Planes
\date 05 06 09
*/
class GMDATAHANDLING_EXPORT BaseFilterPort : public Core::SmartPointerObject{

public:
	coreDeclareSmartPointerTypesMacro(BaseFilterPort,Core::SmartPointerObject) \
	coreClassNameMacro(BaseFilterPort)

	//!
	Core::DataEntityHolder::Pointer GetDataEntityHolder() const;

	/** If CheckDataEntityRestrictions( ) return true, set subject 
	of m_DataEntityHolder to val
	*/
	bool SetDataEntity(Core::DataEntity::Pointer val);

	//!
	Core::DataEntity::Pointer GetDataEntity( );

	//!
	Core::DataEntityType GetDataEntityType() const;
	virtual void SetDataEntityType(int val);

	//!
	std::string GetName() const;
	void SetName(std::string val);

	//!
	std::string GetProcessorName() const;
	void SetProcessorName(std::string val);

	//!
	bool GetOptional() const;
	void SetOptional(bool val);

	//!
	bool GetActive() const;
	void SetActive(bool val);

protected:
	//!
	BaseFilterPort( );

	//!
	virtual ~BaseFilterPort( );

	//! Check data entity restrictions
	virtual bool CheckDataEntityRestrictions( Core::DataEntity::Pointer val ) = 0;

private:
	//!
	Core::DataEntityHolder::Pointer m_DataEntityHolder;
	//!
	std::string m_Name;
	//!
	std::string m_ProcessorName;
	//!
	Core::DataEntityType m_DataEntityType;
	//! This port is optional. The client can use it or not
	bool m_Optional;
	//! If it's optional, this flag tells if is active or not
	bool m_Active;
};

} // namespace Core{

#endif // _coreBaseFilterPort_H


