/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreProgressTicket_H
#define coreProgressTicket_H

#include "gmDataHandlingWin32Header.h"
#include "coreObject.h"
#include "coreDataHolder.h"

namespace Core{

/** 
\brief Progress ticket

\author Xavi Planes
\date 05 Dec 2008
\ingroup gmDataHandling
*/
class GMDATAHANDLING_EXPORT ProgressTicket : public Core::SmartPointerObject
{
public:
	//!
	coreDeclareSmartPointerClassMacro(ProgressTicket, Core::SmartPointerObject);

	float GetProgress() const;
	void SetProgress(float val);

	std::string GetStatus() const;
	void SetStatus( const char* status );

	bool GetAbortProcessing() const;
	void SetAbortProcessing(bool val);

	std::string GetProcessorName() const;
	void SetProcessorName(std::string val);

protected:
	//!
	ProgressTicket( );

	//!
	virtual ~ProgressTicket();

private:
	coreDeclareNoCopyConstructors(ProgressTicket);

private:

	//! Progress
	float m_fProgress;

	//! Status message
	std::string m_strStatus;

	//! Abort processing
	bool m_bAbortProcessing;

	//! Processor name
	std::string m_strProcessorName;
};

typedef Core::DataHolder<ProgressTicket::Pointer> ProgressTicketHolderType;


} //namespace Core

#endif // coreProgressTicket_H

