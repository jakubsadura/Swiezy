/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blTextUtils.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <stdlib.h>


/**
\brief   Text utilities
\ingroup 
\author  Federico Sukno 
\date    2007-11-23
*/
std::vector<float> blGetFloatsFromString (std::string theString, 
										  char* theTokens, 
										  unsigned int nToExtract)
{
	char *aux_ptr;
	std::vector<float> theVector;
	theVector.resize(nToExtract);
	
	char *str_auxiliar;
	str_auxiliar = new char[theString.length() + 1];
	theString.copy (str_auxiliar, theString.length(), 0);
	str_auxiliar[theString.length()] = 0;

	aux_ptr = strtok (str_auxiliar, theTokens);
	for (unsigned int j = 0; j < nToExtract; j++)
	{
		if (aux_ptr == NULL)
		{			
			theVector.resize(0);
			return theVector;
		}
		theVector[j] = float( atof (aux_ptr) );		
		aux_ptr = strtok (NULL, theTokens);
	}
	delete[] str_auxiliar;
	return theVector;	
}

std::string blTimeString ( double nSeconds )
{
	std::stringstream outString;
	if (nSeconds < 0)
	{
		outString << "0.00";		
		return outString.str();
	}

	unsigned int DD, hh, mm, ss;
	std::stringstream s_mm, s_ss;
	double resto;

	DD = (unsigned int) (nSeconds / (24 * 3600));
	resto = nSeconds - DD * (24 * 3600);
	hh = (unsigned int) (resto / 3600);
	resto -= hh * 3600;
	mm = (unsigned int) (resto / 60);
	resto -= mm * 60;
	ss = (unsigned int) resto;
	resto -= ss;

	if (mm < 10)
	{
		s_mm << "0";
	}
	s_mm << mm;

	if (ss < 10)
	{
		s_ss << "0";
	}
	s_ss << ss;

	if (DD)
	{
		outString << DD;
		outString << "d ";
		outString << hh; 
		outString << ":";
		outString << s_mm.str();
		outString << ":";
		outString << s_ss.str();
	}
	else
	{
		if (nSeconds > 59)
		{
			outString << hh; 
			outString << ":";
			outString << s_mm.str();
			outString << ":";
			outString << s_ss.str();
		}
		else
		{
			unsigned int aux = (int)(100 * resto);
			outString << ss << "." << aux << "s";
		}
	}	

	return outString.str();
}

std::string AddFileNumber( const std::string& fileName, int iNumber )
{
	std::string strFileName;

	// If there's more than one image -> Create new name for each one
	size_t pos = fileName.rfind( "." );
	std::string baseName = fileName.substr( 0, pos );
	std::string extName = fileName.substr( pos, fileName.size() );

	std::ostringstream currentFileName;
	currentFileName.fill( '0' );
	currentFileName << baseName << std::setw(2) << iNumber << extName;
	strFileName = currentFileName.str();

	return strFileName;
}


blTextProgressBar::blTextProgressBar ()
{
	this->minValue = 0;
	this->maxValue = 1;
	this->currentValue = 0;
	this->currentStep = 0;
	this->stepIncs = 0;

	this->barLength = 79;
	this->displayCharacter = 178;
	this->backgroundCharacter = 179;
}

blTextProgressBar::~blTextProgressBar ()
{
}


// increment is in the bar range (min to max)
void blTextProgressBar::Add ( float increment )
{
	// Normalize to 0-1 range
	float x = (increment - this->minValue) / (this->maxValue - this->minValue);

	// increment and update
	this->SetStatus (x + this->currentValue);	
}

// ---------------------------------------------------------------------
// 'completed' is in the same range 0-1
void blTextProgressBar::SetStatus ( float completed )
{	
	if (completed < this->currentValue)
	{
		return;
	}
	if (completed > 1)
	{
		completed = 1;
	}
	
	while (this->barLength * completed - this->currentStep >= 1)
		// && this->currentStep <= this->barLength )
	{
		this->currentStep++;
		this->stepIncs++;
	}

	this->currentValue = completed;
	this->Draw();
}



 
// ---------------------------------------------------------------------
// Displays the progress bar status on the screen
// Each time the work completed is enough to increment one step of
// the bar, a DispCh character is printed to the screen
// ---------------------------------------------------------------------
void blTextProgressBar::Draw ()
{
	while (this->stepIncs > 0) {
		std::cout << this->displayCharacter;
		this->stepIncs--;
	}
}

void blTextProgressBar::DrawBackground ()
{
	unsigned int i = this->barLength;
	while (i > 0) {
		std::cout << this->backgroundCharacter;
		i--;
	}
	
	while (i < this->barLength) {
		std::cout << "\b";
		i++;
	}
}

void blTextUtils::ConvertPath(std::string& path)
{
	while(path.find("\\") != std::string::npos)
	{
		path.replace(path.find("\\"), 1, "/");	
	}
}

std::string blTextUtils::GetFilenameLastExtension(const std::string& filename)
{
	std::string::size_type dot_pos = filename.rfind(".");
	if(dot_pos != std::string::npos)
	{
		return filename.substr(dot_pos);
	}
	else
	{
		return "";
	}
}

void blTextUtils::ParseLine( const std::string &_line, char delimiter, std::list<std::string> &data )
{
	std::string line = _line;
	// now bust the line up into individual words
	while(line != "")
	{
		size_t found;
		found = line.find(delimiter);
		if(found!=std::string::npos)
		{
			data.push_back(line.substr(0,found));
			line = line.substr(found+1);
		}
		else
		{
			data.push_back(line);
			line = "";
		}
	}
}


void blTextUtils::StrSub(std::string& cp,std::string for_this,std::string sub_this)
{
	int loc = 0;
	if (cp.empty())
	{
		cp = sub_this;
		return;
	}
	while( true )
	{
		loc = cp.find( for_this, loc);
		if (loc >= 0) 
		{
			cp.replace(loc,for_this.length(),sub_this);
			loc += sub_this.length();
		}
		else return;
	}
}

std::string blTextUtils::format_arg_list(const char *fmt, va_list args)
{
	if (!fmt) return "";
	int   result = -1, length = 256;
	char *buffer = 0;
	while (result == -1)
	{
		if (buffer) delete [] buffer;
		buffer = new char [length + 1];
		memset(buffer, 0, length + 1);
#ifdef _WIN32
		result = _vsnprintf(buffer, length, fmt, args);
#else
		result = vsnprintf(buffer, length, fmt, args);
#endif
		length *= 2;
	}
	std::string s(buffer);
	delete [] buffer;
	return s;
}

std::string blTextUtils::format(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	std::string s = format_arg_list(fmt, args);
	va_end(args);
	return s;
}
