/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDRRVisualizationProcessor.h"
#include "coreDataContainer.h"
#include "coreDataEntityHolder.h"
#include "coreDataEntityList.h"
#include "coreDataEntityUtilities.h"
#include "coreLoggerHelper.h"
#include "coreDataEntityHelper.h"

#include "vtkImageData.h"

//itk
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageMaskSpatialObject.h"
#include "itkRayCastInterpolateImageFunction.h"
#include "itkImageFileWriter.h"
#include "itkImageSeriesReader.h"

using namespace Core;

//!
DRRVisualizationProcessor::DRRVisualizationProcessor(void)
{
	m_Params.reset( new Core::DRRParams);

#ifdef _USE_MVIEWLIB
	SetName( "Radiograph Visualization (MViewLib)" );
#else
	SetName( "Radiograph Visualization (ITK)" );
#endif

	SetNumberOfInputs(1);
	GetInputPort( 0 )->SetName( "Input image" );
	GetInputPort( 0 )->SetDataEntityType( Core::ImageTypeId );

	SetNumberOfOutputs(1);
}

//!
DRRVisualizationProcessor::~DRRVisualizationProcessor(void)
{
}

/**
*/
void DRRVisualizationProcessor::Update( )
{
	if (GetInputDataEntity(0).IsNull())
		return;

	if ( GetInputDataEntity(0)->GetType() != Core::ImageTypeId )
	{
		throw Core::Exceptions::Exception(
			"DRRVisualizationProcessor::Update",
			"You must select a Image input from the Processing browser" );
	}

//	for ( int i = 0 ; i < GetInputDataNumberOfTimeSteps( 0 ) ; i++ )
//	{

		InputImageType::Pointer inputImage;
		GetProcessingData( 0, inputImage);
		//inputImage = GenerateITKImageFromBMP( 1 );


		SetInputImage( inputImage);

#ifdef _USE_MVIEWLIB
		CreateDRRUsingMview();	
#else
		CreateDRRUsingITK();
#endif
//	}

}


/**
*/
void DRRVisualizationProcessor::CreateDRRUsingITK( )
{
	m_Params->rx = -90;
	m_Params->ry = 0;
	m_Params->spacing[0]=0.3;
	m_Params->spacing[1]=0.3;

	const double dtr = ( atan(1.0) * 4.0 ) / 180.0;

	SetUpImage();

	FilterType::Pointer filter = FilterType::New();
	filter->SetInput( m_image );
	filter->SetDefaultPixelValue( 0 );

	TransformType::Pointer transform = TransformType::New();
	transform->SetComputeZYX(true);
	transform->SetTranslation( m_translation );
	transform->SetRotation( dtr*m_Params->rx, dtr*m_Params->ry, dtr*m_Params->rz );
	transform->SetCenter(m_center);

	InterpolatorType::Pointer interpolator = InterpolatorType::New();
	interpolator->SetTransform(transform);
	interpolator->SetThreshold(m_Params->threshold);
	interpolator->SetFocalPoint(m_focalpoint);
	interpolator->Print (std::cout);
	filter->SetInterpolator( interpolator );
	filter->SetTransform( transform );

	filter->SetSize( m_size );
	filter->SetOutputSpacing( m_spacing );
	filter->SetOutputOrigin( m_origin );

	filter->Update();

	RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
	rescaler->SetOutputMinimum(  m_Params->rangeMin );
	rescaler->SetOutputMaximum( m_Params->rangeMax );
	rescaler->SetInput( filter->GetOutput() );


	//===========================================================
	// Write image
	typedef itk::ImageFileWriter< OutputImageType >  WriterType;
	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName( "testDRR.vtk" );
	writer->SetInput( rescaler->GetOutput() );
	writer->Update();

	//UpdateOutputImageAsVtk( 0, rescaler->GetOutput(), "DRR", false, 1, GetInputDataEntity( 0 ) );

}

/**
*/
void DRRVisualizationProcessor::CreateDRRUsingMview()
{
#ifdef _USE_MVIEWLIB
	unsigned int num_volume, indexoffset;
	int startindex;
	num_volume =1;
	indexoffset = 1;
	startindex = 1;

	
	m_Params->angle1 = -4;
	m_Params->angle2 = 90;
	m_Params->translation[0] = 0.;
	m_Params->translation[1] = 0.;
	m_Params->translation[2] = 0.;
	m_Params->rangeMin = 200;
	m_Params->rangeMax = 255;
	m_Params->spacing[0]= 240./(float)(m_Params->size[0]-1);
	m_Params->spacing[1]= 240./(float)(m_Params->size[1]-1);
	m_Params->distance2 = 200.;

	FlipAxesArrayType flipArray;
	flipArray[0] = m_Params->flipAxis[0];
	flipArray[1] = m_Params->flipAxis[1];

	// missing part with rotAnglePoints...

	// distribute the volumes for the projection
	int quotient = (int)(1/num_volume );	
	int residual = 1 % num_volume;
	cout << quotient << ", " << residual << endl;		
	mvLib::VectorIntType numberOfProjections ( num_volume, quotient+1 );
	for (unsigned i=residual; i<num_volume; i++)
		numberOfProjections[i] =  numberOfProjections[i]-1;

	for (unsigned i=0; i<num_volume; i++)
		std::cout << numberOfProjections[i] << " " <<std::endl;


	//Project the volIdx(th) volume numberOfProjections[volIdx] times 
	// with different angles

	for (unsigned volIdx=0; volIdx<num_volume; volIdx++ )
	{
		SetUpImage();
		for (int projIdx=0; projIdx<numberOfProjections[volIdx]; projIdx++)
		{
			unsigned int ithAngle = projIdx*num_volume + volIdx;
			//unsigned int ithAngle = projIdx;
			std::cout << "\nProjecting the " << volIdx << "th volume to be " 
				<< ithAngle+1 << "th DRR image..." << std::endl;			

			// Compute a transform matrix based on the two angles
			mvLib::mvCameraRotationTransform::Pointer trans = mvLib::mvCameraRotationTransform::New();
			//trans->SetPrimaryAngle( rotAnglePoints[ithAngle][0] );
			//trans->SetSecondaryAngle( rotAnglePoints[ithAngle][1] );
			trans->SetPrimaryAngle( m_Params->angle1 );
			trans->SetSecondaryAngle( m_Params->angle2 );
			trans->Update();

			RotationMatrixType rotMat;
			mvLib::mvVTKToITKTransform::Pointer convertor = mvLib::mvVTKToITKTransform::New();
			convertor->SetVTKMatrix( trans->GetMatrix() );
			convertor->GetITKRotationMatrix( rotMat );

			TransformType::Pointer transform = TransformType::New();
			transform->SetTranslation( m_translation );
			transform->SetCenter(m_center);
			transform->SetRotationMatrix( rotMat );

			MvInterpolatorType::Pointer interpolator = MvInterpolatorType::New();
			interpolator->SetThreshold(m_Params->threshold);
			interpolator->SetFocalPoint(m_focalpoint);
			interpolator->SetTransform(transform);

			FilterType::Pointer filter = FilterType::New();
			filter->SetInput( m_image );
			filter->SetSize( m_size );
			filter->SetOutputSpacing( m_spacing );
			filter->SetOutputOrigin( m_origin );
			filter->SetDefaultPixelValue( 0 );
			filter->SetInterpolator( interpolator );
			filter->Update();

			// write image
			RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
			rescaler->SetOutputMinimum(m_Params->rangeMax-m_Params->rangeMin );
			rescaler->SetOutputMaximum( m_Params->rangeMax );
			rescaler->SetInput( filter->GetOutput() );

			InvertImageFilterType::Pointer inverter = InvertImageFilterType::New();
			inverter->SetInput( rescaler->GetOutput() );
			inverter->SetMaximum( m_Params->rangeMax );

			FlipFilterType::Pointer flip = FlipFilterType::New();
			flip->SetInput( inverter->GetOutput() );
			flip->SetFlipAxes( flipArray );

			typedef itk::ImageFileWriter< OutputImageType >  UnsignedCharWriterType;
			UnsignedCharWriterType::Pointer ucWriter = UnsignedCharWriterType::New();
			ucWriter->SetFileName("testpicDRR.png" );
			ucWriter->SetInput( flip->GetOutput() );
			ucWriter->Update();

			UpdateOutputImageAsVtk( 0, flip->GetOutput( ), "DRR" );//, false, 1, GetInputDataEntity( 0 ) );

			//UpdateOutputImageAsVtk(0, filter->GetOutput(),"DRR", false, 1, GetInputDataEntity( 0 ) );
		}

	}
#endif
}

/**
*/
void  DRRVisualizationProcessor::SetUpImage()
{
	InputImageType::PointType   imOrigin = m_image->GetOrigin();
	InputImageType::SpacingType imRes    = m_image->GetSpacing();
	InputImageRegionType imRegion = m_image->GetBufferedRegion();
	InputImageSizeType   imSize   = imRegion.GetSize();

//	imRes[0] = 1;
//	imRes[1] = 1;
//	imRes[2] = 1;

	imOrigin[0] += imRes[0] * static_cast<double>( imSize[0] ) / 2.0; 
	imOrigin[1] += imRes[1] * static_cast<double>( imSize[1] ) / 2.0; 
	imOrigin[2] += imRes[2] * static_cast<double>( imSize[2] ) / 2.0; 

	// center
	m_center[0] = m_Params->center[0] + imOrigin[0];
	m_center[1] = m_Params->center[1] + imOrigin[1];
	m_center[2] = m_Params->center[2] + imOrigin[2];

	// focal point
	m_focalpoint[0]= imOrigin[0];
	m_focalpoint[1]= imOrigin[1];
	m_focalpoint[2]= imOrigin[2] + m_Params->distance2;
	//m_focalpoint[2]= imOrigin[2] - m_Params->distance/2.;

	// size
	m_size[0] = m_Params->size[0];  // number of pixels along X of the 2D DRR image 
	m_size[1] = m_Params->size[1]; // number of pixels along Y of the 2D DRR image 
	m_size[2] = 1;

	//spacing
	m_spacing[0] = m_Params->spacing[0];  // pixel spacing along X of the 2D DRR image [mm]
	m_spacing[1] = m_Params->spacing[1];  // pixel spacing along Y of the 2D DRR image [mm]
	m_spacing[2] = 1.0; 

	//origin
	m_origin[0] = imOrigin[0] + m_Params->normal[0] - m_Params->spacing[0]*((double) m_Params->size[0] - 1.)/2.; 
	m_origin[1] = imOrigin[1] + m_Params->normal[0] - m_Params->spacing[1]*((double) m_Params->size[1]- 1.)/2.; 
	m_origin[2] = imOrigin[2] - (m_Params->distance - m_Params->distance2); 
	//origin[2] = imOrigin[2] + m_Params->distance/2.; 

	// translation
	m_translation[0] = m_Params->translation[0];
	m_translation[1] = m_Params->translation[1];
	m_translation[2] = m_Params->translation[2];
}

/**
*/
void DRRVisualizationProcessor::SetInputImage( InputImageType::Pointer image)
{
	m_image = image;
}



/**
*/
DRRParamsPtr DRRVisualizationProcessor::GetParameters()
{
	return m_Params;
}

DRRVisualizationProcessor::InputImageType::Pointer 
DRRVisualizationProcessor::GenerateITKImageFromBMP(
	unsigned int num_volume )
{

	std::string volFileFormat = ".\\tmask\\tmask";
	int stardIndex = 1;
	unsigned int indexOffset = 1;
	int slice_number = 128;

	//----------------------------------------------------------------------
	// generate the file names		   
	//----------------------------------------------------------------------
	typedef itk::NumericSeriesFileNames NumSerFNM;
	NumSerFNM::Pointer volume_filenames_gen = NumSerFNM::New(); 
	volume_filenames_gen->SetSeriesFormat( volFileFormat.c_str() );
	volume_filenames_gen->SetStartIndex( stardIndex );
	volume_filenames_gen->SetEndIndex( (num_volume-1)*indexOffset+stardIndex );
	std::vector<std::string > volume_fln = volume_filenames_gen->GetFileNames();	

	//temporarily added because FBP images from JMP phantoms are inversed, at least
	//typedef itk::FlipImageFilter< InputImageType > FlipImageFilterType;
	//typedef FlipImageFilterType::FlipAxesArrayType FlipAxesArrayType;
	//\temporarily added

	//Perform the projection
	for (unsigned volIdx=0; volIdx<num_volume; volIdx++ )
		//for (int volIdx=0; volIdx<1; volIdx++ )
	{		
		// Read the volume data
		std::cout << "Processing Volume: " << volume_fln[volIdx*indexOffset].c_str() << std::endl;
		typedef itk::ImageSeriesReader< InputImageType >  SeriesReaderType;
		SeriesReaderType::Pointer reader = SeriesReaderType::New();
		//reader->SetFileName( volume_fln[volIdx*indexOffset].c_str() );
		// ADDED BY NACHO
		std::string ss;
		for (int i = 0;i< slice_number;i++)
		{
			char nn[256];
			sprintf(nn, "%s.%03d.bmp", volume_fln[volIdx*indexOffset].c_str(), i);
			//std::cout << nn << std::endl;
			ss = nn;
			reader->AddFileName( ss.c_str() );
		}
		// END ADED BY NACHO

		try 
		{ 
			reader->Update();
		} 
		catch( itk::ExceptionObject & err ) 
		{ 
			std::cerr << "ERROR: ExceptionObject caught !" << std::endl; 
			std::cerr << err << std::endl; 
			return NULL;
		} 

		InputImageType::Pointer image = reader->GetOutput();

		itk::ImageFileWriter<InputImageType>::Pointer writer;
		writer = itk::ImageFileWriter<InputImageType>::New( );
		writer->SetFileName( "itkImage.vtk" );
		writer->SetInput( image );
		writer->Update( );

		return image;
	}

	return InputImageType::Pointer();
}
