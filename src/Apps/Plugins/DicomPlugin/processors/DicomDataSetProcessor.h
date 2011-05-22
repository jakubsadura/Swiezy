/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _DicomDataSetProcessor_H
#define _DicomDataSetProcessor_H

#include "coreBaseProcessor.h"
#include "dcmDataSet.h"
#include "dcmTypes.h"

namespace DicomPlugin{

typedef Core::DataHolder<dcmAPI::DataSet::Pointer> DataSetHolderType;

enum INPUT_PATH_TYPE{
	INPUT_PATH_DICOMDIR,
	INPUT_PATH_DIRECTORY,
	INPUT_PATH_DCMFILE
};

/**
Processor for Reading dcmAPI::DataSets

\ingroup DicomPlugin
\author Xavi Planes
\date 24 july 2009
*/
class DataSetProcessor : public Core::BaseProcessor
{
public:
	//!
	coreDeclareSmartPointerClassMacro(DataSetProcessor, Core::SmartPointerObject);

	//! Call library to perform operation
	void SetPath(const std::string& path);
	std::string GetPath() const;

	//!
	DataSetHolderType::Pointer GetDataSetHolder() const;

	//!
	dcmAPI::DataSet::Pointer GetDataSet() const;

	//!
	void Update( );

	//!
	INPUT_PATH_TYPE GetInputPathType() const;

	//! Check that a tag is present
	bool SearchTag( dcmAPI::TagId tag );

	//! Set the tag used as time identifier.
	void SetTimeTag(const dcmAPI::TagId& timeTag);

	//!
	void EnableObservers( bool enable );

private:
	//!
	DataSetProcessor();

	//!
	~DataSetProcessor();

	//! Purposely not implemented
	DataSetProcessor( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );

	//!
	void InternalUpdate( );

	//!
	bool SearchTag( dcmAPI::TagId tag, dcmAPI::TagsMap *map );

private:
	
	//! Contains structure of the dicom data (from patient to slice)
	DataSetHolderType::Pointer m_DataSetHolder;

	//!
	std::string m_Path;

	//!
	INPUT_PATH_TYPE m_InputPathType;

	//!
	dcmAPI::TagId m_TimeTag;

	//!
	bool m_EnableObservers;
};
    
} // namespace DicomPlugin

#endif //_DicomDataSetProcessor_H
