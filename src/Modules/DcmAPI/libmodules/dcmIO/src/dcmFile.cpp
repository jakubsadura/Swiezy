/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dcmFile.h"

//gdcm
#include "gdcmDocEntry.h"
#include "gdcmValEntry.h"
#include "gdcmUtil.h"

using namespace dcmAPI;


File::File()
:
gdcmFile(new gdcm::File())
{
}

File::~File()
{
}

bool File::Open(std::string path)
{
	bool ret = true;

	if(path.empty())
		return false;

	gdcmFile->SetFileName(path);
	if(!gdcmFile->Load())
		return false;

	return ret;
}

void File::ImageStringParametersToDouble(const char * string, double * array, 
										 unsigned int numberOfTokens)
{
	unsigned int length = strlen(string);
	char * auxString = new char[length + 1];
	strcpy(auxString, string);

	char * token = 0;
	token = strtok(auxString, "\\");

	int tokenCounter = 0;
	while (token && tokenCounter < (int) numberOfTokens)
	{
		array[tokenCounter] = atof(token);

		token = strtok(0, "\\");
		tokenCounter++;
	}
	delete[] auxString;
}

std::string File::ReadTagValue(const TagId& tagId)
{
	GdcmValEntryPtr valEntry;
	//GdcmDocEntryPtr entry; //for some reason it crashes when I use smart pointer for DocEntry
	//gdcm::DocEntry* entry = NULL;
	std::string val = CleanGdcmEntryStringValue( gdcmFile->GetEntryValue(tagId.m_group, tagId.m_element) );
	return val;
}

std::string File::CleanGdcmEntryStringValue(const std::string& value)
{
	std::string ret = value;
	gdcm::Util::CreateCleanString(ret);

	//if the last character is a blank space remove it
	int length = ret.length();
	if(length > 0)
	{
		if(ret.at(length-1) == ' ')
			ret.resize(length - 1);
	}

	return ret;
}
