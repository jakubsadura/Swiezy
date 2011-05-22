/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreException.h"


/**
*/
template <class ProcessingDataType>
void Core::BaseFilter::GetProcessingData( 
	int num,
	ProcessingDataType &processingData,
	int iTimeStep /*= 0*/ )
{
	DataEntity::Pointer	dataEntity;

	dataEntity = this->GetInputDataEntity( num );
	if ( dataEntity.IsNull( ) )
	{
		throw Core::Exceptions::Exception("DataEntityHelper::GetProcessingData", 
			"You must select an input data from the Processing Browser and set it as input" 
			);
	}

	dataEntity->GetProcessingData( processingData, iTimeStep, true );
	if( !processingData )
	{
		throw Core::Exceptions::Exception("DataEntityHelper::GetProcessingData", 
			"You must select an input data from the Processing Browser and set it as input" 
			);
	}
}




/**
*/
template <class ProcessingDataType>
void Core::BaseFilter::GetProcessingData( 
	int num,
	std::vector< ProcessingDataType> &processingDataVector )
{
	Core::DataEntity::Pointer	dataEntity;

	dataEntity = this->GetInputDataEntity( num );
	if ( dataEntity.IsNull( ) )
	{
		throw Core::Exceptions::Exception("DataEntityHelper::GetProcessingData", 
			"You must select an input data from the Processing Browser and set it as input" 
			);
	}

	for ( int iTimeStep = 0 ; iTimeStep < dataEntity->GetNumberOfTimeSteps( ) ; iTimeStep++ )
	{
		ProcessingDataType processingData = NULL;
		bool ok = dataEntity->GetProcessingData( processingData, iTimeStep );
		if( !processingData || !ok )
		{
			throw Core::Exceptions::Exception("DataEntityHelper::GetProcessingData", 
				"You must select an input data from the Processing Browser and set it as input" 
				);
		}

		processingDataVector.push_back( processingData );
	}
}
