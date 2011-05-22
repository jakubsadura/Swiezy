/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dcmDataSet_H
#define _dcmDataSet_H

#include "dcmAPIDataWin32Header.h"

//Common libraries
#include <string>
#include <vector>
#include <map>

//Boost
#include <boost/foreach.hpp>

//DcmAPI
#include "dcmSeries.h"
#include "dcmPatient.h"
#include "dcmTypes.h"

namespace dcmAPI
{

/** 
* \brief Class that holds the structure of a DICOM DataSet.
* 
* Reminder: the DICOM data structure can be modeled as a hierarchy tree which levels are:
* DataSet > Patient > Study > Series > TimePoint > Slice
* 
* The reader associated to this structure is the dcmAPI::DataSetReader.
*
* \ingroup data
* \author Pedro Omedas
* \date 15 May 2008
*/
class DCMAPIDATA_EXPORT DataSet
{
public:
	//!
	cistibBoostPointerMacro(DataSet);
	//!
	enum StorageType {SINGLE_SLICE_PER_FILE, MULTI_SLICE_PER_FILE, PACS};
	//!
	enum DataType {STANDARD, PHILIPS_US, GE_US};
	//!
	//enum SliceLayoutType {PARALLEL_SLICES, NON_PARALLEL_SLICES};
	//!
	typedef std::vector<Patient::Pointer> PatientsVector;

	//! Return a vector of patient ids in the current DICOM Data set.
	PatientIdVectorPtr GetPatientIds() const;
	
	/** Returns a Patient object from the vector of patients in the DICOM Data set.
	\note if patientId is not found -> Return a new patient without 
	inserting it into the vector
	*/
	Patient::Pointer GetPatient(const PatientId& patientId) const;
	//!
	Series::Pointer GetSeries(
		PatientId patientId, 
		StudyId studyId, 
		SeriesId seriesId);

	//!
	TimePoint::Pointer GetTimePoint(
		PatientId patientId, 
		StudyId studyId, 
		SeriesId seriesId, 
		TimePointId timePointId);

	//!
	Slice::Pointer GetSlice(
		PatientId patientId, 
		StudyId studyId, 
		SeriesId seriesId, 
		TimePointId timePointId,
		SliceId sliceId);

	//!
	Base::Pointer GetFirstSpecifiedElement(DataSetElementType type);

	//!
	int GetTest(); 

	//!
	const StorageType& GetStorageType();
	//!
	DataType GetDataType();
	//!
	//SliceLayoutType GetSliceLayoutType();
	//!
	unsigned int GetNumberOfSlices();
	//!
	DataSet();
	//!
	~DataSet();

	//!
	dcmAPI::DataSet::PatientsVector GetPatientsVector() const;
	//!
	void SetPatientsVector(dcmAPI::DataSet::PatientsVector val);

	//! Add a patient to the vector if it is not yet inside
	void AddPatient( dcmAPI::Patient::Pointer patient );

	//!
	bool FindPatient(const PatientId& patientId) const;

	//!
	void Clear( );

	//!
	void SetStorageType(StorageType sType);
	//!
	void SetDataType(DataType type);
	//!
	//void SetSliceLayoutType(SliceLayoutType type);

private:
	PatientsVector m_patientsVector;

	//! is set by RecognizeDicomDataType()
	StorageType m_storageType;

	//! is set by RecognizeStorageType()
	DataType m_dataType;

	//! is set by RecognizeData()
	//SliceLayoutType m_sliceLayoutType;
};

} // namespace dcmAPI

#endif // End _dcmDataSet_H
