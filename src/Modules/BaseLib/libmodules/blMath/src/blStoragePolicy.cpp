/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blStoragePolicy.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace ublas {

class DiskStoragePolicySettings
{
public:
	template <typename ValueType> friend class blDiskStoragePolicy;

	//!
	DiskStoragePolicySettings();
	//! See global function SetDiskFileStorageFolder.
	void SetFolder(const std::string folderName, const bool createFolder = false);
	//! Generates a new path that can be used for storing a matrix.
	std::string GenerateMatrixPath();
	//! See global function GetMaxSizeOfAMatrixStoredInMemoryInMb.
	size_t GetMaxSizeOfAMatrixStoredInMemoryInMb() const;
	//! See global function SetMaxSizeOfAMatrixStoredInMemoryInMb.
	void SetMaxSizeOfAMatrixStoredInMemoryInMb(size_t val);

private:
	//! See SetFolder
	std::string m_FolderName;
	//! Used to generate new matrix paths.
	int fileNumber;
	//!
	size_t m_MaxSizeOfAMatrixStoredInMemoryInMb;
};

CILAB_END_NAMESPACE(ublas)

ublas::DiskStoragePolicySettings::DiskStoragePolicySettings()
{ 
	fileNumber = 0;
	//this->m_MaxSizeOfAMatrixStoredInMemory =1024000; //! default threshold 1GB
	//this->m_MaxSizeOfAMatrixStoredInMemory = std::numeric_limits<int>::max();
	this->m_MaxSizeOfAMatrixStoredInMemoryInMb = 500;
}

void ublas::DiskStoragePolicySettings::SetFolder(const std::string folderName, const bool createFolder) 
{
	try
	{
		m_FolderName = folderName;
		
		if ( !boost::filesystem::exists(folderName) && createFolder)
		{
			//! Create a new folder
			boost::filesystem::create_directory(m_FolderName);
		}
		if( !boost::filesystem::exists(folderName) )
		{
			cilabBoostFormatMacro("The directory %s doesn't exist", folderName, message);
			throw DiskStoragePolicyException(message.c_str());
		}; 
	}
	catch(std::fstream::failure e)
	{
		std::cout<< e.what()  << std::endl;
	}
}

std::string ublas::DiskStoragePolicySettings::GenerateMatrixPath()
{
	std::string aux = "\\";
	std::string s;
	std::stringstream out;
	out << fileNumber;
	s = out.str();
	std::string filename = "temp" + s + ".txt";
	std::string matrixPath = m_FolderName + aux + filename;
	//! Increase the file number in order to avoid collapsing filenames
	fileNumber++;
	return matrixPath;
}

size_t ublas::DiskStoragePolicySettings::GetMaxSizeOfAMatrixStoredInMemoryInMb() const 
{ 
	return m_MaxSizeOfAMatrixStoredInMemoryInMb; 
}

void ublas::DiskStoragePolicySettings::SetMaxSizeOfAMatrixStoredInMemoryInMb(size_t val) 
{
	if( m_MaxSizeOfAMatrixStoredInMemoryInMb == val )
		return;
	m_MaxSizeOfAMatrixStoredInMemoryInMb = val; 
}

typedef blSingleton< ublas::DiskStoragePolicySettings > blDiskStoragePolicySettingsSingleton;

//! Set the folder where blDiskStoragePolicy stores its matrices.
//! \arg createFolder - Flag: create the folder if \a folderName does not exist.
void ublas::SetDiskFileStorageFolder( const std::string folderName, const bool createFolder )
{
	blDiskStoragePolicySettingsSingleton::Instance()->SetFolder(folderName, createFolder);
}

//! Returns the current maximum size of a matrix that is still stored in memory. Matrices larger than this size
//! will be stored on disk.
size_t ublas::GetMaxSizeOfAMatrixStoredInMemoryInMb()
{ 
	return blDiskStoragePolicySettingsSingleton::Instance()->GetMaxSizeOfAMatrixStoredInMemoryInMb(); 
}

//! Sets the current maximum size (in MB) of a matrix that is still stored in memory. Matrices larger than this size
//! will be stored on disk.
void ublas::SetMaxSizeOfAMatrixStoredInMemoryInMb(size_t valueInMegaBytes) 
{
	blDiskStoragePolicySettingsSingleton::Instance()->SetMaxSizeOfAMatrixStoredInMemoryInMb(valueInMegaBytes); 
}

std::string ublas::GenerateUblasMatrixPath()
{
	return blDiskStoragePolicySettingsSingleton::Instance()->GenerateMatrixPath(); 
}
