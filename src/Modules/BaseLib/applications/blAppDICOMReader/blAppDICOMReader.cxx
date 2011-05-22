/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "itkOrientedImage.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
#include "itkOrientImageFilter.h"

/**
The following example illustrates how to use ITK functionalities in order
to read a DICOM series into a volume and then save this volume in another
file format.
\note Took as a starting point DicomSeriesReadImageWrite2.cxx
*/
int main( int argc, char* argv[] )
{

	if( argc < 4 )
	{
		std::cerr << "Usage: " << std::endl;
		std::cerr << argv[0] << " DicomDirectory  outputFileName outputFileNameOriented [seriesName]" 
			<< std::endl;
		return EXIT_FAILURE;
	}

	typedef signed short    PixelType;
	const unsigned int      Dimension = 3;
	typedef itk::OrientedImage< PixelType, Dimension >         ImageType;


	typedef itk::ImageSeriesReader< ImageType >        ReaderType;
	ReaderType::Pointer reader = ReaderType::New();


	typedef itk::GDCMImageIO       ImageIOType;
	ImageIOType::Pointer dicomIO = ImageIOType::New();

	reader->SetImageIO( dicomIO );


	typedef itk::GDCMSeriesFileNames NamesGeneratorType;
	NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();

	nameGenerator->SetUseSeriesDetails( true );
	nameGenerator->AddSeriesRestriction("0008|0021" );

	nameGenerator->SetDirectory( argv[1] );

	try
	{
		std::cout << std::endl << "The directory: " << std::endl;
		std::cout << std::endl << argv[1] << std::endl << std::endl;
		std::cout << "Contains the following DICOM Series: ";
		std::cout << std::endl << std::endl;
		typedef std::vector< std::string >    SeriesIdContainer;

		const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();

		SeriesIdContainer::const_iterator seriesItr = seriesUID.begin();
		SeriesIdContainer::const_iterator seriesEnd = seriesUID.end();
		while( seriesItr != seriesEnd )
		{
			std::cout << seriesItr->c_str() << std::endl;
			seriesItr++;
		}
		std::string seriesIdentifier;

		if( argc > 4 ) // If no optional series identifier
		{
			seriesIdentifier = argv[3];
		}
		else
		{
			seriesIdentifier = seriesUID.begin()->c_str();
		}
		// Software Guide : EndCodeSnippet


		std::cout << std::endl << std::endl;
		std::cout << "Now reading series: " << std::endl << std::endl;
		std::cout << seriesIdentifier << std::endl;
		std::cout << std::endl << std::endl;

		typedef std::vector< std::string >   FileNamesContainer;
		FileNamesContainer fileNames;

		fileNames = nameGenerator->GetFileNames( seriesIdentifier );
		reader->SetFileNames( fileNames );
		try
		{
			reader->Update();
		}
		catch (itk::ExceptionObject &ex)
		{
			std::cout << ex << std::endl;
			return EXIT_FAILURE;
		}


		typedef itk::ImageFileWriter< ImageType > WriterType;
		WriterType::Pointer writer = WriterType::New();

		writer->SetFileName( argv[2] );

		writer->SetInput( reader->GetOutput() );
		// Software Guide : EndCodeSnippet    

		std::cout  << "Writing the image as " << std::endl << std::endl;
		std::cout  << argv[2] << std::endl << std::endl;
		try
		{
			// Software Guide : BeginCodeSnippet    
			writer->Update();
			// Software Guide : EndCodeSnippet    
		}
		catch (itk::ExceptionObject &ex)
		{
			std::cout << ex << std::endl;
			return EXIT_FAILURE;
		}




		// Change orientation to standard LPS 
		// itk orientation names are the oposite of DICOM names, so it
		// will be in DICOM: RAI
		itk::OrientImageFilter<ImageType,ImageType>::Pointer orienter =
			itk::OrientImageFilter<ImageType,ImageType>::New();
		orienter->UseImageDirectionOn();
		orienter->SetDesiredCoordinateOrientation(
			itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_LPI);
		orienter->SetInput( reader->GetOutput() );

		writer->SetInput( orienter->GetOutput() );
		writer->SetFileName( argv[3] );
		writer->Update();

	}
	catch (itk::ExceptionObject &ex)
	{
		std::cout << ex << std::endl;
		return EXIT_FAILURE;
	}


	return 0;
}




