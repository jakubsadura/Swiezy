/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreBaseFilterOutputPort_H
#define _coreBaseFilterOutputPort_H

// CoreLib
#include "coreObject.h"
#include "coreBaseFilterPort.h"

namespace Core{

/**
\brief Output port for a filter

\ingroup gmDataHandling
\author Xavi Planes
\date 06 11 09
*/
class GMDATAHANDLING_EXPORT BaseFilterOutputPort : public Core::BaseFilterPort{
public:
	coreDeclareSmartPointerClassMacro(BaseFilterOutputPort, Core::BaseFilterPort);

	//!
	bool GetReuseOutput() const;
	void SetReuseOutput(bool val);

	//!
	std::string GetDataEntityName() const;
	void SetDataEntityName(const std::string &val);

	//!
	int GetTotalTimeSteps() const;
	void SetTotalTimeSteps(int val);

	/**
	*/
	void UpdateOutput( 
		boost::any any, 
		int timeStepNumber,
		Core::DataEntity::Pointer fatherDataEntity );

private:
	//!
	BaseFilterOutputPort( );

	//!
	~BaseFilterOutputPort( );

	//! Redefined
	bool CheckDataEntityRestrictions( Core::DataEntity::Pointer val );

private:
	//! Use the same output data entity or build a new one
	bool m_ReuseOutput;
	//!
	std::string m_DataEntityName;
	//!
	int m_TotalTimeSteps;
};

} // namespace Core{

#endif // _coreBaseFilterOutputPort_H


