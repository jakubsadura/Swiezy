/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreSessionWriter_H
#define coreSessionWriter_H


#include "gmKernelWin32Header.h"
#include "coreSession.h"
#include "coreObject.h"
#include "coreDataEntityList.h"


namespace Core 
{
	class RenderingTreeManager;

namespace IO 
{

/** 
\brief The SessionWriter will save all data in Core::DataList on disc and its
parent-children relation in session xml file 

\ingroup gmKernel
\author Jakub Lyko
\date 10 Dec 2009
*/
class GMKERNEL_EXPORT SessionWriter : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(SessionWriter, Core::SmartPointerObject);	

	//! 
	void Update( );

	//!
	std::string GetFilepath() const { return m_filepath; }

	//!
	void SetFilepath(std::string val) { m_filepath = val; }

	//!
	void SetTreeManager(Core::RenderingTreeManager* val);

protected:
	//! 
	SessionWriter();

	//! 
	virtual ~SessionWriter();

private:
	/**
	\brief Save data entity in default format and returns LightData which 
	will be used for saving session. LightData will be stored in xml and
	will provide information to load DataEntity. 
	*/
	Core::LightData* SaveDataEntity(
		Core::DataEntity::Pointer dataEntity,
		std::string dirPath);

private:
	//!
	std::string m_filepath;

	//!
	Core::RenderingTreeManager* m_TreeManager;

private:
	coreDeclareNoCopyConstructors(SessionWriter);

};

} // namespace IO
} // namespace Core

#endif
