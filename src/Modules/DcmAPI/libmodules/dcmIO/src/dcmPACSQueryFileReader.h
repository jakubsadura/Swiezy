/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dcmPACSQueryFileReader_H
#define _dcmPACSQueryFileReader_H

// DCMAPIIO_EXPORT
#include "dcmAPIIOWin32Header.h"

// BaseLib
#include "blLightObject.h"
#include "CILabExceptionMacros.h"

//DcmAPI
#include "dcmDataSet.h"
#include "dcmTypes.h"

namespace dcmAPI{

/** 
* \brief DICOM PACS query file DataSet reader.
* 
* Reads a DICOM PACS query file and create the objects to 
* store the DataSet using the hierarchy defined by dcmAPI classes. 
* 
* \ingroup dset_readers
* \author Xavi Planes
* \date 24 July 2009
*/
class DCMAPIIO_EXPORT PACSQueryFileReader : public blLightObject
{
public:
	typedef PACSQueryFileReader Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)

	//!
	dcmAPI::DataSet::Pointer GetDataSet() const;
	void SetDataSet(dcmAPI::DataSet::Pointer val);

	//!
	std::string GetPath() const;
	void SetPath(std::string val);

	std::vector< dcmAPI::Tag > GetQuery() const;
	void SetQuery( const std::vector< dcmAPI::Tag > &val );

	//! 
	void Update( );

private:

	//!
	PACSQueryFileReader();
	//!
	~PACSQueryFileReader();

private:

	std::vector< dcmAPI::Tag > m_Query;

	//!
	std::string m_Path;

	//!
	dcmAPI::DataSet::Pointer m_DataSet;
};

} // namespace dcmAPI

#endif // _dcmPACSQueryFileReader_H
