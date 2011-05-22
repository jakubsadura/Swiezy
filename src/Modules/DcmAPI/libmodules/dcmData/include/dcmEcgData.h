/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef DCMDECGDATA_H
#define DCMDECGDATA_H

#include <vector>

namespace dcmAPI
{

/**
* Storage for ECG data.
* Provides ECG data as an array of floats, a start time and a time increment.
*/
class DCMAPIIO_EXPORT EcgData
{
public:

    /**
    * Default constructor.
    */
    EcgData() : m_data(0), m_startTime(0), m_timeIncrement(0) {}
    
    /**
    * Constructor.
    * @param data A reference to the ECG data, will be over written.
    * @param startTime The ECG start time.
    * @param timeIncrement The ECG time increment.
    */
    EcgData( const std::vector< float >& data, double startTime, double timeIncrement )
    : m_data(data), m_startTime(startTime), m_timeIncrement(timeIncrement) {}
    
    /**
    * Get the ECG data (array of floats).
    * @param data A reference to the ECG data, will be over written (copy of the internal vector). 
    */
    void GetData( std::vector< float >& data ) { data = m_data; }
	
    /**
    * Get the ECG start time.
    * @return The ECG start time.
    */
	double GetStartTime() const { return m_startTime; }

    /**
    * Get the ECG time increment.
    * @return The ECG time increment.
    */
	double GetTimeIncrement() const { return m_timeIncrement; }

private:

    // ECG data
    std::vector< float > m_data;
    // Start time
    double m_startTime;
    // Time increment
    double m_timeIncrement;
    
}; // class EcgData

} // namespace dcmAPI

#endif // DCMDECGDATA_H
