/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <cstring>
#include "blStringTokenizer.h"

/**
\brief   This class defines tokenizer for strings.
\ingroup 
\author  Sergio Andres Ruiz
\date    2007/04/19
*/
/**
*	The tokenizer uses the default delimiter set, which is " \t\n\r\f"
*/
blStringTokenizer::blStringTokenizer():	
	numberTokens(0), nextToken(0), delim(" \t\n\r\f")
{
}

/**
 *	The tokenizer uses the default delimiter set, which is " \t\n\r\f"
 *	It generates tokens for the given string.
 *
 *	\param str the specified string
 */
blStringTokenizer::blStringTokenizer(const char * str):
	str(str),
	delim(" \t\n\r\f") 
{
	//this->delim = " \t\n\r\f";
	//this->str = str;
	this->numberTokens = 0;
	this->nextToken = 0;
	this->GetTokens();
}

/**
 * The characters in the delim argument are the delimiters 
 * for separating tokens.
 * It generates tokens for the given string.
 *
 * \param str the specified string
 * \param delim a set of delimiters
 */
blStringTokenizer::blStringTokenizer(const char * str, 
									 const char * delim):
	str(str),
	delim(delim) 
{
	//this->delim = delim;
	//this->str = str;
	this->numberTokens = 0;
	this->nextToken = 0;
	this->GetTokens();
}

/**
 *	This method is called when an blStringTokenizer is deleted.
 */
blStringTokenizer::~blStringTokenizer()
{
}

/**
 *	This method goes through the string and get the tokens
 */
void blStringTokenizer::GetTokens() 
{

	// initialize member vars.
	this->nextToken = 0;
	this->numberTokens = 0;
	this->tokens.clear();	// remove previous tokens

	// copy input string into buffer
	char * buffer = new char[this->str.length() + 1];
	strcpy(buffer, this->str.c_str());

	// call strtok with buffer
	char * result = 0;
	result = strtok(buffer, this->delim.c_str());	

	while (result)
	{
		this->tokens.push_back(result);
		this->numberTokens++;

		result = strtok(0, this->delim.c_str());
	}

	delete buffer;
}

/** 
 *	This method returns next token as a C string
 *
 *	\return a const pointer to next token
 */
const char * blStringTokenizer::NextToken() 
{ 
	std::string & strRef = this->tokens[this->nextToken++];
	return strRef.c_str();
}

/**
 *	This method sets a new string to be tokenized and 
 *	generates tokens.
 *
 *	\param string input string
 */
void blStringTokenizer::Tokenize(const char * str)
{
	this->str = str;
	this->GetTokens();
}

/**
 *	This method sets a new string to be tokenized and 
 *	generates tokens.
 *
 *	\param string input string
 *	\param length number of chars to be considered
 */
void blStringTokenizer::Tokenize(const char * str, unsigned int length)
{
	this->str.assign(str, length);
	this->GetTokens();
}
