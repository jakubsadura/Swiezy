/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreFile.h"
#include "coreException.h"
#include "coreDirectory.h"
#include <itksys/SystemTools.hxx>
#include <stdio.h>

using namespace Core::IO;

//!
File::File(void)
{
}

//!
File::~File(void)
{
}

//!
void File::SetFileNameFullPath(const std::string& fullPath)
{
	this->m_FileNameFullPath = fullPath;
	DirectoryHelper::ConvertToOutputPath(this->m_FileNameFullPath);
	unsigned int pos = this->m_FileNameFullPath.find_last_of(SlashChar);
	if((pos > 0) && (pos < this->m_FileNameFullPath.length()))
	{
		this->m_PathToFile = this->m_FileNameFullPath.substr(0, pos);
		this->m_FileName = this->m_FileNameFullPath.substr(pos+1, this->m_FileNameFullPath.length());
		DirectoryHelper::RemoveQuotesFromPath(this->m_PathToFile);
		DirectoryHelper::RemoveQuotesFromPath(this->m_FileNameFullPath);
	}
	else
	{
		// throw exception
	}
}


//!
void File::SetFileName(const std::string& path, const std::string& fileName)
{
	this->m_FileName = fileName;
	this->m_PathToFile = path;
	this->m_FileNameFullPath = path + SlashChar + fileName;
	DirectoryHelper::RemoveQuotesFromPath(this->m_PathToFile);
	DirectoryHelper::RemoveQuotesFromPath(this->m_FileNameFullPath);
}

//!		
bool File::Exists(void)
{
	FILE* fp;
	bool exists = false;
	if(this->m_FileNameFullPath.length() > 0)
	{
		fp = fopen(this->m_FileNameFullPath.c_str(), "r");
		exists = (fp != NULL);
		if(exists) fclose(fp);
	}
	return exists;
}

//!
void File::Create(void)
{
	FILE* fp;
	if(this->m_FileNameFullPath.length() > 0)
	{
		fp = fopen(this->m_FileNameFullPath.c_str(), "a");
		if(fp != NULL) fclose(fp);
	}
}

/**
* Return file name of a full filename (i.e. file name without path)
*/
std::string File::GetFilenameName(const std::string& fileName)
{
	return itksys::SystemTools::GetFilenameName(fileName);
}


/**
* Return file name without extension of a full filename
*/
std::string File::GetFilenameWithoutExtension(const std::string& fileName)
{
	return itksys::SystemTools::GetFilenameWithoutExtension(fileName);
}

//!
void File::Append(const std::string& input)
{
	FILE* fp;
	int nread = 0;
	if(this->m_FileNameFullPath.length() > 0)
	{
		try
		{
			fp = fopen(this->m_FileNameFullPath.c_str(), "a");
			nread = fwrite(input.c_str(), sizeof(char), input.length(), fp);
			if(fp != NULL) fclose(fp);
		}
		coreCatchExceptionsAddTraceAndThrowMacro(File::Append)
	}

	if(nread < 0) 
	{
		Core::Exceptions::CannotWriteFileException e("File::Append");
		e.Append("\nFile was: ");
		e.Append(this->m_FileNameFullPath.c_str());
		throw e;
	}
}

//!
void File::Write(const std::string& input)
{
	FILE* fp;
	int nread = 0;
	if(this->m_FileNameFullPath.length() > 0)
	{
		try
		{
			fp = fopen(this->m_FileNameFullPath.c_str(), "w");
			nread = fwrite(input.c_str(), sizeof(char), input.length(), fp);
			if(fp != NULL) fclose(fp);
		}
		coreCatchExceptionsAddTraceAndThrowMacro(File::Write)
	}
	if(nread < 0) 
	{
		Core::Exceptions::CannotWriteFileException e("File::Write");
		e.Append("\nFile was: ");
		e.Append(this->m_FileNameFullPath.c_str());
		throw e;
	}
}

//!
void File::Delete(void)
{
	try
	{
		remove(this->m_FileNameFullPath.c_str());
	}
	coreCatchExceptionsAddTraceAndThrowMacro(File::Delete);
}

/**
 */
std::string Core::IO::File::GetFileName() const
{
	return m_FileName;
}

/**
 */
std::string Core::IO::File::GetPathToFile() const
{
	return m_PathToFile;
}

/**
 */
std::string Core::IO::File::GetFileNameFullPath() const
{
	return m_FileNameFullPath;
}


