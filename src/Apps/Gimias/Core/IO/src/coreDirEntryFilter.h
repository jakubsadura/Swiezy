/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDirEntryFilter_H
#define coreDirEntryFilter_H

#include "gmIOWin32Header.h"
#include "coreObject.h"
#include <list>
#include <string>

namespace Core
{
namespace IO
{
typedef std::list<std::string> GlobExprList;


/**
\brief This class helps handling globbing expressions that filter the 
directory contents of a Directory object. It allows to filter the 
directory list of a Directory object by retrieving only files or 
directories, and to apply globbing expressions like "*.cxx" or "*.vtk"

\sa Core::IO::Directory
\ingroup gmIO
\author Juan Antonio Moya
\date 21 Feb 2007
*/
class GMIO_EXPORT DirEntryFilter : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(
		DirEntryFilter, 
		Core::SmartPointerObject);

	enum FilterMode { AllEntries, SubdirsOnly, FilesOnly } ;
	
	Core::IO::DirEntryFilter::FilterMode GetMode() const;
	void SetMode(Core::IO::DirEntryFilter::FilterMode val);
	Core::IO::GlobExprList GetGlobbingExpressions() const;

	virtual bool HasGlobbingExpression(void);
	virtual void AddGlobbingExpression(const std::string& expr);
	virtual void ResetGlobbingExpressions(void);

protected:
	DirEntryFilter();
	~DirEntryFilter();

private:
	FilterMode m_Mode;
	GlobExprList m_GlobbingExpressions;
	coreDeclareNoCopyConstructors(DirEntryFilter);

};
}
}

#endif // coreDirEntryFilter_H

