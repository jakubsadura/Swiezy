/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDirectory.h"
#include <itksys/Glob.hxx>
#include <itksys/SystemTools.hxx>

using namespace Core::IO;

//!
Directory::Directory(void) : dirName("")
{
	this->filter = Core::IO::DirEntryFilter::New();
}

//!
Directory::~Directory(void)
{
}

//!
bool Directory::Exists(void)
{
	bool found = this->dirName != "" && itksys::SystemTools::FileIsDirectory(this->dirName.c_str());
	return found;
}

//!
void Directory::Create(void)
{
	if(this->dirName != "" && !Exists())
	{
		itksys::SystemTools::MakeDirectory(this->dirName.c_str());
	}
}

//!
std::string Directory::GetDirName(void)
{
	return itksys::SystemTools::GetFilenameName(this->dirName);
}

//!
std::string Directory::GetFullPathDirName(void)
{
	return this->dirName;
}

//!
File::Pointer Directory::GetFile(const std::string& fileName)
{
	Core::IO::File::Pointer file;
	if(CheckFileExists(fileName)) 
	{
		file = Core::IO::File::New();
		file->SetFileName(this->dirName, fileName);
	}
	return file;
}


/** Returns true if the given file or subdir exists in this directory */
bool Directory::CheckFileExists(const std::string& fileName)
{
	std::string unixDirName = this->dirName;
	std::string justTheFileName = itksys::SystemTools::GetFilenameName(fileName);
	itksys::SystemTools::ConvertToUnixSlashes(unixDirName);
	unixDirName.append("/" + justTheFileName);
	bool found = itksys::SystemTools::FileExists(unixDirName.c_str());
	return found;
}

/** Sets the full path of the directory to handle */
void Directory::SetDirNameFullPath(const std::string& dirName)
{
	this->dirName = DirectoryHelper::ConvertToOutputPath(dirName);
}


FileNameList Directory::GetContents(void)
{
	itksys::Glob glob;
	GlobExprList globList;
	GlobExprList::iterator it;
	std::string fileName;
	bool isSubdir = false;
	bool isFile = false;
	FileNameList outFileList;
	FileNameList tempOutFileList;
	FileNameList::iterator itCopy;
	std::string dirPath = this->dirName;
	std::string expr;
	std::string baseExpression;

	
	itksys::SystemTools::ConvertToUnixSlashes(dirPath);

	if(!this->filter->HasGlobbingExpression())
	{
		expr = "*";
		this->filter->AddGlobbingExpression(expr);
	}
	// Get files using the filter
	glob.RecurseOff();
	globList = this->filter->GetGlobbingExpressions();
	for(it = globList.begin(); it != globList.end(); ++it)
	{
		baseExpression = dirPath + "/" + (*it);
		if(glob.FindFiles(baseExpression))
		{
			tempOutFileList = glob.GetFiles();
			// Copy them to the output
			for(itCopy = tempOutFileList.begin(); itCopy != tempOutFileList.end(); ++itCopy)
			{
				// filter active, check just files, just dirs or no filtering
				switch(this->filter->GetMode())
				{
				case DirEntryFilter::FilesOnly:
					isFile = itksys::SystemTools::FileExists((*itCopy).c_str());
					isSubdir = itksys::SystemTools::FileIsDirectory((*itCopy).c_str());
					if(isFile && !isSubdir) outFileList.push_back(*itCopy);
					break;

				case DirEntryFilter::SubdirsOnly:
					isSubdir = itksys::SystemTools::FileIsDirectory((*itCopy).c_str());
					if(isSubdir) outFileList.push_back(*itCopy);
					break;

				case DirEntryFilter::AllEntries:
				default:
					// no filtering
					outFileList.push_back(*itCopy);
					break;
				}
			}
		}
	}

	return outFileList;
}

//!
void Directory::SetFilter(DirEntryFilter::Pointer filter)
{
	this->filter = filter;
}

//!
DirEntryFilter::Pointer Directory::GetFilter(void)
{
	return this->filter;
}





// --- Directory Helper functions ----------------------------------------------------------------------


/** Convert all backwards slashes '\' to forward slashes '/' in a path variable given by the input argument str */
std::string DirectoryHelper::ConvertToOutputPath(const std::string& str)
{
	std::string dir = itksys::SystemTools::ConvertToOutputPath(str.c_str());
	DirectoryHelper::RemoveQuotesFromPath(dir);
	return dir;
}

//!
std::string DirectoryHelper::GetHomePath(void)
{
	std::string home;

#if defined(WIN32) || defined(_WINDOWS_)
	std::string homeDrive;
	std::string homePath;
	itksys::SystemTools::GetEnv("HOMEDRIVE", homeDrive);
	itksys::SystemTools::GetEnv("HOMEPATH", homePath);
	home = homeDrive + homePath;
#endif

#if defined(unix) || defined(__unix) || defined(__unix__)
	itksys::SystemTools::GetEnv("HOME", home);
#endif

	return DirectoryHelper::ConvertToOutputPath(home);
}

//!
std::string DirectoryHelper::GetApplicationData(void)
{
	std::string appDataPath;

#if defined(WIN32) || defined(_WINDOWS_)
	// C:\Documents and Settings\{username}\Application Data
	itksys::SystemTools::GetEnv("APPDATA", appDataPath);
#endif

#if defined(unix) || defined(__unix) || defined(__unix__)
	itksys::SystemTools::GetEnv("HOME", appDataPath);
#endif

	return DirectoryHelper::ConvertToOutputPath(appDataPath);
}

//!
std::string DirectoryHelper::GetFilenameWithoutPath(const std::string& str)
{
	return itksys::SystemTools::GetFilenameName(str);
}

//!
void DirectoryHelper::ConvertSlashesToUnixStyle(std::string& path)
{
	itksys::SystemTools::ConvertToUnixSlashes(path);
}


#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable: 4018) // Disables signed/unsigned mismatch warning, that does not make a problem here
#endif
//!
void DirectoryHelper::RemoveQuotesFromPath(std::string& path)
{
	for(int i = 0; i < path.length(); ++i)
	{
		if(i >= 0 && path.at(i) == '\"')
		{
			path.erase(i, 1);
			--i;
		}
	}
}
#ifdef _MSC_VER
#	pragma warning(pop)
#endif

//!
std::string DirectoryHelper::GetPathSeparatorChar(void)
{
	return SlashChar;
}


