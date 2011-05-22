/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef coreDirectory_H
#define coreDirectory_H

#include "coreObject.h"
#include "coreFile.h"
#include "coreDirEntryFilter.h"
#include <string>
#include <vector>

namespace Core
{
// forward declaration
namespace Runtime { class Environment; }

namespace IO
{
// Define the slash character that separes the directories in the current 
// OS filesystem signature
#if defined(WIN32) || defined(_WINDOWS_)
	static const std::string SlashChar = "\\";
#endif

#if defined(unix) || defined(__unix) || defined(__unix__)
	static const std::string SlashChar = "/";
#endif

typedef std::vector<std::string> FileNameList;

/**
\brief The Directory class hides the complexity of working with directories, 
in platform independent way. A portable directory browser for windows 
and unix filesystems.

This class is provided as interface for the OS-dependant implementation 
of the Directory handler, for controlling and accessing a single directory.

\ingroup gmIO
\author Juan Antonio Moya
\date 20 Jan 2007
*/
class GMIO_EXPORT Directory : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(Directory, Core::SmartPointerObject)

	virtual bool Exists(void);
	virtual void Create(void);
	virtual FileNameList GetContents(void);

	virtual void SetFilter(DirEntryFilter::Pointer filter);
	virtual DirEntryFilter::Pointer GetFilter(void);
	virtual bool CheckFileExists(const std::string& fileName);
	virtual void SetDirNameFullPath(const std::string& dirName);
	virtual File::Pointer GetFile(const std::string& fileName);
	
	virtual std::string GetDirName(void);
	virtual std::string GetFullPathDirName(void);
	
protected:
	Directory(void);
	~Directory(void);

private:
	std::string dirName;
	DirEntryFilter::Pointer filter;

	coreDeclareNoCopyConstructors(Directory);

};


/**
The Directory class hides the complexity of working with directories, in platform
independent way. A portable directory browser for windows and unix filesystems.

This class is provided as interface for the OS-dependant implementation of 
the Directory handler, for global directory functions and other helper 
functions. It also allows toset and get the current working dir of the
application runtime.

\ingroup CoreIO
\author Juan Antonio Moya
\date 20 Jan 2007
*/
class GMIO_EXPORT DirectoryHelper : public Core::Object
{
public:
	coreClassNameMacro(DirectoryHelper);
	
	static std::string ConvertToOutputPath(const std::string& str);
	static void ConvertSlashesToUnixStyle(std::string& path);
	static void RemoveQuotesFromPath(std::string& path);
	static std::string GetFilenameWithoutPath(const std::string& str);
	static std::string GetHomePath(void);
	static std::string GetApplicationData( );
	static std::string GetPathSeparatorChar(void);

private:
	// Allow only to change the working dir from the application enviroment manager
	friend class Core::Runtime::Environment;
};

}
}

#endif

