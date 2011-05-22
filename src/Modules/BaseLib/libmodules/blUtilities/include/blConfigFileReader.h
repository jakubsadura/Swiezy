/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blConfigFileReader_h
#define _blConfigFileReader_h

#include <map>
#include <string>
#include <vector>

//#define DEBUG_MESSAGES_blConfigFileReader

const int CONFIG_FILE_MAX_LINE_LENGTH = 255;
const char CONFIG_FILE_DELIMITERS[] = " \t";


/**
 * \brief This class reads values from a given file.
 * \ingroup blUtilities
 *
 *	Each line of the config file will contain the name of a parameter 
 *	followed by the values associated to it.
 *
 *	paramName value1 value2 ...
 *
 *	params and values are delimited by blanks
 */
class blConfigFileReader
{

public:

	/**\brief type of vector of strings*/
	typedef std::vector<std::string> StringVectorType;

	/**\brief type of vector of int*/
	typedef std::vector<int> IntVectorType;

	/**\brief type of vector of double*/
	typedef std::vector<double> DoubleVectorType;


	/**\brief Constructor*/
	blConfigFileReader();

	/**\brief Destructor*/
	~blConfigFileReader();

	// fsukno 20040415
	/**\brief Last error code generated*/
	int GetLastError ()
	{
		return this->lastErrorCode;
	};

	/**\brief Reset last error code to zero*/
	void ResetLastError ()
	{
		this->lastErrorCode = 0;
	}

	/**\brief Returns the filename */
	const char * GetFilename()
	{		
		return this->filename;
	};

	/**
	 * \brief Returns the value of a param
	 * \param [in] bRemoveLastCRCharacter Search the last CR character '\r'
	 * and if it's found, erase it from the string
	 */
	std::string GetValue(
						const char * paramName, 
						std::string defaultValue = "",
						bool bRemoveLastCRCharacter = false );

	/**\brief Returns the value of a param*/
	bool GetValueAsBool(const char * paramName, bool defaultValue = false);

	/**\brief Returns the value of a param*/
	int GetValueAsInt(const char * paramName, int defaultValue = 0);

	/**\brief Returns the value of a param*/
	double GetValueAsDouble(const char * paramName, double defaultValue = 0);

	/**\brief Returns the values of a param*/
	StringVectorType GetVectorOfValues(const char * paramName);

	/**\brief Returns the values of a param*/
	IntVectorType GetVectorOfIntValues(const char * paramName);

	/**\brief Returns the values of a param*/
	DoubleVectorType GetVectorOfDoubleValues(const char * paramName);

	/**\brief Sets the filename */
	void SetFilename(const char * filename);

	/**\brief Reads from the file*/
	void Update();

	

private:
	char * filename;
	int lastErrorCode;  
	// Last error inficator: 
	//	0:	No errors
	//	-1:	Could not open file

	typedef std::map<std::string, StringVectorType> MapType;

	typedef MapType::iterator MapIteratorType;

	MapType paramsMap;
	
};

#endif
