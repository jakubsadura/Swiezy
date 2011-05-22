/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef DCMMULTISLICEREADER_H
#define DCMMULTISLICEREADER_H

// DCMAPIIO_EXPORT
#include "dcmAPIIOWin32Header.h"

// BaseLib
#include "blLightObject.h"
#include "blSignalCollective.h"
#include "CILabExceptionMacros.h"

//DcmAPI
#include "dcmDataSet.h"
#include "dcmTypes.h"
#include "dcmAbstractImageReader.h"

//VTK
#include "vtkSmartPointer.h"
#include "vtkImageData.h"

namespace dcmAPI{

/** 
* \brief DICOM multislice file reader.
* 
* Read ONE MultiSlice DICOM file:
*  - Can be 3D
*  - Philips
*  - GE
* 
* \ingroup img_readers
* \author Xavi Planes
* \date 24 July 2009
*/
class DCMAPIIO_EXPORT MultiSliceReader : public blLightObject
{
public:
	typedef MultiSliceReader Self;
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

	//!
	std::vector< vtkSmartPointer<vtkImageData> > GetOutput( );

	//!
	std::vector< float > GetVolumesTimestamps() const;

	//!
	blSignalCollective::Pointer GetSignalCollectiveECG() const;

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
	MultiSliceReader();
	//!
	~MultiSliceReader();

private:

	//!
	std::string m_Path;

	//!
	dcmAPI::DataSet::Pointer m_DataSet;

	//! 
	std::vector< vtkSmartPointer<vtkImageData> > m_vtkVolumesVector;

	//!
	std::vector < float > m_VolumesTimestamps;

	//!
	blSignalCollective::Pointer m_SignalCollectiveECG;
	
	// Array of possible readers
	std::vector< AbstractImageReader::Pointer > m_Readers;

};

} // namespace dcmAPI

#endif // DCMMULTISLICEREADER_H
