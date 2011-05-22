/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreBaseProcessor_H
#define _coreBaseProcessor_H

// CoreLib
#include "gmKernelWin32Header.h"
#include "coreObject.h"
#include "coreDataHolder.h"
#include "coreDataEntity.h"
#include "coreProgressTicket.h"
#include "coreBaseFilter.h"
#include "coreEnvironment.h"

namespace Core{

/**
\brief Generic BaseProcessor with UpdateOutput( ) functions for different types
of data

\ingroup gmKernel
\author Xavi Planes
\date 10 09 09
*/
class GMKERNEL_EXPORT BaseProcessor : public Core::BaseFilter
{
public:
	//!
	coreDeclareSmartPointerTypesMacro(BaseProcessor, Core::BaseFilter)
	coreClassNameMacro(BaseProcessor)

	/**
	\brief Update the subject of m_OutputDataEntityHolderVector[ iIndex ]
	with polyData at time step GetTimeStep( ). 
	
	Perform a DeepCopy() of polyData

	\note Always notify observers

	\param [in] iIndex
	\param [in] data Data to set
	\param [in] strDataEntityName Name of the data entity
	\param [in] bReuseOutput If this is true, the DataEntity of 
		m_OutputDataEntityHolderVector[ iIndex ] will be reused else
		a new DataEntity will be created
	\param [in] iTotalTimeSteps Total time steps of the output data
	*/
	void UpdateOutput( 
		int iIndex, 
		boost::any data,
		std::string strDataEntityName,
		bool bReuseOutput = false,
		int iTotalTimeSteps = 1,
		Core::DataEntity::Pointer fatherDataEntity = NULL );

	//! Use a vector of data to update the output
	template<class T>
	void UpdateOutput( 
		int iIndex, 
		std::vector<T> imageDataVector,
		std::string strDataEntityName,
		bool bReuseOutput = false,
		Core::DataEntity::Pointer fatherDataEntity = NULL );

	//!
	void SetState( 
		Core::Runtime::APP_STATE val,
		Core::ProgressTicketHolderType::Pointer progressTicket = NULL );

protected:
	//!
	BaseProcessor(void);

	//!
	virtual ~BaseProcessor(void);

	//!
	bool CheckBuildOutput( 
		bool bReuseOutput,
		Core::DataEntity::Pointer fatherDataEntity,
		Core::DataEntity::Pointer dataEntityOutput,
		unsigned totalTimeSteps );

private:
	//!
	BaseProcessor( const Self& );

	//!
	void operator=(const Self&);


protected:
};

} // namespace Core{


/**
\brief Create standard smart pointer and processor factory
\ingroup gmKernel
\author Xavi Planes
\date 10 May 2010
*/
#define coreProcessor(className,SuperClassName) \
	coreDeclareSmartPointerClassMacro(className,SuperClassName)\
	coreDefineProcessorFactory(className) 


#include "coreBaseProcessor.txx"

#endif // _coreBaseProcessor_H


