/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blStringTokenizer_H
#define __blStringTokenizer_H

#include <iostream>
#include <string>
#include <vector>

/**	
* \brief This class defines a tokenizer, which can break a 
*	string into tokens
* \ingroup blCommon
*
*	This class has some delimiters by default (" \n\t\r\f"), but it is 
*	possible to specify any other set of delimiters.
*/

class blStringTokenizer 
{

public:
	/** \brief Empty Constructor */
	blStringTokenizer();
	/** \brief Constructor */
	blStringTokenizer(const char * str);
	/** \brief Constructor */
	blStringTokenizer(const char * str, const char * delim); 		
	/** \brief Destructor */
	~blStringTokenizer();
	/** \brief return the number of tokens */
	int CountTokens() const
	{ return this->numberTokens; };
	/** \brief return true if there are more tokens */
	bool HasMoreTokens() const
	{ 	return ( this->nextToken < this->numberTokens ); };
	/** \brief gets next token as a const pointer*/
	const char * NextToken();
	/** \brief sets the string to be tokenized */
	void SetDelimiters(const char * delim)
	{
		this->delim = delim;
	};
	void Tokenize(const char * str);
	void Tokenize(const char * str, unsigned int length);

private:
	unsigned int numberTokens;
	unsigned int nextToken;
	std::string str;
	std::string delim;
	std::vector<std::string> tokens;	//!< tokens in current string
	void GetTokens();
};

#endif
