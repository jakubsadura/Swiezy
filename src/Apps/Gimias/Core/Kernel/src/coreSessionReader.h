/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreSessionReader_H
#define coreSessionReader_H


#include "gmKernelWin32Header.h"
#include "coreSession.h"
#include "coreObject.h"
#include "coreDataEntityList.h"


namespace Core 
{
namespace IO 
{

/** 
\brief The SessionReader will read all data into Core::DataList that was saved
in a previous session

\ingroup gmKernel
\author Jakub Lyko
\date 10 Dec 2009
*/
class GMKERNEL_EXPORT SessionReader : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(SessionReader, Core::SmartPointerObject);	
	//! 
	void Update( );

	//!
	std::string GetFilepath() const { return m_filepath; }

	//!
	void SetFilepath(std::string val) { m_filepath = val; }

protected:
	//! 
	SessionReader();

	//! 
	virtual ~SessionReader();

private:
	/**
	\brief The method first creates a DataEntity from gettingn necessary
	information from LightData, and then adds it to core DataList
	*/
	void LoadLightData( LightData* lightData, Core::DataEntity::Pointer father = NULL );

private:
	//!
	std::string m_filepath;

private:
	coreDeclareNoCopyConstructors(SessionReader);
};

} // namespace IO
} // namespace Core

#endif
