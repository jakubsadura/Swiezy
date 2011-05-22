/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _DCMFILE_H
#define _DCMFILE_H

#include "dcmAPIIOWin32Header.h"

//DcmAPI
//#include "dcmImage.h"
#include "dcmTypes.h"
#include "gdcmFile.h"
#include "gdcmValEntry.h"

//!
typedef boost::shared_ptr< gdcm::File > GdcmFilePtr;
//!
typedef boost::shared_ptr< gdcm::ValEntry > GdcmValEntryPtr;

namespace dcmAPI
{

/** 
* \brief Dicom File.
* 
* Wrapper around a gdcm file.
* \ingroup io
* \author Jakub Lyko
* \date 15 May 2008
*/

class DCMAPIIO_EXPORT File
{
public:
	typedef enum {UNKNOWNCOMPONENTTYPE,UCHAR,CHAR,USHORT,SHORT,UINT,INT, ULONG,LONG, FLOAT,DOUBLE} PxType;
	//! 
	cistibBoostPointerMacro(File);
	//!
	File();
	//!
	~File();
	//!
	bool Open(std::string file);
	//!
	std::string ReadTagValue(const TagId& tag);
	//!
	//std::string GetPixelType();
	//!
	void SetGdcmFile(gdcm::File* _gdcmFile) { gdcmFile.reset(_gdcmFile); }
	////!
	//const gdcm::File* GetGdcmFile() { return gdcmFile.get(); };
	////!
	//static std::string ReadPixelType(std::string path);
	////!
	//template< typename ImageType > static typename ImageType::Pointer ReadImageFromFile
	//(
	//	const std::string& path
	//);

private:
	//!
	void ImageStringParametersToDouble(const char * string, double * array, unsigned int numberOfTokens);
	//!
	std::string CleanGdcmEntryStringValue(const std::string& value);

private:
	 GdcmFilePtr gdcmFile;

};


} // namespace dcmAPI

#endif //end _DCMFILE_H
