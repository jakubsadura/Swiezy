/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <string.h>

#include "coreStringHelper.h"
#include "coreAssert.h"
#include <boost/foreach.hpp>

using namespace Core;

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable: 4018) // Disables signed/unsigned mismatch warning, that does not make a problem here
#endif

StringList StringHelper::m_locales;
std::string StringHelper::m_oldLocale;

/**
Removes all undesirable whitespaces. It will remove all whitespaces at the beggining and at the end of the string,
and any other whitespaces that appear more than once in a row.
*/
void StringHelper::StripWhitespaces(std::string& str)
{
	// Erase at the beggining
	size_t k = 0;
	while(k < str.size() && (str[k] == ' ' || str[k] == '\t'))
		k++;
	str.erase(0, k);
	
	// Erase at the end
	k = str.size()-1;
	while(k >= 0 && (str[k] == ' ' || str[k] == '\t'))
		k--;

	str.erase(k+1, str.size()+1-k);

	// Erase duplicated whitespaces
	k = 0;
	while(k < str.size())
	{
		if((str[k] == ' ' || str[k] == '\t') && k+1 < str.size() && (str[k+1] == ' ' || str[k+1] == '\t'))
			str.erase(k, 1);
		else
			k++;
	}
}

#ifdef _MSC_VER
#	pragma warning(pop)
#endif


/** 
Splits a string when it finds a one of the given set of character that act as separators, and returns
the list of substrings.
\param str is the string to split. It will not be modified
\param seps are a set of characters where to split the string. 
These characters will be removed also from the set of returned substrings, so they are actually stripped.
\return a string list, being a std::vector of strings.
*/
Core::StringList StringHelper::Split(const std::string& str, const std::string& seps)
{
	coreAssertMacro(seps != "" && "The split function expected at least one separator as parameter");
	StringList list;
	list.reserve(4);
	char* chrStr = new char[str.size()];
	strcpy(chrStr, str.c_str());
	char* ptr = strtok(chrStr, seps.c_str());
	while(ptr != NULL)
	{
		std::string res(ptr);
		list.push_back(res);
		ptr = strtok(NULL, seps.c_str());
	}
	return list;
}


/**
Removes all undesirable characters. It will remove from the string all characters passed as parameter.
*/
void StringHelper::RemoveCharacters(std::string& str, const std::string& characters)
{
	for(unsigned j = 0; j < characters.size(); ++j)
	{
		for(unsigned i = 0; i < str.size(); ++i)
		{
			if(str[i] == characters[j])
			{
				str.erase(i, 1);
			}
		}
	}
}

/**
Converts all chars to lowercase
*/
std::string StringHelper::ToLowerCase(const std::string& str)
{
	std::string aux = "";
	for(unsigned i = 0; i < str.size(); ++i)
		aux.append(1, tolower(str[i]));
	return aux;
}

/**
Converts all chars to uppercase
*/
std::string StringHelper::ToUpperCase(const std::string& str)
{
	std::string aux;
	for(unsigned i = 0; i < str.size(); ++i)
		aux.append(1, toupper(str[i]));
	return aux;
}


bool Core::StringHelper::CompareStrings( const std::string &str1,const std::string &str2 )
{
	return strcmp( str1.c_str( ), str2.c_str( ) ) < 0; 
}

void Core::StringHelper::setLocale()
{
	if ( m_locales.empty() )
	{
		m_locales.push_back("en_US");
		m_locales.push_back("en_US.UTF-8");
		m_locales.push_back("en_US.ISO-8859-1");
	}

	m_oldLocale = setlocale(LC_ALL, NULL);

	char *result = NULL;
	BOOST_FOREACH(std::string locale, m_locales) {
		result = setlocale(LC_ALL, locale.c_str());
		if (result != NULL) {
			//std::cout << "!!!!! Set locale to: '" << locale << "'" << std::endl;
			break;
		}
	}
	/*struct lconv * currentlocale = localeconv();
	std::cout << "decimal_point='" << currentlocale->decimal_point << "'" << std::endl;*/
}

void Core::StringHelper::restoreLocale()
{
	//std::cout << "!!!!! Restore locale to: '" << m_oldLocale << "'" << std::endl;
	setlocale(LC_ALL, m_oldLocale.c_str());
}
