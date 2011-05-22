/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef blSignal_H
#define blSignal_H

#include "BaseLibSignalWin32Header.h"
#include "blObject.h"
#include "blMacro.h"
#include <vector>
#include "CILabExceptionMacros.h"
#include "blSignalAnnotation.h"

#ifdef WIN32
  typedef __int64 InternalTimeType;
#else
  #include <inttypes.h>
  typedef int64_t InternalTimeType;
#endif

/**
Signal like an ECG

There are two types of signal:
 - Equidistant
 - Non equidistant

\author Xavi Planes, Jakub Lyko
\date 24 April 2009
\ingroup blSignal
*/
class BASELIBSIGNAL_EXPORT blSignal : public blObject 
{
public:
	typedef blSignal Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)
	
	typedef float SampleType;
	
	/** Duration of data record in seconds, defined as a rational number 
	consisting of the numerator and dominator (look in the .gdf format documentation)
	*/
	struct Duration {
		unsigned int numerator;
		unsigned int dominator;
		Duration( unsigned int num = 1, unsigned int den = 1)
		{
			numerator = num;
			dominator = den;
		}
	};

public:

	//! Build a signal equidistant instance
	static blSignal::Pointer BuildSignalFromEcgData(
		std::vector< float > &ecgSamples,
		double ecgStartTime,
		double ecgIncrementTime,
		int scaleEcgSamplesByValue
		);

	//! Build a non equidistant signal
	static blSignal::Pointer Build(
		std::string name,
		std::string xUnit,
		std::string yUnit,
		std::vector< SampleType >& xVector,
		std::vector< SampleType >& yVector );	

	//! Build an equidistant signal 
	static blSignal::Pointer Build(
		std::string name,
		std::string yUnit,
		std::vector< SampleType >& yVector );	

	//! Build an equidistant signal and allocate memory
	static blSignal::Pointer Build( 
		std::string name,
		unsigned int numberOfRecords,
		Duration recordDuration,
		unsigned long samplesPerRecord);

	//! Build a copy without copying values/annotations
	static blSignal::Pointer Build(
		blSignal::Pointer input,
		SampleType value,
		bool copyAnnotations );

	//!
	void Copy( blSignal::Pointer input );

	//!
	std::string GetName() const;
	void SetName(const std::string val);

	//!
	std::string GetXUnit() const;
	void SetXUnit(const std::string val);

	//!
	std::string GetYUnit() const;
	void SetYUnit(std::string val);

	//!
	std::vector< SampleType > GetXVector() const;
	void SetXVector(const std::vector< SampleType >& val);

	//!
	std::vector< SampleType > GetYVector() const;
	void SetYVector(const std::vector< SampleType >& val);

	//!
	size_t GetNumberOfValues( );

	//!
	SampleType GetMinXValue();

	//!
	SampleType GetMaxXValue();

	//!
	SampleType GetMinYValue();

	//!
	SampleType GetMaxYValue();

	/*!
	Find the position of the x value in the m_xVector
	\param x [in]  The x vector value 
	\param y [out] The corresponding position in m_xVector
	\return true if the x value was successfully found, false otherwise
	*/
	bool FindXValuePosition(SampleType x, unsigned& pos); 

	//! Get Value of m_xVector. If m_xVector is not set, return xPos
	SampleType GetValueX( unsigned xPos );

	//! Get Value of m_yVector. If m_yVector is not set, return 0
	SampleType GetValueY( unsigned yPos );

	//!
	void SetValue( long pos, SampleType x, SampleType y );

	//!
	void SetValue( long pos, SampleType y );

	//!
	unsigned int GetNumberOfRecords() const;
	void SetNumberOfRecords(unsigned int val);

	//!
	Duration GetRecordDuration() const;
	void SetRecordDuration(Duration val);

	//!
	unsigned long GetSamplesPerRecord() const;
	void SetSamplesPerRecord(unsigned long val);

	//!
	void AddAnnotation( blSignalAnnotation::Pointer annotation );

	//!
	void RemoveAnnotation(int id );

	//!
	void UpdateAnnotation(int id, 
						int type,
						std::size_t duration );

	//!
	std::vector< blSignalAnnotation::Pointer > GetAnnotationsVector() const;

	//!
	blSignalAnnotation::Pointer GetAnnotation( int id );

	//! Find the first annotation
	blSignalAnnotation::Pointer FindAnnotation( unsigned short type );

	//! frequency: Number of samples per second (1/T)
	double GetFrameRate( );

	//!
	void Allocate( );

	//!
	bool IsEquidistant( );

	//!
	SampleType GetStartTime( );
	void SetStartTime( SampleType startTime );

	InternalTimeType GetInternalStartTime( );
	void SetInternalStartTime( InternalTimeType startTime );

	//!
	void CopyAnnotations( blSignal::Pointer input );

protected:
	blSignal( );

	~blSignal( );

private:

	//!
	std::string m_name;
	
	//!
	std::string m_xUnit;
	
	//!
	std::string m_yUnit;
	
	//!
	std::vector< SampleType > m_xVector;
	
	//!
	std::vector< SampleType > m_yVector;

	//!
	std::vector< blSignalAnnotation::Pointer > m_AnnotationsVector;

	/** 
	Record is a data segment with a fixed length. Each record is 
	related to the sample rate
	SampleRate = m_SamplesPerRecord / m_recordDuration;
	*/
	unsigned int m_numberOfRecords;
	
	/** Duration of a record, as a rational number in seconds (first 
	the numerator, secondly the denominator.
	*/
	Duration m_recordDuration;

	/**
	The “Samples Per Record” is the	sampling rate multiplied by the 
	duration of a record. A value of zero (samples per record), indicates 
	a signal with sparse (non equidistant) sampling. 
	SampleRate = m_SamplesPerRecord / m_recordDuration;
	*/
	unsigned long m_SamplesPerRecord;

	/** Start date and time of recording: 
	8byte (64 bit) binary format
	- The 4 least significant bytes indicate the fraction of a day (2^32 
	corresponds to 3 600 * 24 = 86 400s or 1 day) and provides a time
	resolution of approx 20 µs
	- The 4 most significant bytes count the number of the days since 1.
	Jan 0000
	*/
	InternalTimeType m_StartTime;
};

#endif //blSignal_H
