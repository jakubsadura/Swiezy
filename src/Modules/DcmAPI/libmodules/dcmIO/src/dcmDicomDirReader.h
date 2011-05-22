/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dcmDicomDirReader_H
#define _dcmDicomDirReader_H

// DCMAPIIO_EXPORT
#include "dcmAPIIOWin32Header.h"

#include "blLightObject.h"
#include "CILabExceptionMacros.h"

//DcmAPI
#include "dcmDataSet.h"
#include "dcmTypes.h"
#include "dcmFile.h"

namespace dcmAPI{

/** 
* \brief DICOMDIR DataSet reader.
* 
* Reads a DICOMDIR file and create the objects to 
* store the DataSet using the hierarchy defined by dcmAPI classes. 
* 
* \ingroup dset_readers
* \author Xavi Planes
* \date 24 July 2009
*/
class DicomDirReader : public blLightObject
{
public:
	typedef DicomDirReader Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)

	std::string GetPath() const;
	void SetPath(std::string val);

	dcmAPI::DataSet::Pointer GetDataSet() const;
	void SetDataSet(dcmAPI::DataSet::Pointer val);

	//! read DICOMDIR file
	void Update();

private:
	DicomDirReader( );

private:

	//!
	dcmAPI::DataSet::Pointer m_DataSet;

	//!
	std::string m_Path;
};

} // namespace dcmAPI{

#endif // _dcmDicomDirReader_H
