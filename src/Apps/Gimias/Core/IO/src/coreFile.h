/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreFile_H
#define coreFile_H

#include "gmIOWin32Header.h"
#include "coreObject.h"
#include <string>

namespace Core
{
namespace IO
{
/**
\brief The File class hides the complexity of working with files, using a 
platform independent approach.

\sa Core::IO::Directory
\ingroup gmIO
\author Juan Antonio Moya
\date 21 Feb 2007
*/
class GMIO_EXPORT File : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(File, Core::SmartPointerObject)
			
	void SetFileName(const std::string& path, const std::string& fileName);
	void SetFileNameFullPath(const std::string& fullPath);
	
	bool Exists(void);
	void Create(void);
	void Append(const std::string& input);
	void Write(const std::string& input);
	void Delete(void);

	std::string GetFileName() const;
	std::string GetPathToFile() const;
	std::string GetFileNameFullPath() const;

	static std::string GetFilenameWithoutExtension(const std::string& fileName);
	static std::string GetFilenameName(const std::string& fileName);

protected:
	File(void);
	~File(void);

private:
	std::string m_FileName;
	std::string m_PathToFile;
	std::string m_FileNameFullPath;
};
}
}

#endif

