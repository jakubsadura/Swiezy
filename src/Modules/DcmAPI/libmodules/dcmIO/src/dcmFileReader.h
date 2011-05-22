/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef DCMFILEREADER_H
#define DCMFILEREADER_H

// DCMAPI_EXPORT
#include "dcmAPIIOWin32Header.h"

// BaseLib
#include "blLightObject.h"
#include "CILabExceptionMacros.h"

// DcmAPI
#include "dcmDataSet.h"
#include "dcmTypes.h"
#include "dcmFile.h"
#include "dcmAbstractImageReader.h"

namespace dcmAPI{

/** 
* \brief DICOM file DataSet reader.
* 
* Reads a file and create the objects to 
* store the DataSet using the hierarchy defined by dcmAPI classes. 
* The data can be 2D or 3D (also 2D + time).
* 
* \ingroup dset_readers
* \author Xavi Planes
* \date 24 July 2009
*/
class FileReader : public blLightObject
{
public:
	typedef FileReader Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)

	//!
	dcmAPI::DataSet::Pointer GetDataSet() const;
	void SetDataSet(dcmAPI::DataSet::Pointer val);

	//!
	std::string GetPath() const;
	void SetPath(std::string val);

	//! 
	void Update( );
	
	/**
	* Add an image reader to the reader list.
	* Depending on the manufacturer, modality and data type, the appropriate reader will be used.
	* @param reader An image reader
	*/
	void AddReader( AbstractImageReader::Pointer reader )
	{
	    m_Readers.push_back( reader );
	}

private:

	//!
	FileReader();
	//!
	~FileReader();

	//!
	DcmDataDimension GetDataDimensionFromDcmFile(
		GdcmFilePtr gdcmFile, 
		std::string manufacturer, 
		std::string modality);

private:

	//!
	std::string m_Path;

	//!
	dcmAPI::DataSet::Pointer m_DataSet;
	
	// Array of possible readers
	std::vector< AbstractImageReader::Pointer > m_Readers;
};

} // namespace dcmAPI

#endif // DCMFILEREADER_H
