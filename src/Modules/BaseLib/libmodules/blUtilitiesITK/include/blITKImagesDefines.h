/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blITKImagesDefines_h
#define _blITKImagesDefines_h

#include <itkImage.h>
#include <itkImageRegionIterator.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <vnl/vnl_math.h>
#include <vector>
#include <string>

/// \b Type                           \b Size
/// bool                                1 byte   \n
/// char, unsigned char, signed char 	1 byte   \n
/// short, unsigned short               2 bytes  \n
/// int, unsigned int                   4 bytes  \n
/// long, unsigned long                 4 bytes  \n
/// float                               4 bytes  \n
/// double                              8 bytes  \n
/// long double(*)                      8 bytes  \n
/// (*)   The representation of long double and double is identical. However,
/// long double and double are separate types.
///

namespace base {

	const int kDIM_3D =	3;
	const int kDIM_2D = 2;

	inline float DEG2RAD(float angleDeg) { return(angleDeg*vnl_math::pi/180.0f); };
	inline float RAD2DEG(float angleRad) { return(angleRad*180.0f/vnl_math::pi); };

	/*typedef unsigned char							itkPixelType;
	typedef unsigned short							itkVoxelType;
	typedef itk::Image< itkPixelType, kDIM_2D >		itkImage;
	typedef itk::Image< itkVoxelType, kDIM_3D >		itkVolume;*/
	//typedef itkVolume::SizeType<itkVoxelType,3>		itkVolumeSize;
	//typedef itkVolume::RegionType<itkVoxelType,3>	itkVolumeRegion;

	//VectorType
	typedef std::vector<int> VectorIntType;
	typedef std::vector<VectorIntType> VectorVectorIntType;
	
	typedef std::vector<float> VectorFloatType;
	typedef std::vector<VectorFloatType> VectorVectorFloatType;
	
	typedef std::vector<double> VectorDoubleType;
	typedef std::vector<VectorDoubleType> VectorVectorDoubleType;

	typedef std::vector<unsigned int> VectorUIntType;
	typedef std::vector<VectorUIntType> VectorVectorUIntType;

	typedef std::vector< std::string > VectorStringType;
	
	// PixelType
	typedef unsigned char	UnsignedCharPixelType;
	typedef char 			CharPixelType;
	typedef unsigned short	UnsignedShortPixelType;
	typedef short			ShortPixelType;
	typedef unsigned int	UnsignedIntPixelType;
	typedef int				IntPixelType;
	typedef long			LongPixelType;
	typedef float			RealPixelType;
	typedef double			DoublePixelType;
	typedef itk::Vector< RealPixelType, base::kDIM_3D > RealVectorPixelType;

	// NumericTraits
	typedef itk::NumericTraits<RealPixelType>	RealTraits;

	// ImageType
	typedef itk::Image < UnsignedCharPixelType,  base::kDIM_2D > UnsignedCharImageType;
	typedef itk::Image < CharPixelType,          base::kDIM_2D > CharImageType;
	typedef itk::Image < UnsignedShortPixelType, base::kDIM_2D > UnsignedShortImageType;
	typedef itk::Image < ShortPixelType,         base::kDIM_2D > ShortImageType;
	typedef itk::Image < UnsignedIntPixelType,	 base::kDIM_2D > UnsignedIntImageType;
	typedef itk::Image < IntPixelType,			 base::kDIM_2D > IntImageType;
	typedef itk::Image < LongPixelType,			 base::kDIM_2D > LongImageType;
	typedef itk::Image < RealPixelType,          base::kDIM_2D > RealImageType;
	typedef itk::Image < DoublePixelType,        base::kDIM_2D > DoubleImageType;
	typedef itk::Image < RealVectorPixelType,    base::kDIM_2D > RealVectorImageType;

	// VolumeType
	typedef itk::Image < UnsignedCharPixelType,  base::kDIM_3D > UnsignedCharVolumeType;
	typedef itk::Image < CharPixelType,          base::kDIM_3D > CharVolumeType;
	typedef itk::Image < UnsignedShortPixelType, base::kDIM_3D > UnsignedShortVolumeType;
	typedef itk::Image < ShortPixelType,         base::kDIM_3D > ShortVolumeType;
	typedef itk::Image < UnsignedIntPixelType,   base::kDIM_3D > UnsignedIntVolumeType;
	typedef itk::Image < IntPixelType,           base::kDIM_3D > IntVolumeType;
	typedef itk::Image < LongPixelType,			 base::kDIM_3D > LongVolumeType;
	typedef itk::Image < RealPixelType,          base::kDIM_3D > RealVolumeType;
	typedef itk::Image < DoublePixelType,        base::kDIM_3D > DoubleVolumeType;
	typedef itk::Image < RealVectorPixelType,    base::kDIM_3D > RealVectorVolumeType;

	//// IteratorType
	//typedef itk::ImageRegionIterator < UnsignedCharImageType >		UnsignedCharIteratorType;
	//typedef itk::ImageRegionIterator < CharImageType >				CharIteratorType;
	//typedef itk::ImageRegionIterator < UnsignedShortImageType >		UnsignedShortIteratorType;
	//typedef itk::ImageRegionIterator < ShortImageType >				ShortIteratorType;
	//typedef itk::ImageRegionIterator < RealImageType >				RealIteratorType;
	//typedef itk::ImageRegionIterator < DoubleImageType >			DoubleIteratorType;
	//typedef itk::ImageRegionIterator < RealVectorImageType >		RealVectorIteratorType;

	//typedef itk::NeighborhoodIterator < UnsignedCharImageType >		UnsignedCharNeighborhoodIteratorType;
	//typedef itk::NeighborhoodIterator < CharImageType >				CharNeighborhoodIteratorType;
	//typedef itk::NeighborhoodIterator < UnsignedShortImageType >	UnsignedShortNeighborhoodIteratorType;
	//typedef itk::NeighborhoodIterator < ShortImageType >			ShortNeighborhoodIteratorType;
	//typedef itk::NeighborhoodIterator < RealImageType >				RealNeighborhoodIteratorType;
	//typedef itk::NeighborhoodIterator < DoubleImageType >			DoubleNeighborhoodIteratorType;
	//typedef itk::NeighborhoodIterator < RealVectorImageType >		RealVectorNeighborhoodIteratorType;

	//typedef itk::ConstNeighborhoodIterator < RealImageType >		RealConstNeighborhoodIteratorType;

	// IndexType
	// 2D
	typedef UnsignedCharImageType::IndexType	UnsignedCharImageIndexType;
	typedef CharImageType::IndexType			CharImageIndexType;
	typedef UnsignedShortImageType::IndexType	UnsignedShortImageIndexType;
	typedef ShortImageType::IndexType			ShortImageIndexType;
	typedef UnsignedIntImageType::IndexType		UnsignedIntImageIndexType;
	typedef IntImageType::IndexType				IntImageIndexType;
	typedef LongImageType::IndexType			LongImageIndexType;
	typedef RealImageType::IndexType			RealImageIndexType;
	typedef DoubleImageType::IndexType			DoubleImageIndexType;

	// 3D
	typedef UnsignedCharVolumeType::IndexType	UnsignedCharVolumeIndexType;
	typedef CharVolumeType::IndexType			CharVolumeIndexType;
	typedef UnsignedShortVolumeType::IndexType	UnsignedShortVolumeIndexType;
	typedef ShortVolumeType::IndexType			ShortVolumeIndexType;
	typedef UnsignedIntVolumeType::IndexType	UnsignedIntVolumeIndexType;
	typedef IntVolumeType::IndexType			IntVolumeIndexType;
	typedef LongVolumeType::IndexType			LongVolumeIndexType;
	typedef RealVolumeType::IndexType			RealVolumeIndexType;
	typedef DoubleVolumeType::IndexType			DoubleVolumeIndexType;

	// Iterator
	// 2D
	typedef itk::ImageRegionIterator<UnsignedCharImageType>		UnsignedCharImageIteratorType;
	typedef itk::ImageRegionIterator<CharImageType>				CharImageIteratorType;
	typedef itk::ImageRegionIterator<UnsignedShortImageType>	UnsignedShortImageIteratorType;
	typedef itk::ImageRegionIterator<ShortImageType>			ShortImageIteratorType;
	typedef itk::ImageRegionIterator<UnsignedIntImageType>		UnsignedIntImageIteratorType;
	typedef itk::ImageRegionIterator<IntImageType>				IntImageIteratorType;
	typedef itk::ImageRegionIterator<LongImageType>				LongImageIteratorType;
	typedef itk::ImageRegionIterator<RealImageType>				RealImageIteratorType;
	typedef itk::ImageRegionIterator<DoubleImageType>			DoubleImageIteratorType;

	// 3D
	typedef itk::ImageRegionIterator<UnsignedCharVolumeType>	UnsignedCharVolumeIteratorType;
	typedef itk::ImageRegionIterator<CharVolumeType>			CharVolumeIteratorType;
	typedef itk::ImageRegionIterator<UnsignedShortVolumeType>	UnsignedShortVolumeIteratorType;
	typedef itk::ImageRegionIterator<ShortVolumeType>			ShortVolumeIteratorType;
	typedef itk::ImageRegionIterator<IntVolumeType>				IntVolumeIteratorType;
	typedef itk::ImageRegionIterator<UnsignedIntVolumeType>		UnsignedIntVolumeIteratorType;
	typedef itk::ImageRegionIterator<LongVolumeType>			LongVolumeIteratorType;
	typedef itk::ImageRegionIterator<RealVolumeType>			RealVolumeIteratorType;
	typedef itk::ImageRegionIterator<DoubleVolumeType>			DoubleVolumeIteratorType;

	// RegionType
	// 2D
	typedef UnsignedCharImageType::RegionType		UnsignedCharImageRegionType;
	typedef CharImageType::RegionType				CharImageRegionType;
	typedef UnsignedShortImageType::RegionType		UnsignedShortImageRegionType;
	typedef ShortImageType::RegionType				ShortImageRegionType;
	typedef UnsignedIntImageType::RegionType		UnsignedIntImageRegionType;
	typedef IntImageType::RegionType				IntImageRegionType;
	typedef LongImageType::RegionType				LongImageRegionType;
	typedef RealImageType::RegionType				RealImageRegionType;
	typedef DoubleImageType::RegionType				DoubleImageRegionType;
			
	// 3D	
	typedef UnsignedCharVolumeType::RegionType		UnsignedCharVolumeRegionType;
	typedef CharVolumeType::RegionType				CharVolumeRegionType;
	typedef UnsignedShortVolumeType::RegionType		UnsignedShortVolumeRegionType;
	typedef ShortVolumeType::RegionType				ShortVolumeRegionType;
	typedef UnsignedIntVolumeType::RegionType		UnsignedIntVolumeRegionType;
	typedef IntVolumeType::RegionType				IntVolumeRegionType;
	typedef LongVolumeType::RegionType				LongVolumeRegionType;
	typedef RealVolumeType::RegionType				RealVolumeRegionType;
	typedef DoubleVolumeType::RegionType			DoubleVolumeRegionType;

	// SizeType
	// 2D
	typedef UnsignedCharImageType::SizeType		UnsignedCharImageSizeType;
	typedef CharImageType::SizeType				CharImageSizeType;
	typedef UnsignedShortImageType::SizeType	UnsignedShortImageSizeType;
	typedef ShortImageType::SizeType			ShortImageSizeType;
	typedef UnsignedIntImageType::SizeType		UnsignedIntImageSizeType;
	typedef IntImageType::SizeType				IntImageSizeType;
	typedef LongImageType::SizeType				LongImageSizeType;
	typedef RealImageType::SizeType				RealImageSizeType;
	typedef DoubleImageType::SizeType			DoubleImageSizeType;
			
	// 3D	
	typedef UnsignedCharVolumeType::SizeType	UnsignedCharVolumeSizeType;
	typedef CharVolumeType::SizeType			CharVolumeSizeType;
	typedef UnsignedShortVolumeType::SizeType	UnsignedShortVolumeSizeType;
	typedef ShortVolumeType::SizeType			ShortVolumeSizeType;
	typedef UnsignedIntVolumeType::SizeType		UnsignedIntVolumeSizeType;
	typedef IntVolumeType::SizeType				IntVolumeSizeType;
	typedef LongVolumeType::SizeType			LongVolumeSizeType;
	typedef RealVolumeType::SizeType			RealVolumeSizeType;
	typedef DoubleVolumeType::SizeType			DoubleVolumeSizeType;


	// IO
	// Readers 2D
	typedef itk::ImageFileReader < UnsignedCharImageType >		UnsignedCharImageReaderType;
	typedef itk::ImageFileReader < CharImageType>				CharImageReaderType;
	typedef itk::ImageFileReader < UnsignedShortImageType >		UnsignedShortImageReaderType;
	typedef itk::ImageFileReader < ShortImageType >				ShortImageReaderType;
	typedef itk::ImageFileReader < UnsignedIntImageType >		UnsignedIntImageReaderType;
	typedef itk::ImageFileReader < IntImageType >				IntImageReaderType;
	typedef itk::ImageFileReader < LongImageType >				LongImageReaderType;
	typedef itk::ImageFileReader < RealImageType > 				RealImageReaderType;
	typedef itk::ImageFileReader < DoubleImageType > 			DoubleImageReaderType;
	typedef itk::ImageFileReader < RealVectorImageType >		RealVectorImageReaderType;

	// Readers 3D
	typedef itk::ImageFileReader < UnsignedCharVolumeType >		UnsignedCharVolumeReaderType;
	typedef itk::ImageFileReader < CharVolumeType>				CharVolumeReaderType;
	typedef itk::ImageFileReader < UnsignedShortVolumeType >	UnsignedShortVolumeReaderType;
	typedef itk::ImageFileReader < ShortVolumeType >			ShortVolumeReaderType;
	typedef itk::ImageFileReader < UnsignedIntVolumeType >		UnsignedIntVolumeReaderType;
	typedef itk::ImageFileReader < IntVolumeType >				IntVolumeReaderType;
	typedef itk::ImageFileReader < LongImageType >				LongImageReaderType;
	typedef itk::ImageFileReader < RealVolumeType > 			RealVolumeReaderType;
	typedef itk::ImageFileReader < DoubleVolumeType > 			DoubleVolumeReaderType;
	typedef itk::ImageFileReader < RealVectorVolumeType >		RealVectorVolumeReaderType;

	// Writers 2D
	typedef itk::ImageFileWriter < UnsignedCharImageType >		UnsignedCharImageWriterType;
	typedef itk::ImageFileWriter < CharImageType >				CharImageWriterType;
	typedef itk::ImageFileWriter < UnsignedShortImageType >		UnsignedShortImageWriterType;
	typedef itk::ImageFileWriter < ShortImageType >				ShortImageWriterType;
	typedef itk::ImageFileWriter < UnsignedIntImageType >		UnsignedIntImageWriterType;
	typedef itk::ImageFileWriter < IntImageType >				IntImageWriterType;
	typedef itk::ImageFileWriter < LongImageType >				LongImageWriterType;
	typedef itk::ImageFileWriter < RealImageType >				RealImageWriterType;
	typedef itk::ImageFileWriter < DoubleImageType >			DoubleImageWriterType;
	typedef itk::ImageFileWriter < RealVectorImageType >		RealVectorImageWriterType;

	// Writers 3D
	typedef itk::ImageFileWriter < UnsignedCharVolumeType >		UnsignedCharVolumeWriterType;
	typedef itk::ImageFileWriter < CharVolumeType >				CharVolumeWriterType;
	typedef itk::ImageFileWriter < UnsignedShortVolumeType >	UnsignedShortVolumeWriterType;
	typedef itk::ImageFileWriter < ShortVolumeType >			ShortVolumeWriterType;
	typedef itk::ImageFileWriter < UnsignedIntVolumeType >		UnsignedIntVolumeWriterType;
	typedef itk::ImageFileWriter < IntVolumeType >				IntVolumeWriterType;
	typedef itk::ImageFileWriter < LongVolumeType >				LongVolumeWriterType;
	typedef itk::ImageFileWriter < RealVolumeType >				RealVolumeWriterType;
	typedef itk::ImageFileWriter < DoubleVolumeType >			DoubleVolumeWriterType;
	typedef itk::ImageFileWriter < RealVectorVolumeType >		RealVectorVolumeWriterType;


	typedef std::vector<base::RealVolumeType::Pointer> RealVolumeVectorType;

} // end namespace base

#endif // _blITKImagesDefines_h
