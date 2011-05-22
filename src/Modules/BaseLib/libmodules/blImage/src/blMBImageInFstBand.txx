/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//Multi-Band image, original image is loaded into the first band

#ifndef _blMBImageInFstBand_txx
#define _blMBImageInFstBand_txx

#include "blMBImageInFstBand.h"
#include <blImageReader.h>
#include <blImageWriter.h>
#include <blIAMultiResolutionFilter.h>

/**
 * Constructor
 */
//--------------------------------------------------
template <class TImage, unsigned int NBands>
blMBImageInFstBand<TImage, NBands>::blMBImageInFstBand()
{ 
};



/**
 *	Destructor
 */
//--------------------------------------------------
template <class TImage, unsigned int NBands>
blMBImageInFstBand<TImage, NBands>::~blMBImageInFstBand() 
//--------------------------------------------------
{ 
};



template <class TImage, unsigned int NBands>
void blMBImageInFstBand<TImage, NBands>::LoadImageFromFilename( const char* filename )
{
	typedef blImageReader<TImage> ImageReaderType;

	typename ImageReaderType::Pointer reader = ImageReaderType::New();
	reader->SetFileName( filename );
	reader->Update();
	
	if( this->GetResolution() == 1 )
	{
        SetBand( 0, reader->GetOutput(), true, false );
	}
	else //rescale the image
	{
		typedef blIAMultiResolutionFilter<TImage, TImage> MResFilterType;
		typedef typename MResFilterType::ScheduleType  ScheduleType;

		ScheduleType schedule( 1, TImage::ImageDimension );
		for( int c = 0; c<schedule.cols(); c++ )
		{
			schedule[0][c] = this->GetResolution();
		}

		typename MResFilterType::Pointer mresFilter = MResFilterType::New();
		mresFilter->SetNumberOfLevels(1); //only one level used
		mresFilter->SetSchedule( schedule );
		mresFilter->SetInput( reader->GetOutput() );
		mresFilter->Update();

		SetBand(0, mresFilter->GetOutput(0), true, false );
	}

	//set the rest of bands to equal size
	for( int i = 0; i < this->GetNumberOfBands(); i++ )
		this->GetBand(i)->SetRegions( this->GetBand(0)->GetLargestPossibleRegion() );
};

template <class TImage, unsigned int NBands>
void blMBImageInFstBand<TImage, NBands>::SaveImage( const char* filename ) 
{
	typedef blImageWriter<TImage> ImageFileWriterType;

	typename ImageFileWriterType::Pointer writer = ImageFileWriterType::New();
	writer->SetFileName( filename );
	writer->SetInput( this->GetBand(0) );
	writer->Update();
};



#endif


