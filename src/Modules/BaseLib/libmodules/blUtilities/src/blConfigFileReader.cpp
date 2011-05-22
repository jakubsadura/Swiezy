/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blConfigFileReader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstring>
#include "CILabAssertMacros.h"

/**
*	Constructor
*/
blConfigFileReader::blConfigFileReader()
{
	this->filename = 0;
	this->lastErrorCode = 0;
}


/**
*	Destructor
*/
blConfigFileReader::~blConfigFileReader()
{
	if (this->filename)
		delete [] this->filename;
}



/**
*	Sets the file to be read
*/
void blConfigFileReader::SetFilename(const char * filename)
{
	if (this->filename)
	{
		delete [] this->filename;
	}

	this->filename = new char[strlen(filename) + 1];

	strcpy(this->filename, filename);

}



/**
*	Reads from the specified file and extract parameters and its values
*/
void blConfigFileReader::Update()
{

	char buffer[CONFIG_FILE_MAX_LINE_LENGTH];

	StringVectorType multiString;	// stores n strings


	// open the file for reading
	std::ifstream file(this->filename, std::ios::in);

	if (!file)	// error
	{
		this->lastErrorCode = -1;
		//std::cerr << "Error: File " << this->filename << 
		//							" not found" << std::endl;
	}
	else
	{
		// read a new line while it is possible
		while (file.getline(buffer, CONFIG_FILE_MAX_LINE_LENGTH))
		{			

			multiString.clear();	

			std::string lineString;
			std::string tokenString;
			std::string paramString("");
			std::string valueString("");


			// tokens separated by blanks --> paramName Value			
			char * token;

			// ----------------------
			// remove comments
			// ----------------------
			lineString = buffer;

			//TODO::INT->UNSIGNED INT
			// 			int position = lineString.find_first_of('#');	
			// //////////////////////////
			unsigned int position = lineString.find_first_of('#');	
			// first occurrence of char #

			if (position == std::string::npos)
			{
				// not found 
			}

			else if (position > 0)
			{
				// extract token before #
				lineString = lineString.substr(0, position);
			}
			else if (position == 0)
			{
				// whole line is a comment line
				lineString = "";
			}



			// ----------------------
			// extract tokens from lineString
			// ----------------------
			cilabAssertMacro(CONFIG_FILE_MAX_LINE_LENGTH > (1 + lineString.size()) );
			strcpy(buffer, lineString.c_str());

			token = strtok(buffer, CONFIG_FILE_DELIMITERS);
			int tokenIndex = 0;

			while (token)
			{

#ifdef DEBUG_MESSAGES_blConfigFileReader
				std::cout << "Token: " << token << std::endl;
#endif
				// if comment not in
				if (tokenIndex == 0)
				{
					paramString = token;
				}
				else 
				{
					valueString = token;
					multiString.push_back(valueString);
				}

				token = strtok(0, CONFIG_FILE_DELIMITERS);
				tokenIndex++;
			}

			// end of line


			// if number of tokens is at least 2 (paramName and 1 valueString)
			if (tokenIndex > 1)
			{
				this->paramsMap[paramString] = multiString;	
			}
		}
	}
}


/**
*	Returns the value for a given parameter
*	If parameter does not exist, a empty string is returned
*
*	\param paramName name of the parameter
*	\return the string corresponding to this parameter
*/
std::string blConfigFileReader::GetValue(
	const char * paramName, 
	std::string defaultValue,
	bool bRemoveLastCRCharacter )
{
	std::string paramValue = defaultValue;

	MapIteratorType iter = this->paramsMap.find(paramName);

	if (iter != this->paramsMap.end())
	{
		StringVectorType & multiString = iter->second;
		if (multiString.size() > 0)
		{
			paramValue = multiString[0];
		}
	}

	if ( bRemoveLastCRCharacter )
	{
		long lPos = paramValue.rfind( '\r' );
		if ( lPos != -1 )
		{
			paramValue.erase( lPos, 1 );
		}
	}

	return paramValue;
}
/*
const char * blConfigFileReader::GetValue(const char * paramName)
{

MapIteratorType iter = this->paramsMap.find(paramName);

if (iter != this->paramsMap.end())
{
StringVectorType & multiString = iter->second;
if (multiString.size() > 0)
{
if (multiString[0].size() > 0)
{
return multiString[0].c_str();
}
else
{
return 0; // NULL
}
}
else
{
return 0; // NULL
}
}
else
{
return 0; // NULL
}
}
*/


/**
*	Returns the value (bool) for a given parameter
*	If parameter does not exist, returns false
*
*	\param paramName name of the parameter
*	\return the boolean value corresponding to this parameter
*/
bool blConfigFileReader::GetValueAsBool(const char * paramName,  bool defaultValue)
{
	///const char * valueString = this->GetValue(paramName);
	const std::string & refString = this->GetValue(paramName);
	/*
	if (valueString)
	{
	if (strcmp(valueString, "YES") == 0 || 
	strcmp(valueString, "yes") == 0 )
	{
	return true;
	}
	else
	{
	return false;
	}
	}

	else
	{
	return false;
	}
	*/
	if ((refString.compare("YES") == 0) || (refString.compare("yes") == 0) )
	{
		return true;
	}
	else if ((refString.compare("NO") == 0) || (refString.compare("no") == 0) )
	{
		return false;
	}
	else return defaultValue;
}


/**
*	Returns the value (int) for a given parameter
*	If parameter does not exist, returns 0
*
*	\param paramName name of the parameter
*	\return the int value corresponding to this parameter
*/
int blConfigFileReader::GetValueAsInt(const char * paramName, int defaultValue)
{
	/*
	const char * valueString = this->GetValue(paramName);

	if (valueString)
	{
	return atoi(valueString);
	}
	else
	{
	return 0;
	}
	*/

	const std::string & refString = this->GetValue(paramName);
	if (refString.size() > 0)
	{
		return atoi(refString.c_str());
	}
	else
	{
		return defaultValue;
	}
}



/**
*	Returns the value (double) for a given parameter
*	If parameter does not exist, returns 0
*
*	\param paramName name of the parameter
*	\return the double value corresponding to this parameter
*/
double blConfigFileReader::GetValueAsDouble(const char * paramName, double defaultValue)
{
	/*
	const char * valueString = this->GetValue(paramName);

	if (valueString)
	{
	return atof(valueString);
	}
	else
	{
	return 0;
	}
	*/
	const std::string & refString = this->GetValue(paramName);
	if (refString.size() > 0)
	{
		return atof(refString.c_str());
	}
	else
	{
		return defaultValue;
	}
}



/**
*	Returns a vector of values for a given parameter.
*	If parameter does not exist, an empty vector is returned
*
*	\param paramName name of the parameter
*	\return vector of strings corresponding to this parameter
*/
blConfigFileReader::StringVectorType
blConfigFileReader::GetVectorOfValues(const char * paramName)
{

	StringVectorType vector;

	MapIteratorType iter = this->paramsMap.find(paramName);

	if (iter != this->paramsMap.end())
	{
		vector = iter->second;
	}

	return vector;
}



/**
*	Returns a vector of values for a given parameter
*	If parameter does not exist, an empty vector is returned
*
*	\param paramName name of the parameter
*	\return vector of int values corresponding to this parameter
*/
blConfigFileReader::IntVectorType
blConfigFileReader::GetVectorOfIntValues(const char * paramName)
{

	IntVectorType vector;

	MapIteratorType iter = this->paramsMap.find(paramName);

	if (iter != this->paramsMap.end())
	{
		StringVectorType & stringVector = iter->second;

		StringVectorType::iterator iter = stringVector.begin();
		while(iter != stringVector.end())
		{
			std::string & str = *iter;
			vector.push_back(atoi(str.c_str()));
			iter++;
		}
	}

	return vector;
}



/**
*	Returns a vector of values for a given parameter
*	If parameter does not exist, an empty vector is returned
*
*	\param paramName name of the parameter
*	\return vector of double values corresponding to this parameter
*/
blConfigFileReader::DoubleVectorType 
blConfigFileReader::GetVectorOfDoubleValues(const char * paramName)
{

	DoubleVectorType vector;

	MapIteratorType iter = this->paramsMap.find(paramName);

	if (iter != this->paramsMap.end())
	{
		StringVectorType & stringVector = iter->second;

		StringVectorType::iterator iter = stringVector.begin();
		while(iter != stringVector.end())
		{
			std::string & str = *iter;
			vector.push_back(atof(str.c_str()));
			iter++;
		}
	}

	return vector;
}
