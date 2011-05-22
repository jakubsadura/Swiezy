/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dcmDataSet.h"

#include <iostream>

using namespace dcmAPI;

DataSet::DataSet()
{
	m_storageType = SINGLE_SLICE_PER_FILE;
	m_dataType = STANDARD;
}

DataSet::~DataSet()
{
	m_patientsVector.clear();
}

PatientIdVectorPtr DataSet::GetPatientIds() const
{
	PatientIdVectorPtr patientIdVector(new PatientIdVector());

	for(unsigned int i=0; i < m_patientsVector.size(); i++)
	{
		Patient::Pointer patient = m_patientsVector.at(i);
		patientIdVector->push_back(patient->GetTagAsText(tags::PatientId));
	}

	return patientIdVector;
}

Patient::Pointer DataSet::GetPatient(const PatientId& patientId) const
{
	Patient::Pointer retPatient;

	for(unsigned int i=0; i < m_patientsVector.size(); i++)
	{
		Patient::Pointer patient = m_patientsVector.at(i);
		if (patient->GetTagAsText(tags::PatientId) == patientId)
		{
			retPatient = patient;
			break;
		}
	}
	return retPatient;
}

Slice::Pointer DataSet::GetSlice(PatientId patientId, StudyId studyId, SeriesId seriesId, TimePointId timePointId,SliceId sliceId)
{
	Slice::Pointer retSlice;

	try
	{
		//Get the patient 
		Patient::Pointer patient = GetPatient(patientId);
		if(patient == NULL)
		{
			std::cerr << "DataSet::GetSlice: PatienId not found" << std::endl;
		}else{
			//Get the Study
			Study::Pointer study = patient->Study(studyId);
			if(study == NULL)
			{
				std::cerr << "DataSet::GetSlice: StudyId not found" << std::endl;
			}else{
				//Get the Serie
				Series::Pointer serie = study->Series(seriesId);
				if(serie == NULL)
				{
					std::cerr << "DataSet::GetSlice: SerieId not found" << std::endl;
				}else
				{
					//Get the Timepoint
					TimePoint::Pointer timePoint = serie->TimePoint(timePointId);
					if(timePoint == NULL)
					{
						std::cerr << "DataSet::GetSlice: TimepointId not found" << std::endl;
					}else
					{
						retSlice = timePoint->Slice(sliceId);
					}

				}
			}
		}
	}
	catch (...)
	{
	}

	return retSlice;
}

Series::Pointer DataSet::GetSeries(PatientId patientId, StudyId studyId, SeriesId seriesId)
{
	Series::Pointer retSeries;
	try
	{	
		//Get the patient 
		Patient::Pointer patient = GetPatient(patientId);
		if(patient == NULL)
		{
			std::cerr << "DataSet::GetSeries: PatienId not found" << std::endl;
		}else{
			//Get the Study
			Study::Pointer study = patient->Study(studyId);
			if(study == NULL)
			{
				std::cerr << "DataSet::GetSeries: StudyId not found" << std::endl;
			}else{
				//Get the Serie
				Series::Pointer series = study->Series(seriesId);
				if(series == NULL)
				{
					std::cerr << "DataSet::GetSeries: SerieId not found" << std::endl;
				}
				else
					retSeries = series;
			}
		}
	}
	catch (...)
	{
	}
	return retSeries;
}

TimePoint::Pointer DataSet::GetTimePoint(PatientId patientId, StudyId studyId, SeriesId seriesId, TimePointId timePointId)
{
	TimePoint::Pointer retTimePoint;
	try
	{
		//Get the patient 
		Patient::Pointer patient = GetPatient(patientId);
		if(patient == NULL)
		{
			std::cerr << "DataSet::GetSlice: PatienId not found" << std::endl;
		}else{
			//Get the Study
			Study::Pointer study = patient->Study(studyId);
			if(study == NULL)
			{
				std::cerr << "DataSet::GetSlice: StudyId not found" << std::endl;
			}else{
				//Get the Serie
				Series::Pointer serie = study->Series(seriesId);
				if(serie == NULL)
				{
					std::cerr << "DataSet::GetSlice: SerieId not found" << std::endl;
				}else
				{
					//Get the Timepoint
					retTimePoint = serie->TimePoint(timePointId);
				}
			}
		}
	}
	catch(...)
	{
	}	
	return retTimePoint;
}

unsigned int DataSet::GetNumberOfSlices()
{
	unsigned int count = 0;
	PatientIdVectorPtr patiendIdVector = GetPatientIds();
	for(size_t i = 0; i < patiendIdVector->size(); i++)
	{
		Patient::Pointer patient = GetPatient(patiendIdVector->at(i));
		//reading studies
		StudyIdVectorPtr studyIdVector = patient->StudyIds();
		for(size_t j=0; j < studyIdVector->size(); j++)
		{
			Study::Pointer study = patient->Study(studyIdVector->at(j));
			//reading series
			SeriesIdVectorPtr seriesIdVector = study->SeriesIds();
			for(size_t k=0; k < seriesIdVector->size(); k++)
			{
				Series::Pointer series = study->Series(seriesIdVector->at(k));
				//reading timepoints
				TimePointIdVectorPtr timepointIdVector = series->TimePointIds();
				for(size_t l=0; l < timepointIdVector->size(); l++)
				{
					TimePoint::Pointer timepoint = series->TimePoint(timepointIdVector->at(l));
					//reading slices
					SliceIdVectorPtr sliceIdVector = timepoint->SliceIds();
					for(size_t m=0; m < sliceIdVector->size(); m++)
						count++;
				}

			}

		}
	}

	return count;
}

bool DataSet::FindPatient( const PatientId& patientId ) const
{
	bool found = false;

	for(unsigned int i=0; i < m_patientsVector.size(); i++)
	{
		Patient::Pointer patient = m_patientsVector.at(i);
		if (patient->GetTagAsText(tags::PatientId) == patientId)
		{
			found = true;
		}
	}

	return found;
}

DataSet::PatientsVector DataSet::GetPatientsVector() const
{
	return m_patientsVector;
}

void DataSet::SetPatientsVector( PatientsVector val )
{
	m_patientsVector = val;
}

void DataSet::AddPatient( Patient::Pointer patient )
{
	if ( !FindPatient( patient->GetTagAsText(tags::PatientId) ) )
	{
		m_patientsVector.push_back( patient );
	}
}

void DataSet::Clear()
{
	m_patientsVector.resize( 0 );
}

const DataSet::StorageType& DataSet::GetStorageType()
{
	return m_storageType;
}

void DataSet::SetStorageType( DataSet::StorageType sType )
{
	m_storageType = sType;
}

void DataSet::SetDataType( DataSet::DataType type )
{
	m_dataType = type;
}

DataSet::DataType DataSet::GetDataType()
{
	return m_dataType;
}

//void DataSet::SetSliceLayoutType( DataSet::SliceLayoutType type )
//{
//	m_sliceLayoutType = type;
//}

//DataSet::SliceLayoutType DataSet::GetSliceLayoutType()
//{
//	return m_sliceLayoutType;
//}

//!
Base::Pointer DataSet::GetFirstSpecifiedElement(DataSetElementType type)
{
	Base::Pointer baseElement = Base::New();
	PatientIdVectorPtr patiendIdVector = GetPatientIds();
	if( patiendIdVector->size()>0 )
	{
		Patient::Pointer patient = GetPatient(patiendIdVector->at(0));
		if(type == PATIENT)
			return patient;

		//reading studies
		StudyIdVectorPtr studyIdVector = patient->StudyIds();
		if(studyIdVector->size()>0)
		{
			Study::Pointer study = patient->Study(studyIdVector->at(0));
			if(type == STUDY)
				return study;

			//reading series
			SeriesIdVectorPtr seriesIdVector = study->SeriesIds();
			if(seriesIdVector->size()>0)
			{
				Series::Pointer series = study->Series(seriesIdVector->at(0));
				if(type == SERIES)
					return series;

				//reading timepoints
				TimePointIdVectorPtr timepointIdVector = series->TimePointIds();
				if(timepointIdVector->size()>0)
				{
					TimePoint::Pointer timepoint = series->TimePoint(timepointIdVector->at(0));
					if(type == TIMEPOINT)
						return timepoint;

					//reading slices
					SliceIdVectorPtr sliceIdVector = timepoint->SliceIds();
					if(sliceIdVector->size()>0)
					{
						Slice::Pointer slice = timepoint->Slice(sliceIdVector->at(0));
						if(type == SLICE)
							return slice;
					}
				}

			}

		}
	}

	return baseElement;
}
