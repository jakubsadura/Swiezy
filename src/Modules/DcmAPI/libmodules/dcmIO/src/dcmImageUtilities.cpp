/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// dcmAPI
#include "dcmImageUtilities.h"
#include "dcmImageUtilities.txx"

// Baselib
#include <blLogger.h>
#include <blITKImagesDefines.h>
#include <blITKImageUtils.h>
#include <blImageUtils.h>

using namespace dcmAPI;

ImageUtilities::ImageUtilities()
{
}

ImageUtilities::~ImageUtilities()
{
}

/**
* \brief Read the Pixel Type of the image.
*
* Possible values are:
* - 8U unsigned 8 bit,
* - 8S signed 8 bit,
* - 16U unsigned 16 bit,
* - 16S signed 16 bit,
* - 32U unsigned 32 bit,
* - 32S signed 32 bit,
* - 32F float 32 bit,
* - 64D double 64 bit
*/         
std::string ImageUtilities::ReadPixelType(std::string path)
{
	std::string retPixelType;

	//use GDCMImageIO to obtain pizel type (which also take into account Rescale Intersect and Slope)
	itk::GDCMImageIO::Pointer gdcmImageIO = itk::GDCMImageIO::New();
	gdcmImageIO->SetFileName(path);
	gdcmImageIO->ReadImageInformation();
	itk::ImageIOBase::IOComponentType pxType = gdcmImageIO->GetComponentType();

	//convert to string
	switch (pxType)
	{
	case itk::ImageIOBase::UCHAR:
		retPixelType = "8U";
		break;
	case itk::ImageIOBase::CHAR:
		retPixelType = "8S";
		break;
	case itk::ImageIOBase::USHORT:
		retPixelType = "16U";
		break;
	case itk::ImageIOBase::SHORT:
		retPixelType = "16S";
		break;
	case itk::ImageIOBase::UINT:
		retPixelType = "32U";
		break;
	case itk::ImageIOBase::INT:
		retPixelType =  "32S";
		break;
	case itk::ImageIOBase::FLOAT:
		retPixelType = "32F";
		break;
	case itk::ImageIOBase::DOUBLE:
		retPixelType = "64D";
		break;
	case itk::ImageIOBase::UNKNOWNCOMPONENTTYPE:
	default:
		retPixelType = "Unknown type";
		break;
	}

	LOG4CPLUS_DEBUG(blLogger::getInstance("dcmapi.ImageUtilities"), "Pixel type: " << retPixelType);
	
	return retPixelType;
}

void ImageUtilities::StringToDouble(const char * string, double * array, 
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


//!
itk::ImageIOBase::IOComponentType ImageUtilities::ParsePixelType(std::string type)
{	

	if( type == "8U")
	{
		return itk::ImageIOBase::UCHAR;
	}
	else if( type == "8S")
	{
		return itk::ImageIOBase::CHAR;
	}
	else if( type == "16U")
	{
		return itk::ImageIOBase::USHORT;
	}
	else if( type == "16S")
	{
		return itk::ImageIOBase::SHORT;
	}
	else if( type == "32U")
	{
		return itk::ImageIOBase::UINT;
	}
	else if( type == "32S")
	{
		return itk::ImageIOBase::INT;
	}
	else if( type == "32F")
	{
		return itk::ImageIOBase::FLOAT;
	}
	else if ( type == "64D" )
	{
		return itk::ImageIOBase::DOUBLE;
	}
	else if ( type == "Unknown type" )
	{
		return itk::ImageIOBase::UNKNOWNCOMPONENTTYPE;
	}

	return itk::ImageIOBase::UNKNOWNCOMPONENTTYPE;
}
