/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef coreDRRVisualizationProcessor_H
#define coreDRRVisualizationProcessor_H

#include "corePluginMacros.h"
#include "coreBaseFilter.h"

#include "coreVTKPolyDataHolder.h"
#include "coreVTKImageDataHolder.h"
#include "coreDRRParams.h"
#include "coreBaseProcessor.h"

#include "itkResampleImageFilter.h"
#include "itkCenteredEuler3DTransform.h"
#include "itkRigid3DTransform.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkInvertIntensityImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkNormalizeImageFilter.h"
#include "itkVTKImageIO.h"
#include "itkGDCMImageIO.h"
#include "itkFlipImageFilter.h"
#include "itkNumericSeriesFileNames.h"
#include "itkRayCastInterpolateImageFunction.h"
#include "itkImageFileWriter.h"

//#include "GIMIAS_OS.h"

#ifdef _USE_MVIEWLIB
#include "mvCameraRotationTransform.h"
#include "mvVTKToITKTransform.h"
#include "mvITKImagesDefines.h"
#include "RayCastInterpolateImageFunction.h"
#endif


namespace Core
{
/**
\brief Processor class that creates an image using DRR algorithm.

\sa Core::DRRVisualization
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 27 Aug 2009
*/
class PLUGIN_EXPORT DRRVisualizationProcessor 
	: public Core::BaseProcessor
{
public:
	coreDeclareSmartPointerClassMacro(DRRVisualizationProcessor, Core::BaseProcessor);

	typedef   float  InputPixelType;
	typedef   unsigned char OutputPixelType;
	//typedef   double OutputPixelType;
	typedef   float	InternalPixelType;

	typedef itk::Image< InputPixelType, 3 >   InputImageType;
	typedef itk::Image< OutputPixelType, 3 >   OutputImageType;
	typedef itk::Image< InternalPixelType, 3 >   InternalImageType;

	typedef itk::ImageRegionIteratorWithIndex< InputImageType > IteratorType;
	typedef itk::ResampleImageFilter<InputImageType, InputImageType > FilterType;
	typedef itk::CenteredEuler3DTransform< double >  TransformType;
	typedef itk::Matrix<double, 3, 3> RotationMatrixType;
	typedef InputImageType::RegionType     InputImageRegionType;
	typedef InputImageRegionType::SizeType InputImageSizeType;
#ifdef _USE_MVIEWLIB
	typedef mvLib::RayCastInterpolateImageFunction<InputImageType,double> MvInterpolatorType;
#endif
	typedef itk::RayCastInterpolateImageFunction<InputImageType,double> InterpolatorType;
	typedef itk::NormalizeImageFilter < InternalImageType, 
		InternalImageType > NormalizeImageFilterType;
	typedef itk::RescaleIntensityImageFilter< InputImageType, 
		OutputImageType > RescaleFilterType;
	typedef itk::InvertIntensityImageFilter< OutputImageType , OutputImageType > InvertImageFilterType;
	typedef itk::FlipImageFilter< OutputImageType > FlipFilterType;
	typedef FlipFilterType::FlipAxesArrayType FlipAxesArrayType;

	typedef itk::InvertIntensityImageFilter<InputImageType, InputImageType> InvertIntensityFilterType;
	typedef itk::ImageFileWriter< InternalImageType > WriterType;

	//!
	void Update( );

	//!
	DRRParamsPtr GetParameters();

protected:
	DRRVisualizationProcessor(void);
	virtual ~DRRVisualizationProcessor(void);

	//!
	void SetInputImage( InputImageType::Pointer image);
	//!
	void SetUpImage();

	//!
	DRRVisualizationProcessor::InputImageType::Pointer GenerateITKImageFromBMP( 
		unsigned int num_volume );

private:
	coreDeclareNoCopyConstructors(DRRVisualizationProcessor);

	void CreateDRRUsingITK( );

	void CreateDRRUsingMview( );

	//! parameters for the algorithm 
	DRRParamsPtr m_Params;

	//! internal image
	InputImageType::Pointer m_image;

	//!
	TransformType::OutputVectorType m_translation;
	TransformType::InputPointType m_center;
	InterpolatorType::InputPointType m_focalpoint;
	InputImageType::SizeType   m_size;
	InputImageType::SpacingType m_spacing;
	double m_origin[ 3 ];
};

} // namespace Core

#endif

