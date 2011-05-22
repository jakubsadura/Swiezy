/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef DCMABSTRACTIMAGEREADER_H
#define DCMABSTRACTIMAGEREADER_H

// dcmapi
#include <dcmFile.h>
#include <dcmTypes.h>
#include <dcmDataSet.h>
#include <dcmEcgData.h>
// vtk
#include <vtkImageData.h>
#include <vtkSmartPointer.h>

namespace dcmAPI
{

class DCMAPIIO_EXPORT AbstractImageReader
{
public:

    // Pointer to an AbstractImageReader
    typedef boost::shared_ptr<AbstractImageReader> Pointer;
    
    // Destructor
    virtual ~AbstractImageReader() {}
    
    //@defgroup group1 Generic methods.
    //@{

    /**
    * Check if this reader can read the input data type.
    * @input dataType The data type to check.
    * @return True if the reader supports the input data type.
    */
    bool CanRead( const DataSet::DataType& dataType ) const
    {
        return dataType == m_DataType;
    }
    
    /**
    * Check if this reader can read the input manufacturer/modality pair.
    * @input manufacturer The manufacturer to check.
    * @input modality The modality to check.
    * @return True if the reader supports the input manufacturer/modality pair.
    */
    bool CanRead( const std::string& manufacturer, const std::string& modality ) const
    {
        return ( manufacturer == m_Manufacturer && modality == m_Modality);
    }
    
	/**
	* Check if there is some ECG data.
	* @return True if the data contains an ECG.
	*/
	bool HasEcg() const { return m_HasEcg; }
	
	/**
	* Initialises internal members.
    * @input manufacturer The manufacturer.
    * @input modality The image modality.
    * @input dataType The image data type.
	*/
	void Initialise( 
	    const std::string& manufacturer, 
	    const std::string& modality,  
	    const DataSet::DataType& dataType,
	    bool hasEcg )
	{
        m_Manufacturer = manufacturer;
	    m_Modality = modality; 
        m_DataType = dataType;
        m_HasEcg = hasEcg;
        
        m_Filename = "";
        
	    memset( m_ImageOrigin, 0, 3 * sizeof( m_ImageOrigin[0] ) );
	    memset( m_ImageSpacing, 0, 3 * sizeof( m_ImageSpacing[0] ) );
	    memset( m_ImageDimensions, 0, 3 * sizeof( m_ImageDimensions[0] ) );
        m_NumberOfVolumes = 0;

        m_Preprocessed = false;
	}
	
	/** 
	* Get the origin of the data.
	* @return The origin of the data.
	*/
    const double* GetImageOrigin() const { return this->m_ImageOrigin; }

	/** 
	* Get the spacing of the data.
	* @return The spacing of the data.
	*/
    const double* GetImageSpacing() const { return this->m_ImageSpacing; }

	/** 
	* Get the image dimensions of the data.
	* @return The image dimensions of the data.
	*/
    const int* GetImageDimensions() const { return m_ImageDimensions; }

    /**
    * Get the data dimensions from a dicom file.
    * @return The data dimensions.
    */
    DcmDataDimension GetDataDimension() const
    { 
        DcmDataDimension dim = { 0, 0 };
        dim.slicesCount = GetNumberOfSlices();
        dim.timepointsCount = GetNumberOfVolumes();
        return dim;
    }

	/**
	* Get the number of volumes.
	* @return The number of volumes.
 	*/ 
	unsigned long GetNumberOfVolumes() const { return m_NumberOfVolumes; }

	/**
	* Get the number of slices of the volumes.
	* @return The number of slices of the volumes.
 	*/ 
	unsigned long GetNumberOfSlices() const { return m_ImageDimensions[2]; } 

	//@}

    //@defgroup group2 Methods to override.
    //@{

    /**
    * Set the name of the file to read.
    * Resets the processed flag, a call to Update will read the new file.
    * @input filename The file name.
    */
	virtual void SetFilename( const std::string& filename ) = 0;
	
    /**
    * Main 'filter' method, reads the files and stores the data.
    * To call after SetFilename and before the Get* methods.
    */
	virtual void Update() = 0;
	
    /**
    * Get the read volumes and timestamps.
    * @param volumes A reference to an array of image pointers, will be over written.
    * @param timestamps A reference to an array of floats, will be over written.
    */
    virtual void GetVolumes( std::vector< vtkSmartPointer<vtkImageData> >& volumes, std::vector< float >& timestamps ) = 0;

    /**
    * Get the ECG data.
    * @param ecg A reference to the ECG data, will be over written.
    */
	virtual void GetEcgData( EcgData& ecg ) = 0;

	//@}

protected:

    //@defgroup group3 Members to initialise at contruction time.
    //@{
    
    // The data manufacturer.
    std::string m_Manufacturer;
    // The data modality.
    std::string m_Modality;
    // The data type.
    DataSet::DataType m_DataType;
	// Does the image contain an ECG?
	bool m_HasEcg;
	
	//@}

	/**
	* Internal flag to know if the update has been run. 
	* Has to be initialised in derived class.
	*/
    bool m_Preprocessed;
	
	// The DICOM file name.
    std::string m_Filename;
    
    // The data origin.
    double m_ImageOrigin[3];
    // The data spacing.
    double m_ImageSpacing[3];
    // The data dimensions.
    int m_ImageDimensions[3];
    // The number of volumes.
    int m_NumberOfVolumes;
    
}; // class AbstractImageReader

/**
* Exception to throw when reading image.
*/
class ImageReaderException : public std::exception
{
public:
	ImageReaderException(const char* what) : m_what(what)
	{};
	
	~ImageReaderException() throw()
	{};
	
	const char* what() const throw()
	{
	    return m_what.c_str();
	};
private:
	std::string m_what; 
};

} // namespace dcmAPI

#endif // DCMABSTRACTIMAGEREADER_H
