/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef coreProfile_H
#define coreProfile_H

#include "gmCommonObjectsWin32Header.h"
#include "coreObject.h"
#include <set>

namespace Core
{
/** 
\brief When the application starts, a user Profile is defined, containing 
Types such as: Cardiac Analysis. plug-ins also have a Profile. 
The application will only activate and display plug-ins that share at 
least one Type with the user Profile.

\sa Core::FrontEndPlugin::FrontEndPlugin
\sa Create plugins

\ingroup gmCommonObjects
\author Juan Antonio Moya
\date 20 Jun 2006
*/

class GMCOMMONOBJECTS_EXPORT Profile : public Core::SmartPointerObject
{
public:
	typedef std::set<std::string> ProfileSet;
	coreDeclareSmartPointerClassMacro(Profile, Core::SmartPointerObject);
	
	bool ProfileHasType(const std::string& userType);
	bool ProfileIsOfType(Profile::Pointer profile);
	bool operator==(Profile::Pointer profile);

	ProfileSet& GetProfileAsStringSet(void);
	bool ProfileIsASubsetOfType(Profile::Pointer profile);
	bool ProfileIsASupersetOfType(Profile::Pointer profile);
	bool ProfileSharesSomeOfType(Profile::Pointer profile);

	void AddToProfile(const std::string& userType);
	void RemoveFromProfile(const std::string& userType);
	void MergeProfiles(Profile::Pointer profile);
	void ClearProfile(void);
	void ResetProfileToDefault(void);

	std::string GetNamesAsCommaSeparedString(void);

private:
	Profile(void);
	~Profile(void);

	ProfileSet innerProfile;
	std::string defaultProfile;

	coreDeclareNoCopyConstructors(Profile);
};

} // namespace Core

#endif
