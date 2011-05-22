/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dcmDirectoryReader_H
#define _dcmDirectoryReader_H

// DCMAPI_EXPORT
#include "dcmAPIIOWin32Header.h"

// BaseLib
#include "blLightObject.h"
#include "CILabExceptionMacros.h"
#include <blLogger.h>

//DcmAPI
#include "dcmDataSet.h"
#include "dcmTypes.h"
#include "dcmFile.h"

namespace dcmAPI{

/** 
* \brief DICOM directory DataSet reader.
* 
* Reads a directory and create the objects to 
* store the DataSet using the hierarchy defined by dcmAPI classes. 
* 
* \ingroup dset_readers
* \author Xavi Planes
* \date 24 July 2009
*/
class DirectoryReader : public blLightObject
{
public:
    typedef DirectoryReader Self;
    typedef blSmartPointer<Self> Pointer;
    blNewMacro(Self);
    cilabDeclareExceptionMacro(Exception, std::exception)

    //!
    dcmAPI::DataSet::Pointer GetDataSet() const;
    void SetDataSet(dcmAPI::DataSet::Pointer val);

    //!
    std::string GetPath() const;
    void SetPath(std::string val);

    //! read DICOM data from selected directory
    void Update();

    //! Get the DICOM tag used as time identifier.
    TagId GetTimeTag() const;

    //! Set the tag used as time identifier.
    void SetTimeTag(const TagId& timeTag);

    //! Set the separator and level for the time tag truncation.
    void SetTimeTagTruncate(char separator, int level);

    //! Set the time tag location: file or image (sometimes the file contains more information).
    void SetTimeTagLocation(bool isInFile);

private:
	
    //! Private contructor.
    DirectoryReader();

    //! logger
    log4cplus::Logger m_logger;

    //!
    DataSet::Pointer m_DataSet;

    //!
    std::string m_Path;

    //! Tag used as time identifier
    TagId m_TimeTag;

    //! Flag to know if the time tag is the UID
    bool m_IsTimeTagUID;

    //! Separator to truncate the UID to use as time value
    int m_TruncateSeparator;

    //! Level up to which to truncate the UID to use as time value
    int m_TruncateLevel;

    //! Flag to know it the time tag is in the file or the image
    bool m_IsTimeTagInFile;

    //! Truncate the string up to the input level
    std::string TruncateTagValue(const std::string& inputStr, char separator, int level) const;

    //! Get the final time tag using the modality information
    TagId GetFinalTimeTag(const std::string& modality) const;

    //! Get the time tag value from the document entry
    std::string GetTimeTagValue(gdcm::DocEntry* docEntry) const;

};

} // namespace dcmApi{

#endif // _dcmDirectoryReader_H
