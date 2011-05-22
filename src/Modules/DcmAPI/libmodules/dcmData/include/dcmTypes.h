/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef DCMAPITYPES_H
#define DCMAPITYPES_H

#include <cstdio>
#include <vector>
#include <string.h>
#include <map>
#include <exception>
#include "boost/shared_ptr.hpp"
#include "CILabBoostMacros.h"
#include "dcmAPIDataWin32Header.h"


namespace dcmAPI{

//!
enum DataSetElementType { PATIENT,  STUDY, SERIES, TIMEPOINT, SLICE};
//!
typedef std::string PatientId;
//!
typedef std::vector<PatientId> PatientIdVector;
//!
typedef boost::shared_ptr< PatientIdVector > PatientIdVectorPtr;
//!
typedef std::string StudyId;
//!
typedef std::vector<StudyId> StudyIdVector;
//!
typedef boost::shared_ptr< StudyIdVector > StudyIdVectorPtr;
//!
typedef std::string SeriesId;
//!
typedef std::vector<SeriesId> SeriesIdVector;
//!
typedef boost::shared_ptr< SeriesIdVector > SeriesIdVectorPtr;
//!
typedef std::string TimePointId;
//!
typedef std::vector<TimePointId> TimePointIdVector;
//!
typedef boost::shared_ptr< TimePointIdVector > TimePointIdVectorPtr;
//!
typedef std::string SliceId;
//!
typedef std::vector<SliceId> SliceIdVector;
//!
typedef boost::shared_ptr< SliceIdVector > SliceIdVectorPtr;

/**
This struct contains a dicom tag identifier as it is described on the DICOM standard.
\ingroup data
\author Pedro Omedas
\date 15 May 2008
*/
struct TagId
{
    //! Constructor
    TagId(int _group = 0, int _element = 0, const std::string& _description = "")
    {
        m_group = _group;
        m_element = _element;
        m_description = _description;
    };
    
    //! Equality comparator
    bool operator== ( const TagId &_right)           
    { 
        return( this->m_group == _right.m_group &&
                this->m_element == _right.m_element);
    }
    
    //! Inferior comparator
    bool operator< ( const TagId &_right) const           
    { 
        if(this->m_group < _right.m_group)
            return true;
        else if(this->m_group == _right.m_group && this->m_element < _right.m_element)
            return true;
        else
            return false;
    }
    
    //! Superior comparator
    bool operator> ( const TagId &_right) const      
    { 
        if(this->m_group > _right.m_group)
            return true;
        else if(this->m_group == _right.m_group && this->m_element > _right.m_element)
            return true;
        else
            return false;
    }
    
    //! Is set
    bool IsSet() const
    {
        return !( m_group == 0 && m_element == 0 );
    }

    //! Dicom tag group
    int m_group;
    //! Dicom tag index within this->group.
    int m_element;
    //! Description of the tag (e.g. "Patient's name")
    std::string m_description;
};

/** 
This structure is used to associate a tag value (stored on the dataSet) to the correspondent tag id. 
It simplifies the use of the tagId on the classes that stores information of the dataSet.
For example Patients contains a vector of DcmAPITags, like this:
Tag(tags::_PatientID, "1").
Tag(tags::_PatientName, "Pedro").

\ingroup data
\author Pedro Omedas
\date 15 May 2008
*/
struct Tag
{
    //!
    Tag(TagId _tagId = 0, std::string _value = "")
    {
        m_tagId = _tagId;
        m_value = _value;
    };

    //!
    Tag( std::string line )
    {
        m_tagId.m_group = 0;
        m_tagId.m_element = 0;

        size_t pos = line.find(',');
        if(pos > 0)
        {
            m_tagId.m_group = strtol(line.substr(0,pos).c_str(), NULL, 16);
            line = line.substr(pos+1);
        }

        pos = line.find('=');
        if(pos > 0)
        {
            m_tagId.m_element = strtol(line.substr(0,pos).c_str(), NULL, 16);
            line = line.substr(pos+1);
        }

        m_value = line;
    }

    /** Get tag in string format. Ex: 0010,0020=Name
    \param [in] joinSymbol Symbol used to join the two words 0010,0020:Name.
    */
    const std::string *GetString( const char joinSymbol = ':' ){
        
        char buffer[128];
        sprintf( buffer, "%04X,%04X%c%.100s", 
            m_tagId.m_group,
            m_tagId.m_element,
            joinSymbol,
            m_value.c_str( ) );

        m_TagInTextFormat = std::string( buffer, buffer + strlen( buffer ) );
        return &m_TagInTextFormat;
    }

    //! 
    TagId m_tagId;
    //! 
    std::string m_value;
    //!
    std::string m_TagInTextFormat;
};

/** This struct describe dimension of DICOM data (used for single DICOM file)
\ingroup data
\author Pedro Omedas
\date 15 May 2008
*/
struct DcmDataDimension
{
    int slicesCount;
    int timepointsCount;
};

typedef boost::shared_ptr< Tag > TagPtr;
//!
typedef std::vector<TagPtr> TagVector;
//!
typedef std::map<TagId, std::string> TagsMap;
//!
typedef std::pair<TagId, std::string> TagPair;
//!
typedef std::map<std::string, TagId> TimeTagForModalityMap;
//!
typedef std::pair<std::string, TagId> TimeTagForModalityPair;

/** 
This namespace is used to store instantiations of TagId.
See http://www.fpimage.com/Manuals/Imp/dicomdic.html for a list of standard dicom tags.
\ingroup DcmAPI
\author Pedro Omedas
\date 15 May 2008
*/
namespace tags
{    
    //!Private Tags
    static TagId SliceFilePath                  (0xFFFF, 0x0001, "Slice filepath");
    static TagId SliceFileName                  (0xFFFF, 0x0002, "Slice filename");
    static TagId TimeValue                      (0xFFFF, 0x0003, "Time value");
    static TagId PxType                         (0xFFFF, 0x0004, "GDCM Pixel Type");
    static TagId SliceId                        (0xFFFF, 0x0005, "Slice ID");
    static TagId TimePointId                    (0xFFFF, 0x0006, "Time Point ID");
    static TagId SeriesId                       (0xFFFF, 0x0005, "Series ID");
    static TagId StudyId                        (0xFFFF, 0x0006, "Study ID");
    static TagId PatientId                      (0xFFFF, 0x0005, "Patient ID");

    //!Groupd 002 - File Meta Elements
    static TagId MediaStoredSOPInstanceUID      (0x0002, 0x0003, "Media Stored SOP Instance UID");
    
    //!Groupd 004 - Directory
    static TagId ReferencedFileID               (0x0004, 0x1500, "Referenced File ID");
    static TagId ReferencedSOPInstanceUIDinFile (0x0004, 0x1511, "Referenced SOP Instance UID in File" );

    //!Group 008 - Study Information
    static TagId ImageType                      (0x0008, 0x0008, "Image Type");
    static TagId SOPInstanceUID                 (0x0008, 0x0018, "SOP Instance UID");
    static TagId StudyDate                      (0x0008, 0x0020, "Study Date");
    static TagId StudyTime                      (0x0008, 0x0030, "Study Time");
    static TagId AcquisitionTime                (0x0008, 0x0032, "Acquisition Time");
    static TagId ImageTime                      (0x0008, 0x0033, "Image Time");
    static TagId AccessionNumber                (0x0008, 0x0050, "Accession Number");
    static TagId Modality                       (0x0008, 0x0060, "Modality");
    static TagId Manufacturer                   (0x0008, 0x0070, "Manufacturer");
    static TagId RefPhysician                   (0x0008, 0x0090, "Referring Physician's Name");
    static TagId StudyDescription               (0x0008, 0x1030, "Study Description");
    static TagId SeriesDescription              (0x0008, 0x103E, "Series Description");
    
    //!Group 010 - Patient
    static TagId PatientName                    (0x0010, 0x0010, "Patient's Name");
    static TagId PatientRealID                  (0x0010, 0x0020, "Patient Real ID");
    static TagId PatientDate                    (0x0010, 0x0030, "Patient Date");
    static TagId PatientSex                     (0x0010, 0x0040, "Patient's Sex");
    static TagId PatientAge                     (0x0010, 0x1010, "Patient's Age");

    //!Group 018 - Adquisition Group
    static TagId PatientPosition                (0x0018, 0x5100, "Patient Posit    ion");
    static TagId SliceThickness                 (0x0018, 0x0050, "Slice Thickness");
    static TagId ProtocolName                   (0x0018, 0x1030, "Protocol Name");
    static TagId TriggerTime                    (0x0018, 0x1060, "Trigger Time" );
    static TagId CardiacNumberOfImages          (0x0018, 0x1090, "Cardiac Number of Images" );
    static TagId SpacingBetweenSlices           (0x0018, 0x0088, "Spacing Between Slices");
    
    //!Group 0020 - Relationship Group
    static TagId StudyInstanceUID               (0x0020, 0x000D, "Study Instance UID");
    static TagId StudyRealID                    (0x0020, 0x0010, "Study Real ID");
    static TagId SeriesNumber                   (0x0020, 0x0011, "Series Number");
    static TagId SeriesInstanceUID              (0x0020, 0x000E, "Series Instance UID");
    static TagId StackID                        (0x0020, 0x9056, "Stack ID");
    static TagId FrameOfReference               (0x0020, 0x0052, "Frame of Reference UID");
    static TagId ImageNumber                    (0x0020, 0x0013, "Image Number");
    static TagId PhaseNumber                    (0x0020, 0x0015, "Phase Number");
    static TagId ImagePositionPatient           (0x0020, 0x0032, "Image Position Patient");
    static TagId ImageOrientationPatient        (0x0020, 0x0037, "Image Orientation Patient");
    
    //!Group 0028 - Image Presentation
    static TagId NumberOfFrames                  (0x0028, 0x0008, "Number of frames");
    static TagId ImageRows                       (0x0028, 0x0010, "Image Rows");
    static TagId ImageColumns                    (0x0028, 0x0011, "Image Columns");
    static TagId PixelSpacing                    (0x0028, 0x0030, "Pixel Spacing");
    static TagId WindowWidth                     (0x0028, 0x1051, "Window Width");
    static TagId WindowCenter                    (0x0028, 0x1050, "Window Center");
    
    //!Group 2001 - 
    static TagId PhaseNumberMR                   (0x2001, 0x1008, "Phase Number (dcmtk)");
    static TagId SliceNumberMR                   (0x2001, 0x100a, "Slice Number MR");
    static TagId SliceOrientation                (0x2001, 0x100b, "Slice Orientation");

    //!Group 7005 - 
    static TagId PercentageOfFullHeartBeat       (0x7005, 0x1004, "Percentage of full heartbeat");

};

/**
Reformat data tag
\ingroup DcmAPI
\author Xavi Planes
\date 11 June 2010
*/
std::string DCMAPIDATA_EXPORT CreateDateFromRawDcmTagDate(const std::string& rawDate);


} // namespace dcmAPI

DCMAPIDATA_EXPORT std::ostream& operator<< (std::ostream& stream, const dcmAPI::TagId& tagId);

#endif // DCMAPITYPES_H
