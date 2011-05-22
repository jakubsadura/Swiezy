/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkPluginUtilities.h"
#include "AffineTransformCLP.h"

#include "itkImageRegistrationMethod.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkMattesMutualInformationImageToImageMetric.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "itkImage.h"


#include "itkCenteredTransformInitializer.h"

#include "itkImageMaskSpatialObject.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkSmoothingRecursiveGaussianImageFilter.h"
#include "itkTransformFileWriter.h"

#include "itkAffineTransform.h"


#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"


//
//  The following piece of code implements an observer
//  that will monitor the evolution of the registration process.
//
#include "itkCommand.h"
class CommandIterationUpdate : public itk::Command 
{
public:
  typedef  CommandIterationUpdate   Self;
  typedef  itk::Command             Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  itkNewMacro( Self );
protected:
  CommandIterationUpdate() {};
public:
  typedef itk::RegularStepGradientDescentOptimizer     OptimizerType;
  typedef   const OptimizerType   *    OptimizerPointer;

  void Execute(itk::Object *caller, const itk::EventObject & event)
  {
    Execute( (const itk::Object *)caller, event);
  }

  void Execute(const itk::Object * object, const itk::EventObject & event)
  {
    OptimizerPointer optimizer = 
                      dynamic_cast< OptimizerPointer >( object );
    if( ! itk::IterationEvent().CheckEvent( &event ) )
      {
      return;
      }
      std::cout << optimizer->GetCurrentIteration() << "   ";
      std::cout << optimizer->GetValue() << "   ";
      //std::cout << optimizer->GetCurrentPosition();
	  std::cout << std::endl;
     
    }
};

namespace {

	template<class T> int DoIt( int argc, char * argv[], T )
	{
		PARSE_ARGS;

		typedef    T       PixelType;
		typedef    T       PixelType;
		const    unsigned int    ImageDimension = 3;

		typedef itk::Image< PixelType,  ImageDimension >   FixedImageType;
		typedef itk::Image< PixelType,  ImageDimension >   MovingImageType;

		typedef itk::AffineTransform< double, ImageDimension > TransformType;

		typedef itk::RegularStepGradientDescentOptimizer       OptimizerType;

		typedef itk::MattesMutualInformationImageToImageMetric< 
			FixedImageType, 
			MovingImageType >    MetricType;
		typedef itk:: LinearInterpolateImageFunction< 
			MovingImageType,
			double          >    InterpolatorType;
		typedef itk::ImageRegistrationMethod< 
			FixedImageType, 
			MovingImageType >    RegistrationType;

		typename MetricType::Pointer         metric        = MetricType::New();
		OptimizerType::Pointer               optimizer     = OptimizerType::New();
		typename InterpolatorType::Pointer   interpolator  = InterpolatorType::New();
		typename RegistrationType::Pointer   registration  = RegistrationType::New();

		registration->SetMetric(        metric        );
		registration->SetOptimizer(     optimizer     );
		registration->SetInterpolator(  interpolator  );



		// Software Guide : BeginCodeSnippet
		TransformType::Pointer  transform = TransformType::New();
		registration->SetTransform( transform );
		// Software Guide : EndCodeSnippet


		typedef itk::ImageFileReader< FixedImageType  > FixedImageReaderType;
		typedef itk::ImageFileReader< MovingImageType > MovingImageReaderType;
		typename FixedImageReaderType::Pointer  fixedImageReader  = FixedImageReaderType::New();
		typename MovingImageReaderType::Pointer movingImageReader = MovingImageReaderType::New();
		fixedImageReader->SetFileName(  fixedImageFileName );
		movingImageReader->SetFileName( movingImageFileName );


		registration->SetFixedImage(    fixedImageReader->GetOutput()    );
		registration->SetMovingImage(   movingImageReader->GetOutput()   );


		try
		{
			fixedImageReader->Update();
			movingImageReader->Update();
		}
		catch( itk::ExceptionObject & excp )
		{
			std::cerr << "Error while reading input image files" << std::endl;
			std::cerr << excp << std::endl;
			std::cerr << "[FAILED]" << std::endl;
			return EXIT_FAILURE;
		}

		typename FixedImageType::ConstPointer fixedImage = fixedImageReader->GetOutput();
		typename MovingImageType::ConstPointer movingImage = movingImageReader->GetOutput();


		registration->SetFixedImageRegion(fixedImageReader->GetOutput()->GetBufferedRegion() );




		// Generate fixed image mask from thresh value if required
		typedef itk::ImageMaskSpatialObject<ImageDimension> FixedImageMaskType;
		typedef itk::ImageMaskSpatialObject<ImageDimension> MovingImageMaskType;
		typedef itk::BinaryThresholdImageFilter<FixedImageType,FixedImageMaskType::ImageType> FixedThresholdImageFilterType;
		typedef itk::BinaryThresholdImageFilter<MovingImageType,MovingImageMaskType::ImageType> MovingThresholdImageFilterType;


		if ( flaglowThreshFixed || flaghighThreshFixed )
		{
			typename FixedThresholdImageFilterType::Pointer fixedThresholdImageFilter = FixedThresholdImageFilterType::New();

			FixedImageMaskType::Pointer mask = FixedImageMaskType::New();

			fixedThresholdImageFilter->SetInput(fixedImage);

			if ( flaglowThreshFixed && flaghighThreshFixed)
			{
				fixedThresholdImageFilter->SetLowerThreshold(lowThreshFixed);
				fixedThresholdImageFilter->SetUpperThreshold(highThreshFixed);
			}
			else if ( flaglowThreshFixed )
			{
				fixedThresholdImageFilter->SetLowerThreshold(lowThreshFixed);
				fixedThresholdImageFilter->SetUpperThreshold(itk::NumericTraits<PixelType>::max());
			}
			else if ( flaghighThreshFixed )
			{
				fixedThresholdImageFilter->SetUpperThreshold(highThreshFixed);
				fixedThresholdImageFilter->SetLowerThreshold(itk::NumericTraits<PixelType>::min());
			}
			fixedThresholdImageFilter->Update();

			mask->SetImage(fixedThresholdImageFilter->GetOutput());
			metric->SetFixedImageMask(mask);
		}


		// Generate moving image mask from thresh value if required
		if ( flaglowThreshMoving || flaghighThreshMoving )
		{
			typename MovingThresholdImageFilterType::Pointer movingThresholdImageFilter = MovingThresholdImageFilterType::New();

			MovingImageMaskType::Pointer mask = MovingImageMaskType::New();

			movingThresholdImageFilter->SetInput(movingImage);

			if ( flaglowThreshMoving && flaghighThreshMoving)
			{
				movingThresholdImageFilter->SetLowerThreshold(lowThreshMoving);
				movingThresholdImageFilter->SetUpperThreshold(highThreshMoving);
			}
			else if ( flaglowThreshMoving )
			{
				movingThresholdImageFilter->SetLowerThreshold(lowThreshMoving);
				movingThresholdImageFilter->SetUpperThreshold(itk::NumericTraits<PixelType>::max());
			}
			else if ( flaghighThreshMoving )
			{
				movingThresholdImageFilter->SetUpperThreshold(highThreshMoving);
				movingThresholdImageFilter->SetLowerThreshold(itk::NumericTraits<PixelType>::min());
			}
			movingThresholdImageFilter->Update();
			mask->SetImage(movingThresholdImageFilter->GetOutput());
			metric->SetMovingImageMask(mask);
		}



		//read and set mask
		if ( !maskFileName.empty( ) ){
			std::cout << "reading mask "  << std::endl;
			typedef itk::ImageMaskSpatialObject<ImageDimension> ImageMaskTypeF;
			ImageMaskTypeF::Pointer  spatialObjectMask = ImageMaskTypeF::New();
			typedef ImageMaskTypeF::ImageType   ImageMaskType;
			typedef itk::ImageFileReader< ImageMaskType >    MaskReaderType;
			MaskReaderType::Pointer  maskReader = MaskReaderType::New();
			maskReader->SetFileName( maskFileName );

			try 
			{ 
				maskReader->Update(); 
			} 
			catch( itk::ExceptionObject & err ) 
			{ 
				std::cerr << "ExceptionObject caught !" << std::endl; 
				std::cerr << err << std::endl; 
				return EXIT_FAILURE;
			}

			spatialObjectMask->SetImage( maskReader->GetOutput() );
			metric->SetFixedImageMask( spatialObjectMask );
		}


		typename FixedImageType::RegionType fixedRegion = fixedImage->GetBufferedRegion();

		typename FixedImageType::IndexType index;
		typename FixedImageType::SizeType size;

		bool emptyRegion = true;
		for ( int i = 0 ; i < region.size( ) ; i++ )
		{
			emptyRegion = emptyRegion && region[ i ] == 0;
		}

		if ( emptyRegion )
		{
			std::cerr << " Warning: Unspecified userSpecifiedRegion - Using the whole imageRegion as userSpecifiedRegion" << std::endl;
			index = fixedImage->GetLargestPossibleRegion().GetIndex();
			size  = fixedImage->GetLargestPossibleRegion().GetSize();
		}
		else
		{
			for (unsigned int r=0; r<ImageDimension; r++)
			{
				index[r] = region[r];
				size[r]  = region[r+ImageDimension];
			}
		}

		fixedRegion.SetSize(size);
		fixedRegion.SetIndex(index);

		std::cout << "index " << index[0] << " " << index[1] << " " << index[2] << " " << std::endl;
		std::cout << "size  " << size[0] << " " << size[1] << " " << size[2] << " " << std::endl;

		registration->SetFixedImageRegion( fixedRegion );


		// Smoothing images
		if ( sigma_smooth != 0 )
		{
			std::cout << "Smoothing fixed image   " << "sigma = " << sigma_smooth << std::endl;
			typedef itk::SmoothingRecursiveGaussianImageFilter<FixedImageType,FixedImageType> Fixed_SmootherType;
			typedef itk::SmoothingRecursiveGaussianImageFilter<MovingImageType,MovingImageType> Moving_SmootherType;
			typename Fixed_SmootherType::Pointer fixed_smoother = Fixed_SmootherType::New();
			typename Moving_SmootherType::Pointer moving_smoother = Moving_SmootherType::New();

			fixed_smoother->SetInput( fixedImage );
			fixed_smoother->SetSigma( sigma_smooth );
			try 
			{ 
				fixed_smoother->Update();
			} 
			catch( itk::ExceptionObject & err ) 
			{ 
				std::cerr << "ExceptionObject caught !" << std::endl; 
				std::cerr << err << std::endl; 
				return EXIT_FAILURE;
			}
			registration->SetFixedImage(  fixed_smoother->GetOutput()   );

			moving_smoother->SetInput( movingImage );
			moving_smoother->SetSigma( sigma_smooth );
			try 
			{ 
				moving_smoother->Update();
			} 
			catch( itk::ExceptionObject & err ) 
			{ 
				std::cerr << "ExceptionObject caught !" << std::endl; 
				std::cerr << err << std::endl; 
				return EXIT_FAILURE;
			}
			registration->SetMovingImage(  moving_smoother->GetOutput()  );
		}
		else
		{
			registration->SetFixedImage(  fixedImage   );
			registration->SetMovingImage(  movingImage  );
		}


		// Software Guide : BeginCodeSnippet

		if(initializeTrans) {
			typedef itk::CenteredTransformInitializer< 
				TransformType, 
				FixedImageType, 
				MovingImageType >  TransformInitializerType;
			typename TransformInitializerType::Pointer initializer = TransformInitializerType::New();
			initializer->SetTransform(   transform );
			initializer->SetFixedImage(  fixedImageReader->GetOutput() );
			initializer->SetMovingImage( movingImageReader->GetOutput() );
			initializer->MomentsOn();
			initializer->InitializeTransform();
		}else{
			transform->SetIdentity();
		}

		// Software Guide : EndCodeSnippet

		// Software Guide : BeginCodeSnippet
		registration->SetInitialTransformParameters( 
			transform->GetParameters() );
		// Software Guide : EndCodeSnippet


		double translationScale = 1.0 / 1000.0;



		// Software Guide : BeginCodeSnippet
		typedef OptimizerType::ScalesType       OptimizerScalesType;
		OptimizerScalesType optimizerScales( transform->GetNumberOfParameters() );

		optimizerScales[0] = 1.0;
		optimizerScales[1] = 1.0;
		optimizerScales[2] = 1.0;
		optimizerScales[3] = 1.0;
		optimizerScales[4] = 1.0;
		optimizerScales[5] = 1.0;
		optimizerScales[6] = 1.0;
		optimizerScales[7] = 1.0;
		optimizerScales[8] = 1.0;

		optimizerScales[9]  = translationScale;
		optimizerScales[10] = translationScale;
		optimizerScales[11] = translationScale;

		optimizer->SetScales( optimizerScales );
		// Software Guide : EndCodeSnippet


		// Software Guide : BeginCodeSnippet
		optimizer->SetMaximumStepLength( nb_maxSteplng ); 
		optimizer->SetMinimumStepLength( nb_minSteplng );
		optimizer->SetNumberOfIterations( nb_Itr );
		// Software Guide : EndCodeSnippet


		// Software Guide : BeginCodeSnippet
		optimizer->MinimizeOn();
		// Software Guide : EndCodeSnippet


		// Create the Command observer and register it with the optimizer.
		//
		if(add_observer){
			CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
			optimizer->AddObserver( itk::IterationEvent(), observer );
		}

		metric->SetNumberOfHistogramBins( nb_Bins );
		const unsigned int numberOfSamples = fixedRegion.GetNumberOfPixels() / frac_S;
		metric->SetNumberOfSpatialSamples( numberOfSamples );
		metric->ReinitializeSeed( 76926294 );


		// Software Guide : BeginCodeSnippet
		try 
		{ 
			registration->StartRegistration(); 
		} 
		catch( itk::ExceptionObject & err ) 
		{ 
			std::cerr << "ExceptionObject caught !" << std::endl; 
			std::cerr << err << std::endl; 
			return EXIT_FAILURE;
		} 
		// Software Guide : EndCodeSnippet


		// Software Guide : BeginCodeSnippet
		OptimizerType::ParametersType finalParameters = 
			registration->GetLastTransformParameters();

        /*
		const unsigned int numberOfIterations = optimizer->GetCurrentIteration();
		const double bestValue = optimizer->GetValue();
        */
		// Software Guide : EndCodeSnippet

		TransformType::Pointer finalTransform = TransformType::New();

		finalTransform->SetCenter( transform->GetCenter() );
		finalTransform->SetParameters( finalParameters );


		// Writing transformation
		if( !OutputTransform.empty( ) ){
			std::cout << std::endl << "Writing Transformation..." << std::endl;

			typedef itk::TransformFileWriter TrWriter;

			TrWriter::Pointer trwr = TrWriter::New();
			trwr->SetInput(finalTransform);
			trwr->SetFileName( OutputTransform );
			try
			{
				trwr->Update();
			}
			catch( itk::ExceptionObject & err )
			{
				std::cerr << "ExceptionObject caught !" << std::endl;
				std::cerr << err << std::endl;
				return -1;
			} 
		}

		// Writing output image
		if( !outputImage.empty( ) ){
			std::cout << std::endl << "Writing output image..." << std::endl;

			typedef itk::ResampleImageFilter< 
				MovingImageType, 
				FixedImageType >    ResampleFilterType;

			typename ResampleFilterType::Pointer resampler = ResampleFilterType::New();

			resampler->SetTransform( finalTransform );
			resampler->SetInput( movingImageReader->GetOutput() );
			resampler->SetSize(    fixedImage->GetLargestPossibleRegion().GetSize() );
			resampler->SetOutputOrigin(  fixedImage->GetOrigin() );
			resampler->SetOutputSpacing( fixedImage->GetSpacing() );
			resampler->SetOutputDirection( fixedImage->GetDirection() );
			resampler->SetDefaultPixelValue( paddingValue );

			typedef itk::Image< PixelType, ImageDimension > OutputImageType;
			typedef itk::ImageFileWriter< OutputImageType >  WriterType;

			typename WriterType::Pointer      writer =  WriterType::New();

			writer->SetFileName( outputImage );
			writer->SetInput( resampler->GetOutput()   );
			writer->Update();
		}


		return EXIT_SUCCESS;
	}


} // end of anonymous namespace


int main( int argc, char *argv[] )
{
	PARSE_ARGS;

	itk::ImageIOBase::IOPixelType pixelType;
	itk::ImageIOBase::IOComponentType componentType;
	try
	{
		itk::GetImageType (fixedImageFileName, pixelType, componentType);

		// This filter handles all types on input, but only produces
		// signed types
		switch (componentType)
		{
		case itk::ImageIOBase::UCHAR:
			return DoIt( argc, argv, static_cast<unsigned char>(0));
			break;
		case itk::ImageIOBase::CHAR:
			return DoIt( argc, argv, static_cast<char>(0));
			break;
		case itk::ImageIOBase::USHORT:
			return DoIt( argc, argv, static_cast<unsigned short>(0));
			break;
		case itk::ImageIOBase::SHORT:
			return DoIt( argc, argv, static_cast<short>(0));
			break;
		case itk::ImageIOBase::UINT:
			return DoIt( argc, argv, static_cast<unsigned int>(0));
			break;
		case itk::ImageIOBase::INT:
			return DoIt( argc, argv, static_cast<int>(0));
			break;
		case itk::ImageIOBase::ULONG:
			return DoIt( argc, argv, static_cast<unsigned long>(0));
			break;
		case itk::ImageIOBase::LONG:
			return DoIt( argc, argv, static_cast<long>(0));
			break;
		case itk::ImageIOBase::FLOAT:
			return DoIt( argc, argv, static_cast<float>(0));
			break;
		case itk::ImageIOBase::DOUBLE:
			return DoIt( argc, argv, static_cast<double>(0));
			break;
		case itk::ImageIOBase::UNKNOWNCOMPONENTTYPE:
		default:
			std::cout << "unknown component type" << std::endl;
			break;
		}
	}

	catch( itk::ExceptionObject &excep)
	{
		std::cerr << argv[0] << ": exception caught !" << std::endl;
		std::cerr << excep << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
