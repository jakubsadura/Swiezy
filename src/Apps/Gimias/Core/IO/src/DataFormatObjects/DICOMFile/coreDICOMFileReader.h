/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDICOMFileReader_H
#define coreDICOMFileReader_H

#include "coreBaseDataEntityReader.h"
#include "dcmPatient.h"
#include "dcmSeries.h"
#include "dcmAbstractImageReader.h"

namespace Core
{
namespace IO
{
/** 
A specialization of the DataEntityReader class DICOM files.

Read DICOM files:
 - General Electric US: 3D+T images and ECG signal
 - Phillips US: 3D+T images
 - DICOMDIR
 - DICOM file
 - DICOM directory

\ingroup gmIO
\author Xavi Planes
\date 15 June 2009
*/
class GMIO_EXPORT DICOMFileReader : public BaseDataEntityReader
{
public:
	coreDeclareSmartPointerTypesMacro(DICOMFileReader,BaseDataEntityReader)
	coreClassNameMacro(DICOMFileReader)

	static Pointer New(void);

	//!
	virtual ::itk::LightObject::Pointer CreateAnother(void) const;

	//!
	void Update();

	//!
	void RegisterReader( dcmAPI::AbstractImageReader::Pointer );

	//!
	void UnRegisterReader( dcmAPI::AbstractImageReader::Pointer );

	//!
	std::list<dcmAPI::AbstractImageReader::Pointer> GetRegisteredReaders( );

protected:
	DICOMFileReader(void);
	virtual ~DICOMFileReader(void);

	//! Use special readers for single DICOM files
	std::vector<Core::DataEntity::Pointer> ImportSingleDICOMFile( 
		std::string filename );

	std::vector<Core::DataEntity::Pointer> ImportDICOM( 
		std::string filename );

	//!
	void AddPatientData( 
		dcmAPI::Patient::Pointer patientData,
		Core::DataEntity::Pointer dataEntity );

	//!
	void AddSeriesData( 
		dcmAPI::Series::Pointer seriesData,
		Core::DataEntity::Pointer dataEntity );

	//!
	virtual boost::any ReadSingleTimeStep( 
		int iTimeStep, 
		const std::string &filename );

private:
	coreDeclareNoCopyConstructors(DICOMFileReader);

	//!
	std::list<dcmAPI::AbstractImageReader::Pointer> m_ReaderList;
};

}
}

#endif //coreDICOMFileReader
