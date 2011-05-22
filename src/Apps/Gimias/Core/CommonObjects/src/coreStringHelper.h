/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreStringHelper_H
#define coreStringHelper_H

#include "gmCommonObjectsWin32Header.h"
#include "coreCommonMacros.h"
#include <string>
#include <vector>

namespace Core
{

typedef std::vector<std::string> StringList;

/**
\brief A helper class for common string manipulation. It presents 
convenience functions so as to help working with strings

\ingroup gmCommonObjects
\author Juan Antonio Moya
\date 22 Jan 2008
*/
class GMCOMMONOBJECTS_EXPORT StringHelper
{
public:
	
	coreClassNameMacro(StringHelper);

	static void StripWhitespaces(std::string& str);
	static StringList Split(const std::string& str, const std::string& seps);
	static void RemoveCharacters(std::string& str, const std::string& characters);
	static std::string ToLowerCase(const std::string& str);
	static std::string ToUpperCase(const std::string& str);
	static bool CompareStrings(const std::string &str1,const std::string &str2);

	//! Try to set locale for "en_US" and other types
	static void setLocale( );

	//! Restore locale
	static void restoreLocale();

	//!
	static StringList m_locales;
	static std::string m_oldLocale;
};

}

#endif //coreStringHelper_H
