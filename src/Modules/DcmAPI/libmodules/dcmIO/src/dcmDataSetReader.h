/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dcmDataSetReader_H
#define _dcmDataSetReader_H

// DCMAPIIO_EXPORT
#include "dcmAPIIOWin32Header.h"

//DcmAPI
#include "dcmDataSet.h"
#include "dcmTypes.h"
#include "dcmAbstractImageReader.h"

#include "CILabExceptionMacros.h"

#include <log4cplus/logger.h>

namespace dcmAPI
{

/** 
* \brief DICOM DataSet reader.
* 
* Reads a DICOMDIR file, directory or DcmFile and create the objects to 
* store the DataSet using the hierarchy defined by dcmAPI classes. 
* 
* Facade class for other 
* 
* \ingroup dset_readers
* \author Pedro Omedas
* \date 15 May 2008
*/
class DCMAPIIO_EXPORT DataSetReader
{
public:
    //!
    typedef boost::shared_ptr<DataSetReader> Pointer;
    //!
    cilabDeclareExceptionMacro(DcmDataSetReaderException, std::exception)
    //!
    static Pointer New();
    //!
    DataSetReader();
    //!
    ~DataSetReader();

    dcmAPI::DataSet::Pointer GetDataSet() const;
    void SetDataSet(dcmAPI::DataSet::Pointer val);

    /** 
    Reads DICOMDIR file in the location specified in path 
    and fill all the structures of data under the hierarchy of the 
    Data Set (Patient, Study, Series, TimePoint and Slice).
    */
    void ReadDicomDir(const std::string& path);

    /** 
    Reads recursively directory in the location specified in path 
    and fill all the structures of data under the hierarchy of the 
    Data Set (Patient, Study, Series, TimePoint and Slice).
    */
    void ReadDirectory(const std::string& path);

    /**
    Reads specified dcm file and fill all the structures of data under
    the hierarchy of the Data Set (Patient, Study, Series, TimePoint and Slice).
    */
    void ReadDcmFile(const std::string& path);

    /**
    Reads multislice(3D or 4D) dcm files and fill all the structures of data under
    the hierarchy of the Data Set (Patient, Study, Series, TimePoint and Slice).
    */
    void ReadMultiSliceDcmFiles(const std::vector< std::string>& filePaths);

    /**
    Gets the vector of filepaths to the multi slice files in a directory
    */
    std::vector<std::string> GetFilepathsOfSlices() const;

    /**
    Merge different datasets in one
    */
    void MergeDataSets(std::vector<dcmAPI::DataSet::Pointer> dataSetVector );

    /**
    * Add an image reader to the reader list.
    * Depending on the manufacturer, modality and data type, the appropriate reader will be used.
    * @param reader An image reader
    */
    void AddReader( AbstractImageReader::Pointer reader )
    {
        m_Readers.push_back( reader );
    }

    //! Get the DICOM tag used as time identifier.
    TagId GetTimeTag() const;

    //! Set the tag used as time identifier.
    void SetTimeTag(const TagId& timeTag);

    //! Set the separator and level for the time tag truncation.
    void SetTimeTagTruncate(char separator, int level);

    //! Set the time tag location: file or image (sometimes the file contains more information).
    void SetTimeTagLocation(bool isInFile);

private:

    //!
    void RecognizeData();

    //!
    DataSet::Pointer m_DataSet;

    // Array of possible readers
    std::vector< AbstractImageReader::Pointer > m_Readers;

    //! logger
    log4cplus::Logger m_logger;

    //! Tag used as time identifier
    TagId m_TimeTag;

    //! Separator to truncate the UID to use as time value
    int m_TruncateSeparator;

    //! Level up to which to truncate the UID to use as time value
    int m_TruncateLevel;

    //! Flag to know it the time tag is in the file or the image
    bool m_IsTimeTagInFile;

};

} // namespace _dcmReader_H

#endif // Engine_H
