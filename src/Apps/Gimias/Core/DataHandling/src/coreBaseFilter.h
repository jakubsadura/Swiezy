/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef CoreBaseFilter_H
#define CoreBaseFilter_H

// CoreLib
#include "coreObject.h"
#include "coreDataHolder.h"
#include "coreDataEntity.h"
#include "coreProgressTicket.h"
#include "coreBaseFilterInputPort.h"
#include "coreBaseFilterOutputPort.h"

namespace Core{


/**
\brief Generic Filter

It has a vector of DataEntityHolder for storing the selected processor input
of the data entity list and a vector of DataEntityHolder for output.

BaseFilter uses input and output ports to communicate with the rest of the 
framework. Each port has a holder that will send notifications each time 
the DataEntity is modified. The constructor of the concrete filter should 
initialize the number of input and output ports, and also the type of 
inputs is required.

You should not create a subclass of BaseFilter. Is better to use the 
subclass BaseProcessor.

\section Examples Examples

Initialize the filter at constructor:

\code
ExtractScalarProcessor::ExtractScalarProcessor( )
{
	SetName( "ExtractScalar" );

	SetNumberOfInputs( 2 );
	GetInputPort( 0 )->SetName( "Input mesh" );
	GetInputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetInputPort( 1 )->SetName( "Input point" );
	GetInputPort( 1 )->SetDataEntityType( Core::PointSetTypeId );

	SetNumberOfOutputs( 1 );
	GetOutputPort( 0 )->SetDataEntityType( Core::SignalTypeId );
	GetOutputPort( 0 )->SetReuseOutput( true );
	GetOutputPort( 0 )->SetName( "Output signal" );
	GetOutputPort( 0 )->SetDataEntityName( "Scalar" );
}

\endcode

To retrieve the processing data:

\code
void ExtractScalarProcessor::Update()
{
	vtkPolyDataPtr surfaceMesh;
	GetProcessingData( 0, surfaceMesh );
}
\endcode

\ingroup gmDataHandling
\author Xavi Planes
\date 05 06 09
*/
class GMDATAHANDLING_EXPORT BaseFilter : public Core::SmartPointerObject
{
public:
	//!
	coreDeclareSmartPointerTypesMacro(BaseFilter,Core::SmartPointerObject)
	coreClassNameMacro(BaseFilter)

	//!
	virtual void Update( );

	//! Processor name
	std::string GetName() const;

	//! Processor name
	void SetName(std::string val);

	//! Resize number of inputs with new DataEntityHolder and NULL Subject
	void SetNumberOfInputs( unsigned iNum );

	//! Resize number of outputs with new DataEntityHolder and NULL Subject
	void SetNumberOfOutputs( unsigned iNum );

	//! 
	unsigned GetNumberOfInputs( );

	//! 
	unsigned GetNumberOfOutputs( );

	//!
	Core::DataEntity::Pointer GetInputDataEntity( int iIndex );

	//!
	void SetInputDataEntity( int iIndex, Core::DataEntity::Pointer dataEntity );

	//!
	Core::DataEntity::Pointer GetOutputDataEntity( int iIndex = 0 );

	//!
	void SetOutputDataEntity( int iIndex, Core::DataEntity::Pointer dataEntity );

	//! Add a new data entity at the end and increases the number of outputs
	void AddOutputDataEntity( Core::DataEntity::Pointer dataEntity );

	//!
	Core::DataEntityHolder::Pointer GetInputDataEntityHolder( int iIndex );

	//!
	Core::DataEntityHolder::Pointer GetOutputDataEntityHolder( int iIndex );

	//! Return the name of this input as "Input image"
	std::string GetInputDataName( int iIndex );

	//! Set the name of this input as "Input image"
	void SetInputDataName( int iIndex, const std::string strName );

	//! Set the name of this output as "Output image"
	void SetOutputDataName( int iIndex, const std::string strName );

	//! Return GetNumberOfTimeSteps( ) of the input data
	unsigned GetInputDataNumberOfTimeSteps( int iIndex );

	//!
	int GetTimeStep() const;

	//!
	void SetTimeStep(int val);

	//!
	ProgressTicketHolderType::Pointer GetProgressTicketHolder() const;

	//!
	ProgressTicket::Pointer GetProgressTicket() const;

	//! Reset progress to 0, abortProcessing to false and optional message
	void ResetProgress( const char* processorName, const char* strMessage = NULL );

	//! Update the contents and notify observers
	void SetProgress( float fProgress, const char* strMessage = NULL );

	//! Abort processing
	virtual void Abort( );

	//!
	Core::BaseFilterInputPort::Pointer GetInputPort( int num );

	//!
	Core::BaseFilterOutputPort::Pointer GetOutputPort( int num );

	//! Get input data of a data entity and throw exception if error
	template <class ProcessingDataType>
	void GetProcessingData( 
		int num,
		ProcessingDataType &processingData,
		int iTimeStep = 0 );

	//! Get input data of a data entity and throw exception if error
	template <class ProcessingDataType>
	void GetProcessingData( 
		int num,
		std::vector< ProcessingDataType> &processingDataVector );

protected:
	//!
	BaseFilter(void);

	//!
	virtual ~BaseFilter(void);

private:
	//!
	BaseFilter( const Self& );

	//!
	void operator=(const Self&);


protected:
	//! Input data entity for this processor
	std::vector<BaseFilterInputPort::Pointer> m_InputPortVector;

	//! Output of this processor
	std::vector<BaseFilterOutputPort::Pointer> m_OutputPortVector;

	//! Process only this time step
	int m_iTimeStep;

	//!
	ProgressTicketHolderType::Pointer m_ProgressTicketHolder;

	//! Filter name
	std::string m_Name;
};

} // namespace Core{

#include "coreBaseFilter.txx"

#endif // ProcessorWorkingData_H


